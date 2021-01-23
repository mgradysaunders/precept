/*-*- C++ -*-*/
#pragma once

namespace pre {

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto abs(const Array<T, N...>& arrx) {
    using U = decltype(pre::abs(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::abs(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto arg(const Array<T, N...>& arrx) {
    using U = decltype(pre::arg(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::arg(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto real(const Array<T, N...>& arrx) {
    using U = decltype(pre::real(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::real(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto imag(const Array<T, N...>& arrx) {
    using U = decltype(pre::imag(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::imag(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto conj(const Array<T, N...>& arrx) {
    using U = decltype(pre::conj(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::conj(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto norm(const Array<T, N...>& arrx) {
    using U = decltype(pre::norm(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::norm(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fabs(const Array<T, N...>& arrx) {
    using U = decltype(pre::fabs(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::fabs(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fma(
        const Array<T, N...>& arrx,
        const Array<T, N...>& arry,
        const Array<T, N...>& arrz) {
    using U = decltype(pre::fma(
            std::declval<const T&>(), std::declval<const T&>(),
            std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrarrz = arrz.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrarrz, ++itrres)
        *itrres = pre::fma(*itrarrx, *itrarry, *itrarrz);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fmin(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::fmin(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::fmin(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fmax(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::fmax(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::fmax(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fdim(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::fdim(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::fdim(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto fmod(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::fmod(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::fmod(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto remquo(
        const Array<T, N...>& arrx,
        const Array<T, N...>& arry,
        Array<int, N...>* arrq) {
    using U = decltype(pre::remquo(
            std::declval<const T&>(), std::declval<const T&>(),
            std::declval<int*>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrarrq = arrq->begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrarrq, ++itrres)
        *itrres = pre::remquo(*itrarrx, *itrarry, &(*itrarrq));
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto remainder(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(pre::remainder(
            std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::remainder(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto nearbyint(const Array<T, N...>& arrx) {
    using U = decltype(pre::nearbyint(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::nearbyint(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto floor(const Array<T, N...>& arrx) {
    using U = decltype(pre::floor(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::floor(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto ceil(const Array<T, N...>& arrx) {
    using U = decltype(pre::ceil(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::ceil(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto trunc(const Array<T, N...>& arrx) {
    using U = decltype(pre::trunc(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::trunc(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto round(const Array<T, N...>& arrx) {
    using U = decltype(pre::round(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::round(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto rint(const Array<T, N...>& arrx) {
    using U = decltype(pre::rint(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::rint(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto lrint(const Array<T, N...>& arrx) {
    using U = decltype(pre::lrint(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::lrint(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto llrint(const Array<T, N...>& arrx) {
    using U = decltype(pre::llrint(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::llrint(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto lround(const Array<T, N...>& arrx) {
    using U = decltype(pre::lround(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::lround(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto llround(const Array<T, N...>& arrx) {
    using U = decltype(pre::llround(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::llround(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto frexp(
        const Array<T, N...>& arrx, Array<int, N...>* arrp) {
    using U = decltype(
            pre::frexp(std::declval<const T&>(), std::declval<int*>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrp = arrp->begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrp, ++itrres)
        *itrres = pre::frexp(*itrarrx, &(*itrarrp));
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto ldexp(
        const Array<T, N...>& arrx, const Array<int, N...>& arrp) {
    using U = decltype(
            pre::ldexp(std::declval<const T&>(), std::declval<const int&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrp = arrp.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrp, ++itrres)
        *itrres = pre::ldexp(*itrarrx, *itrarrp);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto logb(const Array<T, N...>& arrx) {
    using U = decltype(pre::logb(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::logb(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto ilogb(const Array<T, N...>& arrx) {
    using U = decltype(pre::ilogb(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::ilogb(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto scalbn(
        const Array<T, N...>& arrx, const Array<int, N...>& arrp) {
    using U = decltype(
            pre::scalbn(std::declval<const T&>(), std::declval<const int&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrp = arrp.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrp, ++itrres)
        *itrres = pre::scalbn(*itrarrx, *itrarrp);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto scalbln(
        const Array<T, N...>& arrx, const Array<long, N...>& arrp) {
    using U = decltype(pre::scalbln(
            std::declval<const T&>(), std::declval<const long&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrp = arrp.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrp, ++itrres)
        *itrres = pre::scalbln(*itrarrx, *itrarrp);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto modf(
        const Array<T, N...>& arrx, Array<T, N...>* arrp) {
    using U =
            decltype(pre::modf(std::declval<const T&>(), std::declval<T*>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarrp = arrp->begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarrp, ++itrres)
        *itrres = pre::modf(*itrarrx, &(*itrarrp));
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto nextafter(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(pre::nextafter(
            std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::nextafter(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto nexttoward(
        const Array<T, N...>& arrx, const Array<long double, N...>& arry) {
    using U = decltype(pre::nexttoward(
            std::declval<const T&>(), std::declval<const long double&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::nexttoward(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto copysign(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::copysign(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::copysign(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto signbit(const Array<T, N...>& arrx) {
    using U = decltype(pre::signbit(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::signbit(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto isnan(const Array<T, N...>& arrx) {
    using U = decltype(pre::isnan(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::isnan(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto isinf(const Array<T, N...>& arrx) {
    using U = decltype(pre::isinf(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::isinf(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto isfinite(const Array<T, N...>& arrx) {
    using U = decltype(pre::isfinite(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::isfinite(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto isnormal(const Array<T, N...>& arrx) {
    using U = decltype(pre::isnormal(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::isnormal(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto exp(const Array<T, N...>& arrx) {
    using U = decltype(pre::exp(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::exp(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto log(const Array<T, N...>& arrx) {
    using U = decltype(pre::log(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::log(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto exp2(const Array<T, N...>& arrx) {
    using U = decltype(pre::exp2(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::exp2(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto log2(const Array<T, N...>& arrx) {
    using U = decltype(pre::log2(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::log2(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto log10(const Array<T, N...>& arrx) {
    using U = decltype(pre::log10(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::log10(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto expm1(const Array<T, N...>& arrx) {
    using U = decltype(pre::expm1(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::expm1(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto log1p(const Array<T, N...>& arrx) {
    using U = decltype(pre::log1p(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::log1p(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto pow(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::pow(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::pow(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto sqrt(const Array<T, N...>& arrx) {
    using U = decltype(pre::sqrt(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sqrt(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto cbrt(const Array<T, N...>& arrx) {
    using U = decltype(pre::cbrt(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::cbrt(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto hypot(
        const Array<T, N...>& arrx, const Array<T, N...>& arry) {
    using U = decltype(
            pre::hypot(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrarry = arry.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrarry, ++itrres)
        *itrres = pre::hypot(*itrarrx, *itrarry);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto erf(const Array<T, N...>& arrx) {
    using U = decltype(pre::erf(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::erf(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto erfc(const Array<T, N...>& arrx) {
    using U = decltype(pre::erfc(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::erfc(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto lgamma(const Array<T, N...>& arrx) {
    using U = decltype(pre::lgamma(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::lgamma(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto tgamma(const Array<T, N...>& arrx) {
    using U = decltype(pre::tgamma(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::tgamma(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto sin(const Array<T, N...>& arrx) {
    using U = decltype(pre::sin(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sin(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto cos(const Array<T, N...>& arrx) {
    using U = decltype(pre::cos(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::cos(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto tan(const Array<T, N...>& arrx) {
    using U = decltype(pre::tan(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::tan(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto asin(const Array<T, N...>& arrx) {
    using U = decltype(pre::asin(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::asin(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acos(const Array<T, N...>& arrx) {
    using U = decltype(pre::acos(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acos(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto atan(const Array<T, N...>& arrx) {
    using U = decltype(pre::atan(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::atan(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto atan2(
        const Array<T, N...>& arry, const Array<T, N...>& arrx) {
    using U = decltype(
            pre::atan2(std::declval<const T&>(), std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarry = arry.begin();
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarry, ++itrarrx, ++itrres)
        *itrres = pre::atan2(*itrarry, *itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto sinh(const Array<T, N...>& arrx) {
    using U = decltype(pre::sinh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sinh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto cosh(const Array<T, N...>& arrx) {
    using U = decltype(pre::cosh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::cosh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto tanh(const Array<T, N...>& arrx) {
    using U = decltype(pre::tanh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::tanh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto asinh(const Array<T, N...>& arrx) {
    using U = decltype(pre::asinh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::asinh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acosh(const Array<T, N...>& arrx) {
    using U = decltype(pre::acosh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acosh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto atanh(const Array<T, N...>& arrx) {
    using U = decltype(pre::atanh(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::atanh(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto csc(const Array<T, N...>& arrx) {
    using U = decltype(pre::csc(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::csc(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto sec(const Array<T, N...>& arrx) {
    using U = decltype(pre::sec(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sec(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto cot(const Array<T, N...>& arrx) {
    using U = decltype(pre::cot(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::cot(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto csch(const Array<T, N...>& arrx) {
    using U = decltype(pre::csch(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::csch(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto sech(const Array<T, N...>& arrx) {
    using U = decltype(pre::sech(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::sech(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto coth(const Array<T, N...>& arrx) {
    using U = decltype(pre::coth(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::coth(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acsc(const Array<T, N...>& arrx) {
    using U = decltype(pre::acsc(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acsc(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto asec(const Array<T, N...>& arrx) {
    using U = decltype(pre::asec(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::asec(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acot(const Array<T, N...>& arrx) {
    using U = decltype(pre::acot(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acot(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acsch(const Array<T, N...>& arrx) {
    using U = decltype(pre::acsch(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acsch(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto asech(const Array<T, N...>& arrx) {
    using U = decltype(pre::asech(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::asech(*itrarrx);
    return res;
}

template <typename T, size_t... N>
[[gnu::always_inline]] inline auto acoth(const Array<T, N...>& arrx) {
    using U = decltype(pre::acoth(std::declval<const T&>()));
    Array<U, N...> res;
    auto itrarrx = arrx.begin();
    auto itrres = res.begin();
    for (; itrres < res.end(); ++itrarrx, ++itrres)
        *itrres = pre::acoth(*itrarrx);
    return res;
}

} // namespace pre
