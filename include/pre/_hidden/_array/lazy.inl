/*-*- C++ -*-*/
#pragma once

namespace pre {

template <typename Func, size_t Rank>
struct LazyArray {
    using Result = std::decay_t<std::invoke_result_t<Func, MultiIndex<Rank>>>;
    Func func;
    MultiIndex<Rank> sizes;
    explicit constexpr LazyArray(Func&& f, const MultiIndex<Rank>& s) noexcept
        : func(f), sizes(s) {
    }
    template <typename... Args>
    constexpr auto operator()(Args&&... args) const noexcept {
        return std::invoke(
                func, MultiIndex<Rank>{std::forward<Args>(args)...});
    }
};

template <typename Func, size_t Rank>
LazyArray(Func&&, const MultiIndex<Rank>&) -> LazyArray<Func, Rank>;

/// Find minimum.
template <typename Func>
[[gnu::always_inline]] constexpr auto min(
        const LazyArray<Func, 1>& arr) noexcept {
    using Result = typename LazyArray<Func, 1>::Result;
    if (arr.sizes[0] == 0) {
        return Result{};
    }
    else {
        Result res = arr(0);
        for (ssize_t k = 1; k < arr.sizes[0]; k++)
            res = std::min(res, arr(k));
        return res;
    }
}

/// Find maximum.
template <typename Func>
[[gnu::always_inline]] constexpr auto max(
        const LazyArray<Func, 1>& arr) noexcept {
    using Result = typename LazyArray<Func, 1>::Result;
    if (arr.sizes[0] == 0) {
        return Result{};
    }
    else {
        Result res = arr(0);
        for (ssize_t k = 1; k < arr.sizes[0]; k++)
            res = std::max(res, arr(k));
        return res;
    }
}

/// Sum all.
template <typename Func, size_t Rank>
[[gnu::always_inline]] constexpr auto sum(
        const LazyArray<Func, Rank>& arr) noexcept {
    using Result = typename LazyArray<Func, Rank>::Result;
    Result res = {};
    arr.sizes.for_each([&](const auto& k) constexpr noexcept {
        res += arr(k);
    });
    return res;
}

/// Vector dot.
template <typename Func0, typename Func1>
[[gnu::always_inline]] constexpr auto dot(
        const LazyArray<Func0, 1>& arr0,
        const LazyArray<Func1, 1>& arr1) noexcept {
    ASSERT(arr0.sizes == arr1.sizes);
    using Result0 = typename LazyArray<Func0, 1>::Result;
    using Result1 = typename LazyArray<Func1, 1>::Result;
    using Result = decltype(Result0() * Result1());
    Result res = {};
    for (ssize_t k = 0; k < arr0.sizes[0]; k++) {
        res += arr0(k) * arr1(k);
    }
    return res;
}

/// Matrix dot.
template <typename Func0, typename Func1>
[[gnu::always_inline]] constexpr auto dot(
        const LazyArray<Func0, 2>& arr0,
        const LazyArray<Func1, 2>& arr1) noexcept {
    ASSERT(arr0.sizes[1] == arr1.sizes[0]);
    using Result0 = typename LazyArray<Func0, 2>::Result;
    using Result1 = typename LazyArray<Func1, 2>::Result;
    using Result = decltype(Result0() * Result1());
    return LazyArray(
            [&](const auto& k) constexpr noexcept {
                Result res = {};
                for (ssize_t innerk = 0; innerk < arr0.sizes[1]; innerk++) {
                    res += arr0(k[0], innerk) * arr1(innerk, k[1]);
                }
                return res;
            },
            MultiIndex{arr0.sizes[0], arr1.sizes[1]});
}

/// Generalized tensor dot.
template <
        typename Func0,
        typename Func1,
        size_t Rank0,
        size_t Rank1,
        size_t OmitRank>
[[gnu::always_inline]] constexpr auto tensor_dot(
        const LazyArray<Func0, Rank0>& arr0,
        const LazyArray<Func1, Rank1>& arr1,
        const MultiIndex<OmitRank>& dims0,
        const MultiIndex<OmitRank>& dims1) noexcept {
    auto [kept0, omitted0] = arr0.sizes.omit(dims0);
    auto [kept1, omitted1] = arr1.sizes.omit(dims1);
    ASSERT(omitted0 == omitted1);
    MultiIndex<OmitRank> omitted = omitted0;
    using Result0 = typename LazyArray<Func0, Rank0>::Result;
    using Result1 = typename LazyArray<Func1, Rank1>::Result;
    using Result = decltype(Result0() * Result1());
    return LazyArray(
            [&arr0, &arr1, &dims0, &dims1,
             omitted ](const auto& k) constexpr noexcept {
                Result res = {};
                auto itr = k.begin();
                MultiIndex<Rank0> k0(itr, dims0);
                MultiIndex<Rank1> k1(itr, dims1);
                omitted.for_each([&](const auto& innerk) {
                    for (size_t dim = 0; dim < OmitRank; dim++) {
                        k0[dims0[dim]] = innerk[dim];
                        k1[dims1[dim]] = innerk[dim];
                    }
                    res += arr0(k0) * arr1(k1);
                });
                return res;
            },
            kept0.join(kept1));
}

/// Transpose dimensions.
template <typename Func>
[[gnu::always_inline]] constexpr auto transpose(
        const LazyArray<Func, 2>& arr) {
    return LazyArray(
            [&](const auto& k) constexpr noexcept {
                return arr(MultiIndex{k[1], k[0]});
            },
            MultiIndex{arr.sizes[1], arr.sizes[0]});
}

/// Transpose dimensions.
template <typename Func, size_t Rank>
[[gnu::always_inline]] constexpr auto transpose(
        const LazyArray<Func, Rank>& arr,
        const std::type_identity_t<MultiIndex<Rank>>& ord) noexcept
        requires(Rank > 2) {
    MultiIndex<Rank> sizes;
    for (size_t dim = 0; dim < Rank; dim++)
        sizes[dim] = arr.sizes[ord[dim]];
    return LazyArray(
            [&](const auto& k) constexpr noexcept {
                MultiIndex<Rank> reorder;
                for (size_t dim = 0; dim < Rank; dim++)
                    reorder[dim] = k[ord[dim]];
                return arr(reorder);
            },
            sizes);
}

} // namespace pre

#include "lazy_operators.inl"
