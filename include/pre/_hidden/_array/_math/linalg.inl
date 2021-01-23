/*-*- C++ -*-*/
#pragma once

namespace pre {

template <
        concepts::floating_point_or_complex Field,
        typename Alloc = std::allocator<Field>>
struct Linalg {
  public:
    using Float = to_floating_point_t<Field>;
    using FloatLimits = pre::numeric_limits<Float>;

  private:
    /// A local cache object to prevent excessive heap allocations.
    struct Cache {
        auto scoped_push() {
            return mem_stack_.scoped_push();
        }
        void clear() noexcept {
            mem_stack_.clear();
        }
        template <typename T>
        VecView<T> vector(ssize_t n) {
            T* ptr = static_cast<T*>(mem_stack_.allocate(sizeof(T) * n));
            return {ptr, n};
        }
        template <typename T>
        MatView<T> matrix(ssize_t m, ssize_t n) {
            T* ptr = static_cast<T*>(mem_stack_.allocate(sizeof(T) * m * n));
            const ssize_t ssizes[] = {m, n};
            return {ptr, &ssizes[0]};
        }

      private:
        MemStack<Alloc> mem_stack_;
    };

    /// Local cache.
    Cache cache_;

  private:
    /// Expect condition at runtime, else throw `std::runtime_error`.
    static void expect(bool cond, const char* what) {
        if (!cond)
            throw std::runtime_error(what);
    }

  public:
    /// \name Miscellaneous
    /** \{ */

    /// Load identity into matrix.
    static constexpr void load_identity(MatView<Field> a) noexcept {
        a = Field(0);
        a.diag() = Field(1);
    }

    /// Load iota into vector.
    static constexpr void load_iota(VecView<int> v) noexcept {
        int n = 0;
        for (int& i : v)
            i = n++;
    }

    /// Force matrix to be upper triangular (set lower triangle to zero).
    static constexpr void force_upper_triangular(MatView<Field> a) noexcept {
        int m = a.rows();
        int n = a.cols();
        for (int i = 1; i < m; i++)
            a(i, Slice(0, pre::min(i, n))) = Field(0);
    }

    /// Force matrix to be lower triangular (set upper triangle to zero).
    static constexpr void force_lower_triangular(MatView<Field> a) noexcept {
        int m = a.rows();
        int n = a.cols();
        for (int j = 1; j < n; j++)
            a(Slice(0, pre::min(j, m)), j) = Field(0);
    }

    /// Take adjoint in-place.
    static constexpr void do_adjoint(MatView<Field> a) noexcept {
        ASSERT(a.is_square());
        const int m = a.rows();
        for (int i = 0; i < m; i++) {
            a(i, i) = pre::conj(a(i, i));
            for (int j = i + 1; j < m; j++) {
                auto a0 = a(i, j);
                auto a1 = a(j, i);
                a(i, j) = pre::conj(a1);
                a(j, i) = pre::conj(a0);
            }
        }
    }

    static int find_pivot(VecView<Field> v) noexcept {
        Float tmp_max = 0;
        int piv = 0;
        int n = v.size();
        for (int k = 0; k < n; k++) {
            Float tmp = pre::abs(v[k]);
            if (tmp_max < tmp) {
                tmp_max = tmp;
                piv = k;
            }
        }
        return piv;
    }

    template <typename Lhs, concepts::floating_point_or_complex Rhs>
    static constexpr bool robust_div(Lhs&& lhs, Rhs rhs) noexcept {
        if (Float tmp = pre::abs(rhs); tmp > 0) {
            if (tmp >= Float(2) * FloatLimits::min_invertible())
                lhs *= Float(1) / rhs;
            else
                lhs /= rhs;
            return true;
        }
        else {
            return false;
        }
    }

    /** \} */

  public:
    /// \name Norms
    /** \{ */

    /// Calculate \f$ L^1 \f$ norm.
    Float norm1(VecView<const Field> v) noexcept {
        return pre::sum(pre::abs(*v));
    }

    /// Calculate \f$ L^2 \f$ norm.
    Float norm2(VecView<const Field> v) noexcept {
        int n = v.size();
        switch (n) {
        case 0: return 0;
        case 1: return std::abs(v[0]);
        case 2: return std::hypot(std::abs(v[0]), std::abs(v[1]));
        default: break;
        }
        auto _ = cache_.scoped_push();
        auto absv = cache_.template vector<Float>(n);
        absv = pre::abs(*v);
        auto absv_max = *std::max_element(absv.begin(), absv.end());
        if (absv_max == 0)
            return 0;
        else if (
                absv_max * absv_max * n > FloatLimits::max() ||
                absv_max < FloatLimits::min_squarable()) {
            robust_div(absv, absv_max);
            return pre::sqrt(pre::sum(pre::norm(*absv))) * absv_max;
        }
        else {
            return pre::sqrt(pre::sum(pre::norm(*absv)));
        }
    }

