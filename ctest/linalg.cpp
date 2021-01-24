#include "doctest.h"
#include <pre/random>
#include <pre/array>

template <typename Field, size_t M, size_t N>
static auto random_matrix(pre::Pcg32& gen) {
    pre::Array<Field, M, N> a;
    for (size_t i = 0; i < M; i++)
        for (size_t j = 0; j < N; j++)
            if constexpr (pre::concepts::matches<Field, std::complex>)
                a(i, j) = Field(gen(-1.0, 1.0), gen(-1.0, 1.0));
            else
                a(i, j) = gen(-1.0, 1.0);
    return a;
}

template <typename Field>
static bool is_approx(const Field& lhs, const Field& rhs, double eps) {
    if constexpr (pre::concepts::matches<Field, std::complex>)
        return lhs.real() == Approx(rhs.real()).epsilon(eps) &&
               lhs.imag() == Approx(rhs.imag()).epsilon(eps);
    else
        return lhs == Approx(rhs).epsilon(eps);
}

template <typename Field, size_t M, size_t... N>
static bool is_approx(
        const pre::Array<Field, M, N...>& lhs,
        const pre::Array<Field, M, N...>& rhs,
        double eps) {
    for (size_t i = 0; i < M; i++)
        if (!is_approx(lhs[i], rhs[i], eps))
            return false;
    return true;
}

template <typename Field, size_t M, size_t N>
static void test_qr(pre::Pcg32& gen) {
    pre::Linalg<Field> linalg;
    pre::Array<Field, M, N> a = random_matrix<Field, M, N>(gen);
    pre::Array<Field, M, N> r = a;
    pre::Array<Field, M, M> q;
    CHECK_NOTHROW(linalg.qr(r, q));
    CHECK(is_approx(pre::dot(q, r), a, 0.01));
    CHECK(is_approx(
            pre::dot(q, pre::adjoint(q)), pre::Array<Field, M, M>::identity(),
            0.01));
}

template <typename Field, size_t N>
static void test_lu(pre::Pcg32& gen) {
    pre::Linalg<Field> linalg;
    // Do QR factorization to obtain orthogonal basis.
    pre::Array<Field, N, N> a = random_matrix<Field, N, N>(gen);
    pre::Array<Field, N, N> q;
    CHECK_NOTHROW(linalg.qr(a, q));

    // Reset matrix with random non-zero diagonal entries, expand with
    // randomized Q to obtain random invertible matrix.
    a = {};
    for (size_t k = 0; k < N; k++)
        if constexpr (pre::concepts::matches<Field, std::complex>)
            a(k, k) = Field(gen(0.1, 2.0), gen(0.1, 2.0));
        else
            a(k, k) = gen(0.1, 2.0);
    a = pre::dot(q, pre::dot(a, pre::adjoint(q)));

    // Do factorization with and without pivoting.
    pre::Array<Field, N, N> a0 = a;
    pre::Array<Field, N, N> a1 = a;
    pre::Array<int, N> p = {};
    CHECK_NOTHROW(linalg.lu(a0));
    CHECK_NOTHROW(linalg.lu(a1, p));

    // Solve linear system.
    pre::Array<Field, N, 2> b0 = random_matrix<Field, N, 2>(gen), x0;
    pre::Array<Field, N, 2> b1 = random_matrix<Field, N, 2>(gen), x1;
    CHECK_NOTHROW(linalg.lu_solve(a0, b0, x0));
    CHECK_NOTHROW(linalg.lu_solve(a1, b1, x1, p));
    CHECK(is_approx(pre::dot(a, x0), b0, 0.01));
    CHECK(is_approx(pre::dot(a, x1), b1, 0.01));
}

template <typename Field, size_t N>
static void test_chol(pre::Pcg32& gen) {
    pre::Linalg<Field> linalg;
    // Do QR factorization to obtain orthogonal basis.
    pre::Array<Field, N, N> a = random_matrix<Field, N, N>(gen);
    pre::Array<Field, N, N> q;
    CHECK_NOTHROW(linalg.qr(a, q));

    // Reset matrix with random positive, real diagonal entries, expand with
    // randomized Q to obtain random hermitian positive definite matrix.
    a = {};
    for (size_t k = 0; k < N; k++)
        a(k, k) = Field(gen(0.1, 2.0));
    a = pre::dot(q, pre::dot(a, pre::adjoint(q)));
    for (size_t i = 0; i < N; i++) {
        a(i, i) = pre::real(a(i, i));
        for (size_t j = i + 1; j < N; j++)
            a(j, i) = pre::conj(a(i, j));
    }

    // Do factorization with and without pivoting.
    pre::Array<Field, N, N> a0 = a;
    pre::Array<Field, N, N> a1 = a;
    pre::Array<int, N> p = {};
    CHECK_NOTHROW(linalg.chol(a0));
    CHECK_NOTHROW(linalg.chol(a1, p));
    auto p1 = pre::Array<Field, N, N>(pre::permutation_matrix(p));
    CHECK(is_approx(pre::dot(pre::adjoint(a0), a0), a, 0.01));
    CHECK(is_approx(
            pre::dot(pre::adjoint(a1), a1),
            pre::dot(p1, pre::dot(a, pre::transpose(p1))), 0.01));

    // Solve linear system.
    pre::Array<Field, N, 2> b0 = random_matrix<Field, N, 2>(gen), x0;
    pre::Array<Field, N, 2> b1 = random_matrix<Field, N, 2>(gen), x1;
    CHECK_NOTHROW(linalg.chol_solve(a0, b0, x0));
    CHECK_NOTHROW(linalg.chol_solve(a1, b1, x1, p));
    CHECK(is_approx(pre::dot(a, x0), b0, 0.01));
    CHECK(is_approx(pre::dot(a, x1), b1, 0.01));
}

TEST_CASE_TEMPLATE("Linalg", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    SUBCASE("QR") {
        SUBCASE("With real numbers") {
            test_qr<Float, 8, 3>(gen);
            test_qr<Float, 7, 8>(gen);
        }
        SUBCASE("With complex numbers") {
            test_qr<std::complex<Float>, 8, 3>(gen);
            test_qr<std::complex<Float>, 7, 8>(gen);
        }
    }
    SUBCASE("LU") {
        SUBCASE("With real numbers") {
            test_lu<Float, 2>(gen);
            test_lu<Float, 9>(gen);
        }
        SUBCASE("With complex numbers") {
            test_lu<std::complex<Float>, 2>(gen);
            test_lu<std::complex<Float>, 9>(gen);
        }
    }
    SUBCASE("Cholesky") {
        SUBCASE("With real numbers") {
            test_chol<Float, 4>(gen);
            test_chol<Float, 7>(gen);
        }
        SUBCASE("With complex numbers") {
            test_chol<std::complex<Float>, 4>(gen);
            test_chol<std::complex<Float>, 7>(gen);
        }
    }
}
