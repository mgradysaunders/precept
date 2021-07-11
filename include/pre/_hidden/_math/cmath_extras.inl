/*-*- C++ -*-*/
#pragma once

namespace pre {

/// \name Common
/** \{ */

/// Analogous to `std::min()`, except only for numbers.
template <concepts::arithmetic_or_enum T, concepts::arithmetic_or_enum U>
[[gnu::always_inline]] constexpr auto min(T x, U y) noexcept {
    return x < y ? x : y;
}

/// Analogous to `std::max()`, except only for numbers.
template <concepts::arithmetic_or_enum T, concepts::arithmetic_or_enum U>
[[gnu::always_inline]] constexpr auto max(T x, U y) noexcept {
    return x < y ? y : x;
}

/// Clamp in range.
///
/// \param[in] x  Value.
/// \param[in] a  Range minimum.
/// \param[in] b  Range maximum.
///
template <concepts::arithmetic_or_enum T>
[[gnu::always_inline]] constexpr T clamp(
        T x, std::type_identity_t<T> a, std::type_identity_t<T> b) noexcept {
    return pre::min(pre::max(x, a), b);
}

/// Clamp absolute value in range.
///
/// \param[in] x  Value.
/// \param[in] b  Range maximum.
///
template <concepts::arithmetic_or_enum T>
[[gnu::always_inline]] constexpr T clamp_abs(
        T x, std::type_identity_t<T> b) noexcept {
    return clamp(x, -b, b);
}

/// Raise to integer power.
template <concepts::arithmetic T>
constexpr T nthpow(T x, int n) noexcept {
    if (n < 0) {
        return T(1) / nthpow(x, -n);
    }
    else {
        T y = 1;
        while (1) {
            if (n & 1)
                y *= x;
            n >>= 1;
            if (n == 0)
                break;
            x *= x;
        }
        return y;
    }
}

/// Chain-assign variables.
///
/// \note
/// This is meant to reduce chained assignment expressions, such
/// that `chain_assign(a, b, c, d)` is the same as `a = b`, then `b = c`,
/// then `c = d`.
///
template <typename T, typename U, typename... V>
[[gnu::always_inline]] constexpr void chain_assign(
        T& a, U&& b, V&&... c) noexcept {
    a = std::move(b);
    if constexpr (sizeof...(V) > 0) {
        chain_assign(b, std::forward<V>(c)...);
    }
}

/// Cycle-assign variables.
///
/// \note
/// This is meant to reduce cyclical assignment expressions, such
/// that `cycle_assign(a, b, c)` is the same as `tmp = a`, `a = b`,
/// `b = c`, `c = tmp`.
///
template <typename T, typename U, typename... V>
[[gnu::always_inline]] constexpr void cycle_assign(
        T& a, U& b, V&... c) noexcept {
    T t = std::move(a);
    chain_assign(a, b, c..., t);
}

/** \} */

/// \name Integer helpers
/** \{ */

/// Wrap integer in range.
///
/// - If \f$ n > 0 \f$, wrap \f$ k \f$ to \f$ [0, n) \f$.
/// - If \f$ n < 0 \f$, wrap \f$ k \f$ to \f$ (n, 0] \f$.
///
/// \note
/// For \f$ k > 0 \f$, \f$ n > 0 \f$, this operation is
/// equivalent to \f$ k \% n \f$.
///
template <std::integral T>
constexpr T repeat(T k, T n) noexcept {
    if constexpr (std::unsigned_integral<T>) {
        return k % n;
    }
    else {
        if (n < T(0)) {
            return -repeat(-k, -n);
        }
        else {
            if (k >= T(0)) {
                return k % n;
            }
            else {
                k = n + k % n;
                if (k == n) {
                    k = T(0);
                }
                return k;
            }
        }
    }
}

/// Wrap integer in range and mirror with each repeat.
///
/// - If \f$ n > 0 \f$, wrap \f$ k \f$ to \f$ [0, n) \f$.
/// - If \f$ n < 0 \f$, wrap \f$ k \f$ to \f$ (n, 0] \f$.
///
template <std::integral T>
constexpr T mirror(T k, T n) noexcept {
    if (n < T(0)) {
        return -mirror(-k, -n);
    }
    else {
        T r = k % n;
        T q = k / n;
        if (r < T(0)) {
            r += n;
            q++;
        }
        if (q & T(1)) {
            r = n - r - T(1);
        }
        return r;
    }
}

/// Is power of 2?
template <std::integral T>
constexpr bool ispow2(T n) noexcept {
    return n > 0 && (n & (n - 1)) == 0;
}

/// Round up to power of 2.
template <std::integral T>
constexpr T roundpow2(T n) noexcept {
    if (n <= 0) {
        return 1;
    }
    else {
        n--;
        T p = 1;
        while (n) {
            n >>= 1;
            p <<= 1;
        }
        return p;
    }
}

/// Find first bit equal to 1.
///
/// \note
/// This is equal to the binary logarithm if the input integer
/// is a positive power of 2.
///
/// \note
/// Uses [`__builtin_ctz`][1] when compiling with GCC.
/// [1]: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
///
template <std::integral T>
constexpr T first1(T n) noexcept {
    if (n == 0) {
        return 0; // Error?
    }

#if __GNUC__
    if constexpr (sizeof(T) <= sizeof(int)) {
        return __builtin_ctz(n);
    }
    else if constexpr (sizeof(T) == sizeof(long)) {
        return __builtin_ctzl(n);
    }
    else if constexpr (sizeof(T) == sizeof(long long)) {
        return __builtin_ctzll(n);
    }
    else {
        // Error.
    }
#else
    T j = 0;
    while (!(n & 1)) {
        n >>= 1;
        j++;
    }
    return j;
#endif // #if __GNUC__
}

/// Cyclical bit rotate left.
template <std::integral T>
constexpr T rotl(T val, unsigned rot) noexcept {
    return (val << rot) | (val >> ((-rot) & (sizeof(T) * 8 - 1)));
}

/// Cyclical bit rotate right.
template <std::integral T>
constexpr T rotr(T val, unsigned rot) noexcept {
    return (val >> rot) | (val << ((-rot) & (sizeof(T) * 8 - 1)));
}

/// Bit swap.
template <std::integral T>
constexpr T bit_swap(T val, int pos0, int pos1) noexcept {
    T mask = ((val >> pos0) ^ (val >> pos1)) & 1;
    return ((mask << pos0) | (mask << pos1)) ^ val;
}

/// Bit reverse.
template <std::integral T>
constexpr T bit_reverse(T val) noexcept {
    if constexpr (std::signed_integral<T>) {
        return bit_reverse<std::make_unsigned_t<T>>(val);
    }
    else if constexpr (sizeof(T) == sizeof(std::uint8_t)) {
        constexpr std::uint8_t mask[3] = {0xaaU, 0xccU, 0xf0U};
        for (int k = 0; k < 3; k++)
            val = ((val & mask[k]) >> (1 << k)) |
                  ((val & ~mask[k]) << (1 << k));
    }
    else if constexpr (sizeof(T) == sizeof(std::uint16_t)) {
        constexpr std::uint16_t mask[4] = {0xaaaaU, 0xccccU, 0xf0f0U, 0xff00U};
#if __GNUC__
        val = __builtin_bswap16(val);
        for (int k = 0; k < 3; k++)
#else
        for (int k = 0; k < 4; k++)
#endif // #if __GNUC__
            val = ((val & mask[k]) >> (1 << k)) |
                  ((val & ~mask[k]) << (1 << k));
    }
    else if constexpr (sizeof(T) == sizeof(std::uint32_t)) {
        constexpr std::uint32_t mask[5] = {
                0xaaaaaaaaUL, 0xccccccccUL, 0xf0f0f0f0UL, 0xff00ff00UL,
                0xffff0000UL};
#if __GNUC__
        val = __builtin_bswap32(val);
        for (int k = 0; k < 3; k++)
#else
        for (int k = 0; k < 5; k++)
#endif // #if __GNUC__
            val = ((val & mask[k]) >> (1 << k)) |
                  ((val & ~mask[k]) << (1 << k));
    }
    else if constexpr (sizeof(T) == sizeof(std::uint64_t)) {
        constexpr std::uint64_t mask[6] = {
                0xaaaaaaaaaaaaaaaaULL, 0xccccccccccccccccULL,
                0xf0f0f0f0f0f0f0f0ULL, 0xff00ff00ff00ff00ULL,
                0xffff0000ffff0000ULL, 0xffffffff00000000ULL};
#if __GNUC__
        val = __builtin_bswap64(val);
        for (int k = 0; k < 3; k++)
#else
        for (int k = 0; k < 6; k++)
#endif // #if __GNUC__
            val = ((val & mask[k]) >> (1 << k)) |
                  ((val & ~mask[k]) << (1 << k));
    }
    else {
        // Error.
    }
    return val;
}

#if !DOXYGEN

// Interleave integer with zero.
template <std::integral T>
constexpr T bit_interleave_zero_(T val) noexcept {
    if constexpr (std::signed_integral<T>) {
        return bit_interleave_zero_<std::make_unsigned_t<T>>(val);
    }
    else if constexpr (sizeof(T) == sizeof(std::uint8_t)) {
        constexpr std::uint8_t mask[3] = {0xaaU, 0xccU, 0xf0U};
        for (int k = 2; k >= 0; k--)
            val = (val ^ (val << (1 << k))) & ~mask[k];
    }
    else if constexpr (sizeof(T) == sizeof(std::uint16_t)) {
        constexpr std::uint16_t mask[4] = {0xaaaaU, 0xccccU, 0xf0f0U, 0xff00U};
        for (int k = 3; k >= 0; k--)
            val = (val ^ (val << (1 << k))) & ~mask[k];
    }
    else if constexpr (sizeof(T) == sizeof(std::uint32_t)) {
        constexpr std::uint32_t mask[5] = {
                0xaaaaaaaaUL, 0xccccccccUL, 0xf0f0f0f0UL, 0xff00ff00UL,
                0xffff0000UL};
        for (int k = 4; k >= 0; k--)
            val = (val ^ (val << (1 << k))) & ~mask[k];
    }
    else if constexpr (sizeof(T) == sizeof(std::uint64_t)) {
        constexpr std::uint64_t mask[6] = {
                0xaaaaaaaaaaaaaaaaULL, 0xccccccccccccccccULL,
                0xf0f0f0f0f0f0f0f0ULL, 0xff00ff00ff00ff00ULL,
                0xffff0000ffff0000ULL};
        for (int k = 5; k >= 0; k--)
            val = (val ^ (val << (1 << k))) & ~mask[k];
    }
    else {
        // Error.
    }
    return val;
}

#endif // #if !DOXYGEN

/// Bit interleave.
///
/// Form an integer \f$ z \f$ from the
/// bits of \f$ x \f$ and \f$ y \f$ such that
/// - bit \f$ k \f$ of \f$ x \f$ is bit \f$ 2k \f$ of \f$ z \f$,
/// - bit \f$ k \f$ of \f$ y \f$ is bit \f$ 2k + 1 \f$ of \f$ z \f$.
///
/// \note
/// If the input integer type has \f$ 2^n \f$ bits, the top
/// \f$ 2^{n - 1} \f$ bits of the input integers do not appear in
/// the output.
///
template <std::integral T>
constexpr T bit_interleave(T val0, T val1) noexcept {
    val0 = bit_interleave_zero_(val0);
    val1 = bit_interleave_zero_(val1);
    return val0 | (val1 << 1);
}

/// Bayer index for ordered dithering.
///
/// Let \f$ \mathbf{M} \in \mathbb{R}^{n,n} \f$ denote the Bayer index
/// matrix for dimension \f$ n = 2^B \f$, where
/// \f[
///      M_{i,j} =
///         \frac{1}{n^2}
///         \operatorname{reverse}_{2B}(
///         \operatorname{interleave}(\operatorname{xor}(j, i), i)).
/// \f]
///
/// This routine calculates \f$ n^2 M_{i,j} \f$, which is always an
/// integer.
///
template <std::size_t B, std::integral T>
constexpr T bayer_index(T i, T j) noexcept {
    static_assert(B > 0, "B must be greater than 0");
    static_assert(sizeof(T) * 8 > B * 2, "T does not have enough bits");
    i = i & ((1 << B) - 1);
    j = j & ((1 << B) - 1);
    return bit_reverse<T>(bit_interleave<T>(j ^ i, i)) >>
           (sizeof(T) * 8 - B * 2);
}

#if !DOXYGEN

template <std::size_t Nbytes>
constexpr auto sized_int_() {
    if constexpr (sizeof(std::int_least8_t) >= Nbytes) {
        return std::int_least8_t();
    }
    else if constexpr (sizeof(std::int_least16_t) >= Nbytes) {
        return std::int_least16_t();
    }
    else if constexpr (sizeof(std::int_least32_t) >= Nbytes) {
        return std::int_least32_t();
    }
    else if constexpr (sizeof(std::int_least64_t) >= Nbytes) {
        return std::int_least64_t();
    }
    else {
        return; // void
    };
}

template <std::size_t Nbytes>
constexpr auto sized_uint_() {
    if constexpr (sizeof(std::uint_least8_t) >= Nbytes) {
        return std::uint_least8_t();
    }
    else if constexpr (sizeof(std::uint_least16_t) >= Nbytes) {
        return std::uint_least16_t();
    }
    else if constexpr (sizeof(std::uint_least32_t) >= Nbytes) {
        return std::uint_least32_t();
    }
    else if constexpr (sizeof(std::uint_least64_t) >= Nbytes) {
        return std::uint_least64_t();
    }
    else {
        return; // void
    };
}

#endif // #if !DOXYGEN

/// Sized int type selector.
///
/// \tparam Nbytes
/// Least number of bytes.
///
template <std::size_t Nbytes>
struct sized_int {
    /// Type.
    ///
    /// This is the smallest signed integral type
    /// that occupies at least `Nbytes`, or `void` if no such type exists.
    ///
    using type = std::decay_t<decltype(sized_int_<Nbytes>())>;
};

/// Sized unsigned int type selector.
///
/// \tparam Nbytes
/// Least number of bytes.
///
template <std::size_t Nbytes>
struct sized_uint {
    /// Type.
    ///
    /// This is the smallest unsigned integral type
    /// that occupies at least `Nbytes`, or `void` if no such type exists.
    ///
    using type = std::decay_t<decltype(sized_uint_<Nbytes>())>;
};

/// Greatest common divisor.
template <std::integral T>
constexpr T gcd(T a, T b) noexcept {
    T r = 0;
    while (a) {
        r = b % a;
        b = a;
        a = r;
    }
    return b;
}

/// Greatest common divisor with Bezout coefficients.
template <std::integral T>
constexpr T gcd_bezout(T a, T b, T* x, T* y) noexcept {
    T s1 = 0, s0 = 1;
    T t1 = 1, t0 = 0;
    T r1 = b, r0 = a;
    while (r1 != 0) {
        T q = r0 / r1;
        T k;
        k = r1, r1 = r0 - q * r1, r0 = k;
        k = s1, s1 = s0 - q * s1, s0 = k;
        k = t1, t1 = t0 - q * t1, t0 = k;
    }
    if (x)
        *x = s0;
    if (y)
        *y = t0;
    return r0;
}

/// Least common multiple.
template <std::integral T>
constexpr T lcm(T a, T b) noexcept {
    if (!a || !b) {
        return 0;
    }
    else {
        return a * b / pre::gcd(a, b);
    }
}

/// Cantor pairing function.
///
/// \f[
///     \pi(x, y) =
///     \frac{1}{2}(x + y)(x + y + 1) + y
/// \f]
///
template <std::integral T>
constexpr T cantor(T x, T y) noexcept {
    if constexpr (!std::unsigned_integral<T>) {
        // Natural numbers.
        std::make_unsigned_t<T> xn = std::abs(x);
        std::make_unsigned_t<T> yn = std::abs(y);
        xn = 2 * xn - (x > 0);
        yn = 2 * yn - (y > 0);
        return T(cantor(xn, yn));
    }
    else {
        return ((x + y) * (x + y + 1)) / 2 + y;
    }
}

/// Cantor tuple pairing function.
///
/// \f[
///      \pi(x, y, z, \ldots) = \pi(\pi(x, y), z, \ldots)
/// \f]
///
template <std::integral T, std::integral... Ts>
[[gnu::always_inline]] constexpr T cantor(T x, T y, T z, Ts... ws) noexcept {
    return cantor(cantor(x, y), z, ws...);
}

/** \} */

/// \name Floating point helpers
/** \{ */

template <std::floating_point T>
inline T finite_or(T x, T x0) noexcept {
    return std::isfinite(x) ? x : x0;
}

template <std::floating_point T>
inline T finite_or_zero(T x) noexcept {
    return finite_or(x, T(0));
}

template <std::floating_point T>
inline T frepeat(T x, T a, T b) noexcept {
    x -= a;
    b -= a;
    T r = std::remainder(x, b);
    if (r < 0)
        r += b;
    return r + a;
}

template <std::floating_point T>
inline T fmirror(T x, T a, T b) noexcept {
    x -= a;
    b -= a;
    int q = 0;
    T r = std::remquo(x, b, &q);
    if (r < 0) {
        r += b;
        q++;
    }
    if (q & 1)
        r = b - r;
    return r + a;
}

/// Increment float to next representable value.
template <std::floating_point T>
inline T float_incr(T x) noexcept {
    if constexpr (
            std::same_as<T, float> && std::numeric_limits<float>::is_iec559) {

        // To bits.
        std::uint32_t u;
        std::memcpy(&u, &x, 4);
        // Is not positive infinity?
        if (u != (0x7f8UL << 20)) {
            // Skip negative zero.
            if (u == (1UL << 31))
                u = 0;
            // Bump.
            if (u & (1UL << 31))
                u -= 1;
            else
                u += 1;
        }
        // To float.
        std::memcpy(&x, &u, 4);
        return x;
    }
    else if constexpr (
            std::same_as<T, double> &&
            std::numeric_limits<double>::is_iec559) {

        // To bits.
        std::uint64_t u;
        std::memcpy(&u, &x, 8);
        // Is not positive infinity?
        if (u != (0x7ffULL << 52)) {
            // Skip negative zero.
            if (u == (1ULL << 63))
                u = 0;
            // Bump.
            if (u & (1ULL << 63))
                u -= 1;
            else
                u += 1;
        }
        // To float.
        std::memcpy(&x, &u, 8);
        return x;
    }
    else {
        return std::nextafter(x, +std::numeric_limits<T>::infinity());
    }
}

/// Decrement float to next representable value.
template <std::floating_point T>
inline T float_decr(T x) noexcept {
    if constexpr (
            std::same_as<T, float> && std::numeric_limits<float>::is_iec559) {

        // To bits.
        std::uint32_t u;
        std::memcpy(&u, &x, 4);
        // Is not negative infinity?
        if (u != (0xff8UL << 20)) {
            // Skip positive zero.
            if (u == 0)
                u = (1UL << 31);
            // Bump.
            if (u & (1UL << 31))
                u += 1;
            else
                u -= 1;
        }
        // To float.
        std::memcpy(&x, &u, 4);
        return x;
    }
    else if constexpr (
            std::same_as<T, double> &&
            std::numeric_limits<double>::is_iec559) {

        // To bits.
        std::uint64_t u;
        std::memcpy(&u, &x, 8);
        // Is not negative infinity?
        if (u != (0xfffULL << 52)) {
            // Skip positive zero.
            if (u == 0)
                u = (1ULL << 63);
            // Bump.
            if (u & (1ULL << 63))
                u += 1;
            else
                u -= 1;
        }
        // To float.
        std::memcpy(&x, &u, 8);
        return x;
    }
    else {
        return std::nextafter(x, -std::numeric_limits<T>::infinity());
    }
}

template <std::floating_point T>
inline bool relatively_tiny(T a, T b) noexcept {
    a = std::fabs(a);
    b = std::fabs(b);
    return (a + b) == b;
}

template <std::floating_point T>
inline bool relatively_huge(T a, T b) noexcept {
    return relatively_tiny(b, a);
}

/// Fast inverse square root.
template <std::floating_point T>
[[gnu::always_inline]] inline T fast_inv_sqrt(T x) noexcept {
    if constexpr (pre::numeric_limits<float>::is_iec559) {
        float y = x;
        float h = y * 0.5f;
        std::uint32_t u = 0;
        std::memcpy(&u, &y, 4);
        u = 0x5f375a86 - (u >> 1);
        std::memcpy(&y, &u, 4);
        y = y * (1.5f - h * y * y);
        y = y * (1.5f - h * y * y);
        return y;
    }
    else {
        return 1 / pre::sqrt(x);
    }
}

/// Fast floor by int casting.
template <std::floating_point T, std::integral U = int>
[[gnu::always_inline]] inline U fast_floor(T x) noexcept {
    U i(x);
    i = i - (T(i) > x);
    return i;
}

/// Fast ceil by int casting.
template <std::floating_point T, std::integral U = int>
[[gnu::always_inline]] inline U fast_ceil(T x) noexcept {
    U i(x);
    i = i + (T(i) < x);
    return i;
}

/// Fast round by int casting.
template <std::floating_point T, std::integral U = int>
[[gnu::always_inline]] inline U fast_round(T x) noexcept {
    return fast_floor<T, U>(x + T(0.5));
}

/// Fast trunc by int casting.
template <std::floating_point T, std::integral U = int>
[[gnu::always_inline]] inline U fast_trunc(T x) noexcept {
    return U(x);
}

/// Packed cast.
///
/// This function converts between floating point values and (un)signed
/// normalized "packed" integer values.
///
template <concepts::arithmetic U, concepts::arithmetic T>
[[gnu::always_inline]] inline U packed_cast(T x) noexcept {
    // Is input floating point?
    if constexpr (std::floating_point<T>) {

        // Is output floating point?
        if constexpr (std::floating_point<U>) {
            return x;
        }
        else {
            // Is output unsigned?
            if constexpr (std::unsigned_integral<U>) {
                // Clamp in [0, 1].
                x = pre::max(x, T(0));
                x = pre::min(x, T(1));
            }
            else {
                // Clamp in [-1, +1].
                x = pre::max(x, T(-1));
                x = pre::min(x, T(+1));
            }

            return pre::fast_round<T, U>(pre::numeric_limits<U>::max() * x);
        }
    }
    else {
        // Is output floating point?
        if constexpr (std::floating_point<U>) {
            return static_cast<U>(x) /
                   static_cast<U>(pre::numeric_limits<T>::max());
        }
        else {
            // Is output same as input? (Trivial case)
            if constexpr (std::same_as<T, U>) {
                return x;
            }
            else {
                // Use double to convert between integral types.
                return packed_cast<U>(packed_cast<double>(x));
            }
        }
    }
}

/// Calculate `sin` and `cos`, return as pair.
template <concepts::arithmetic T>
inline auto sincos(T x) noexcept {
    // On GCC, could manually call __builtin_sincos, but this should
    // be optimizable as is.
    return std::make_pair(std::sin(x), std::cos(x));
}

/// Multiply by \f$ \pi \f$, then take sine.
///
/// \note
/// This is equal to \f$ \sin(\pi x) \f$. However, this
/// exploits periodicity, using `remquo()` before multiplying by
/// \f$ \pi \f$ to be more accurate for large arguments.
///
template <std::floating_point T>
inline T sinpi(T x) noexcept {
    int quo;
    T rem = pre::remquo(x, T(1), &quo);
    T res = pre::sin(pre::numeric_constants<T>::M_pi() * rem);
    if (unsigned(quo) & 1) {
        res = -res;
    }
    return res;
}

/// Multiply by \f$ \pi \f$, then take cosine.
///
/// \note
/// This is equal to \f$ \cos(\pi x) \f$. However, this
/// exploits periodicity using `remquo()` before multiplying by
/// \f$ \pi \f$ to be more accurate for large arguments.
///
template <std::floating_point T>
inline T cospi(T x) noexcept {
    int quo;
    T rem = pre::remquo(x, T(1), &quo);
    T res = pre::cos(pre::numeric_constants<T>::M_pi() * rem);
    if (unsigned(quo) & 1) {
        res = -res;
    }
    return res;
}

/// Find minimum value of `sinpi()` on interval.
template <std::floating_point T>
inline T sinpi_min(T x1, T x2) noexcept {
    if (!(x1 < x2)) {
        std::swap(x1, x2);
    }
    int n1 = pre::fast_floor(2 * x1);
    int n2 = pre::fast_floor(2 * x2);
    if (n2 - n1 > 3) {
        return -1;
    }
    else {
        // 0 = min is -1
        // 1 = min is sinpi(x1)
        // 2 = min is sinpi(x2)
        // 3 = min is either sinpi(x1) or sinpi(x2)
        constexpr int lookup[4][4] = {
                {1, 3, 2, 0}, {0, 2, 2, 0}, {0, 0, 2, 0}, {1, 1, 3, 1}};
        n1 &= 3;
        n2 &= 3;
        switch (lookup[n1][n2]) {
        case 0: return -1;
        case 1: return pre::sinpi(x1);
        case 2: return pre::sinpi(x2);
        case 3: return pre::min(pre::sinpi(x1), pre::sinpi(x2));
        default: break;
        }
        // Unreachable.
        return 0;
    }
}

/// Find maximum value of `sinpi()` on interval.
template <std::floating_point T>
inline T sinpi_max(T x1, T x2) noexcept {
    if (!(x1 < x2)) {
        std::swap(x1, x2);
    }
    int n1 = pre::fast_floor(2 * x1);
    int n2 = pre::fast_floor(2 * x2);
    if (n2 - n1 > 3) {
        return +1;
    }
    else {
        // 0 = max is +1
        // 1 = max is sinpi(x1)
        // 2 = max is sinpi(x2)
        // 3 = max is either sinpi(x1) or sinpi(x2)
        constexpr int lookup[4][4] = {
                {2, 0, 0, 0}, {3, 1, 1, 1}, {2, 0, 1, 3}, {2, 0, 0, 2}};
        n1 &= 3;
        n2 &= 3;
        switch (lookup[n1][n2]) {
        case 0: return +1;
        case 1: return pre::sinpi(x1);
        case 2: return pre::sinpi(x2);
        case 3: return pre::max(pre::sinpi(x1), pre::sinpi(x2));
        default: break;
        }
        // Unreachable.
        return 0;
    }
}

/// Calculate `sinpi` and `cospi`, return as pair.
template <std::floating_point T>
inline auto sincospi(T x) noexcept {
    int quo;
    T rem = pre::remquo(x, T(1), &quo);
    auto [sinx, cosx] = pre::sincos(pre::numeric_constants<T>::M_pi() * rem);
    if (unsigned(quo) & 1) {
        sinx = -sinx;
        cosx = -cosx;
    }
    return std::make_pair(sinx, cosx);
}

/// Error function inverse.
template <std::floating_point T>
inline T erfinv(T y) noexcept {
    T w = -pre::log((1 - y) * (1 + y));
    T p;
    if (w < T(5)) {
        w = w - T(2.5);
        p = pre::fma(w, T(+2.81022636e-08), T(+3.43273939e-7));
        p = pre::fma(w, p, T(-3.52338770e-6));
        p = pre::fma(w, p, T(-4.39150654e-6));
        p = pre::fma(w, p, T(+2.18580870e-4));
        p = pre::fma(w, p, T(-1.25372503e-3));
        p = pre::fma(w, p, T(-4.17768164e-3));
        p = pre::fma(w, p, T(+2.46640727e-1));
        p = pre::fma(w, p, T(+1.50140941));
    }
    else {
        w = pre::sqrt(w) - 3;
        p = pre::fma(w, T(-2.00214257e-4), T(+1.00950558e-4));
        p = pre::fma(w, p, T(+1.34934322e-3));
        p = pre::fma(w, p, T(-3.67342844e-3));
        p = pre::fma(w, p, T(+5.73950773e-3));
        p = pre::fma(w, p, T(-7.62246130e-3));
        p = pre::fma(w, p, T(+9.43887047e-3));
        p = pre::fma(w, p, T(+1.00167406));
        p = pre::fma(w, p, T(+2.83297682));
    }
    return p * y;
}

/// Quadratic roots.
template <std::floating_point T>
inline bool quadratic(T a, T b, T c, T& x0, T& x1) noexcept {
    if (relatively_tiny(a, b)) {
        x0 = x1 = -c / b;
        return std::isfinite(x0);
    }
    else {
        b /= a;
        c /= a;
        if (not std::isfinite(b) or //
            not std::isfinite(c))
            return false;
        T d = b * b - 4 * c;
        if (not std::isfinite(d))
            d = b * (b - 4 * (c / b)); // Try again
        if (not std::isfinite(d) or d < 0)
            return false;
        x0 = -T(0.5) * (b + std::copysign(std::sqrt(d), b));
        x1 = c / x0;
        if (x0 > x1)
            std::swap(x0, x1);
        return true;
    }
}

/** \} */

} // namespace pre