    /// Normalize with respect to \f$ L^1 \f$ norm.
    Float normalize1(VecView<Field> v) noexcept {
        const Float len = norm1(v);
        if (!robust_div(v, len))
            v = Field(0); // Error
        return len;
    }

    /// Normalize with respect to \f$ L^2 \f$ norm.
    Float normalize2(VecView<Field> v) noexcept {
        const Float len = norm2(v);
        if (!robust_div(v, len))
            v = Field(0); // Error
        return len;
    }

    /** \} */

  public:
    /// \name Factorization: LU
    /** \{ */

    /// Factorize.
    ///
    /// \param[inout] a
    /// Matrix \f$ A \to L, U \f$.
    ///
    /// \param[out] p
    /// _Optional_. Pivot permutation vector. If null, no pivoting is
    /// performed.
    ///
    /// \returns
    /// Returns sign of pivot permutation, either `+1` or `-1`. If no
    /// pivoting, this is always `+1`.
    ///
    static int lu(
            MatView<Field> a, std::optional<VecView<int>> p = std::nullopt) {
        ASSERT(a.is_square());
        ASSERT(!p || a.size() == p->size());
        int n = a.size(); // Dimension.
        int s = 1;        // Permutation sign.
        if (p)
            load_iota(*p);
        for (int j = 0; j < n; j++) {
            if (p) { // Optionally pivot.
                int piv = find_pivot(a(Slice(j, n), j)) + j;
                if (piv != j) {
                    std::swap((*p)[j], (*p)[piv]);
                    a.swap_rows(j, piv);
                    s = -s;
                }
            }
            expect(pre::abs(a(j, j)) > FloatLimits::min_invertible(),
                   "non-singular matrix");
            // Factorize.
            Field fac = Float(1) / a(j, j);
            for (int i = j + 1; i < n; i++) {
                a(i, j) *= fac;
                for (int k = j + 1; k < n; k++)
                    a(i, k) -= a(i, j) * a(j, k);
            }
        }
        return s;
    }

    /// Use factorization to solve linear system \f$ AX = B \f$.
    ///
    /// \param[in] a
    /// Matrix \f$ A \f$ factorized as \f$ L, U \f$ in-place.
    ///
    /// \param[in] b
    /// Matrix \f$ B \f$.
    ///
    /// \param[out] x
    /// Matrix of solutions \f$ X \f$.
    ///
    /// \param[in] p
    /// If `a` was factorized with pivoting, then this _must_ be the
    /// pivot permutation returned by `lu()`. If `a` was not factorized with
    /// pivoting, then this _must_ be null.
    ///
    static void lu_solve(
            MatView<const Field> a,
            MatView<const Field> b,
            MatView<Field> x,
            std::optional<VecView<const int>> p = std::nullopt) {
        ASSERT(a.is_square());
        ASSERT(b.rows() == a.rows() && //
               x.rows() == a.cols() && //
               x.cols() == b.cols());
        ASSERT(!p || a.size() == p->size());
        int m = b.rows();
        int n = b.cols();
        for (int j = 0; j < n; j++) {
            // Solve Ly = b.
            for (int i = 0; i < m; i++) {
                auto k = Slice(0, i);
                x(i, j) = b(p ? (*p)[i] : i, j) - dot(*a(i, k), *x(k, j));
            }
            // Solve Ux = y.
            for (int i = m - 1; i >= 0; i--) {
                auto k = Slice(i + 1, m);
                x(i, j) -= dot(*a(i, k), *x(k, j));
                x(i, j) /= a(i, i);
            }
        }
    }

    /** \} */

  public:
    /// \name Factorization: Cholesky
    /** \{ */

