/*-*- C++ -*-*/
#pragma once

namespace pre {

/// Wrap `pre::min()`.
template <typename T, size_t... N>
inline auto min(const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::min(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::min(*itrarrx, *itrarry);
    return res;
}

/// Wrap `pre::max()`.
template <typename T, size_t... N>
inline auto max(const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::max(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::max(*itrarrx, *itrarry);
    return res;
}

/// Wrap `pre::clamp()`.
template <typename T, size_t... N>
inline auto clamp(
        const Array<T, N...>& arrx,
        const Array<T, N...>& arra,
        const Array<T, N...>& arrb) {
    using U = decltype(pre::clamp(
            std::declval<const T&>(), std::declval<const T&>(),
            std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarra = arra.begin();
    auto itrarrb = arrb.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarra, ++itrarrb, ++itrres)
        *itrres = pre::clamp(*itrarrx, *itrarra, *itrarrb);
    return res;
}

/// Wrap `pre::nthpow()`.
template <typename T, size_t... N>
inline auto nthpow(const Array<T, N...>& arrx, const Array<int, N...>& arrn) {
    using U = decltype(
            pre::nthpow(std::declval<const T&>(), std::declval<const int&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrn = arrn.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrn, ++itrres)
        *itrres = pre::nthpow(*itrarrx, *itrarrn);
    return res;
}

/// Wrap `pre::rotl()`.
template <typename T, size_t... N>
inline auto rotl(
        const Array<T, N...>& arrval, const Array<unsigned, N...>& arrrot) {
    using U = decltype(pre::rotl(
            std::declval<const T&>(), std::declval<const unsigned&>()));
    Array<U, N...> res;
    auto itrarrval = arrval.begin();
    auto itrarrrot = arrrot.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrval, ++itrarrrot, ++itrres)
        *itrres = pre::rotl(*itrarrval, *itrarrrot);
    return res;
}

/// Wrap `pre::rotr()`.
template <typename T, size_t... N>
inline auto rotr(
        const Array<T, N...>& arrval, const Array<unsigned, N...>& arrrot) {
    using U = decltype(pre::rotr(
            std::declval<const T&>(), std::declval<const unsigned&>()));
    Array<U, N...> res;
    auto itrarrval = arrval.begin();
    auto itrarrrot = arrrot.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrval, ++itrarrrot, ++itrres)
        *itrres = pre::rotr(*itrarrval, *itrarrrot);
    return res;
}

/// Wrap `pre::float_incr()`.
template <typename T, size_t... N>
inline auto float_incr(const Array<T, N...>& arrx) {
    using U = decltype(pre::float_incr(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::float_incr(*itrarrx);
    return res;
}

/// Wrap `pre::float_decr()`.
template <typename T, size_t... N>
inline auto float_decr(const Array<T, N...>& arrx) {
    using U = decltype(pre::float_decr(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::float_decr(*itrarrx);
    return res;
}

/// Wrap `pre::fast_floor()`.
template <typename T, size_t... N>
inline auto fast_floor(const Array<T, N...>& arrx) {
    using U = decltype(pre::fast_floor(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::fast_floor(*itrarrx);
    return res;
}

/// Wrap `pre::fast_ceil()`.
template <typename T, size_t... N>
inline auto fast_ceil(const Array<T, N...>& arrx) {
    using U = decltype(pre::fast_ceil(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::fast_ceil(*itrarrx);
    return res;
}

/// Wrap `pre::fast_trunc()`.
template <typename T, size_t... N>
inline auto fast_trunc(const Array<T, N...>& arrx) {
    using U = decltype(pre::fast_trunc(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::fast_trunc(*itrarrx);
    return res;
}

/// Wrap `pre::fast_round()`.
template <typename T, size_t... N>
inline auto fast_round(const Array<T, N...>& arrx) {
    using U = decltype(pre::fast_round(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::fast_round(*itrarrx);
    return res;
}

/// Wrap `pre::sinpi()`.
template <typename T, size_t... N>
inline auto sinpi(const Array<T, N...>& arrx) {
    using U = decltype(pre::sinpi(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sinpi(*itrarrx);
    return res;
}

/// Wrap `pre::cospi()`.
template <typename T, size_t... N>
inline auto cospi(const Array<T, N...>& arrx) {
    using U = decltype(pre::cospi(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::cospi(*itrarrx);
    return res;
}

/// Wrap `pre::erfinv()`.
template <typename T, size_t... N>
inline auto erfinv(const Array<T, N...>& arrx) {
    using U = decltype(pre::erfinv(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::erfinv(*itrarrx);
    return res;
}

} // namespace pre