    /// Factorize.
    ///
    /// \param[inout] a
    /// Matrix \f$ A \to R \f$.
    ///
    /// \param[out] p
    /// _Optional_. Pivot permutation vector. If null, no pivoting is
    /// performed.
    ///
    static void chol(
            MatView<Field> a, std::optional<VecView<int>> p = std::nullopt) {
        ASSERT(a.is_square());
        ASSERT(!p || a.size() == p->size());
        int n = a.size();
        if (p)
            load_iota(*p);
        Float eps = FloatLimits::min_invertible();
        for (int k = 0; k < n; k++) {
            if (p) { // Optionally pivot.
                int piv = find_pivot(a.diag()[Slice(k, n)]) + k;
                if (piv != k) {
                    std::swap((*p)[k], (*p)[piv]);
                    a.swap_rows(k, piv);
                    a.swap_cols(k, piv);
                }
                if (k == 0)
                    eps = pre::abs(a(0, 0)) * FloatLimits::epsilon();
                // Positive semi-definite?
                if (!(pre::abs(a(k, k)) > eps)) {
                    for (int i = k; i < n; i++)
                        for (int j = i; j < n; j++)
                            a(i, j) = Field(0);
                    break;
                }
            }
            Field& akk = a(k, k);
            akk = pre::sqrt(akk);
            expect(pre::isfinite(akk) && pre::abs(akk) > eps,
                   "hermitian and positive semi-definite matrix");
            robust_div(a(k, Slice(k + 1, n)), akk);
            for (int j = k + 1; j < n; j++)
                for (int i = k + 1; i < j + 1; i++) {
                    a(i, j) = a(i, j) - a(k, j) * pre::conj(a(k, i));
                    a(j, i) = pre::conj(a(i, j));
                }
        }
        force_upper_triangular(a);
    }

    /// Use factorization to solve linear system \f$ AX = B \f$.
    ///
    /// \param[in] a
    /// Matrix \f$ A \f$ factorized as \f$ R \f$ in-place.
    ///
    /// \param[in] b
    /// Matrix \f$ B \f$.
    ///
    /// \param[out] x
    /// Matrix of solutions \f$ X \f$.
    ///
    /// \param[in] p
    /// If `a` was factorized with pivoting, then this _must_ be the
    /// pivot permutation returned by `chol()`. If `a` was not factorized with
    /// pivoting, then this _must_ be null.
    ///
    void chol_solve(
            MatView<const Field> a,
            MatView<const Field> b,
            MatView<Field> x,
            std::optional<VecView<const int>> p = std::nullopt) {
        ASSERT(a.is_square());
        ASSERT(b.rows() == a.rows() && //
               x.rows() == a.cols() && //
               x.cols() == b.cols());
        ASSERT(!p || a.size() == p->size());
        const int m = b.rows();
        const int n = b.cols();
        auto _ = cache_.scoped_push();
        auto y = cache_.template vector<Field>(m);
        for (int j = 0; j < n; j++) {
            // Solve R*y = b.
            for (int i = 0; i < m; i++) {
                auto k = Slice(0, i);
                y[i] = b(p ? (*p)[i] : i, j) - dot(pre::conj(*a(k, i)), *y[k]);
                if (a(i, i) != Float(0))
                    y[i] /= pre::conj(a(i, i));
            }
            // Solve Rx = y.
            for (int i = m - 1; i >= 0; i--) {
                auto k = Slice(i + 1, m);
                y[i] -= dot(*a(i, k), *y[k]);
                if (a(i, i) != Float(0))
                    y[i] /= a(i, i);
            }
            for (int i = 0; i < m; i++)
                x(p ? (*p)[i] : i, j) = y[i];
        }
    }

    /** \} */

  public:
    /// \name Factorization: QR
    /** \{ */

    /// Factorize.
    ///
    /// \param[inout] a  Matrix \f$ A \to R \f$.
    /// \param[out]   q  Matrix \f$ Q \f$.
    ///
    void qr(MatView<Field> a, MatView<Field> q) {
        ASSERT(q.size() == a.rows());
        ASSERT(q.is_square());
        load_identity(q);
        auto m = SliceInt(a.rows());
        auto n = SliceInt(a.cols());
        for (int k = 0; k < pre::min(m, n); k++) {
            auto _ = cache_.scoped_push();
            auto w = householder00(a(k | m, k | n));
            householder_reflect(w, q(k | m, 0 | m));
        }
        do_adjoint(q);
    }

  private:
    /// Householder reflect all column vectors in matrix.
    static constexpr void householder_reflect(
            VecView<const Field> w, MatView<Field> a) noexcept {
        auto m = SliceInt(a.rows());
        auto n = SliceInt(a.cols());
        for (int j = 0; j < n; j++) {
            auto c = a(0 | m, j);
            c = *c - (Field(2) * dot(pre::conj(*w), *c)) * (*w);
        }
    }

    /// Apply Householder transform to element 0, 0 of given matrix.
    VecView<Field> householder00(MatView<Field> a) {
        auto m = SliceInt(a.rows());
        auto n = SliceInt(a.cols());
        auto w0 = cache_.template vector<Field>(m);
        auto a0 = a(0 | m, 0);
        w0 = *a0;
        a0[0] = -norm2(w0) * sign(w0[0]), a0(1 | m) = Field(0);
        w0[0] -= a0[0];
        normalize2(w0);
        householder_reflect(w0, a(0 | m, 1 | n));
        return w0;
    }

    /** \} */
};

template <concepts::floating_point_or_complex Field>
inline Linalg<Field> linalg;

} // namespace pre
