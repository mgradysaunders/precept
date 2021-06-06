/*-*- C++ -*-*/
#pragma once

namespace pre {

/// A multi-dimensional array index helper.
template <size_t R>
struct MultiIndex : ArrayLike<MultiIndex<R>, ssize_t> {
  public:
    constexpr MultiIndex() noexcept = default;

    template <std::integral... Ints>
    constexpr MultiIndex(Ints... ints) noexcept : values{ssize_t(ints)...} {
        static_assert(sizeof...(Ints) == R);
    }

    template <std::integral Int>
    constexpr MultiIndex(const std::array<Int, R>& arr) noexcept {
        std::copy(arr.begin(), arr.end(), &values[0]);
    }

    template <std::input_iterator Iterator, size_t S>
    constexpr MultiIndex(Iterator& itr, const MultiIndex<S>& ignore) noexcept {
        for (size_t dim = 0; dim < R; dim++)
            if (!ignore.contains(dim))
                values[dim] = *itr++;
    }

  public:
    /// \name Container API
    /** \{ */

    constexpr size_t size() const noexcept {
        return R;
    }

    constexpr ssize_t* begin() noexcept {
        return &values[0];
    }

    constexpr const ssize_t* begin() const noexcept {
        return &values[0];
    }

    constexpr ssize_t* end() noexcept {
        return &values[0] + R;
    }

    constexpr const ssize_t* end() const noexcept {
        return &values[0] + R;
    }

    constexpr bool contains(ssize_t value) const noexcept {
        return std::find(begin(), end(), value) != end();
    }

    /** \} */

  public:
    /// Increment, wrapping according to sizes.
    constexpr void increment(const MultiIndex<R>& sizes) noexcept {
        auto itr1 = sizes.rbegin();
        auto itr2 = this->rbegin();
        for (; itr2 < this->rend(); ++itr1, ++itr2) {
            ++*itr2;
            if (*itr2 >= *itr1)
                *itr2 = 0;
            else
                return;
        }
    }

    /// Decrement, wrapping according to sizes.
    constexpr void decrement(const MultiIndex<R>& sizes) noexcept {
        auto itr1 = sizes.rbegin();
        auto itr2 = this->rbegin();
        for (; itr2 < this->rend(); ++itr1, ++itr2) {
            --*itr2;
            if (*itr2 < 0)
                *itr2 = *itr1 - 1;
            else
                return;
        }
    }

    /// Linearize.
    constexpr ssize_t linearize(const MultiIndex<R>& skips) const noexcept {
        ssize_t res = 0;
        auto itr1 = skips.begin();
        auto itr2 = this->begin();
        for (; itr2 < this->end(); ++itr1, ++itr2)
            res += (*itr1) * (*itr2);
        return res;
    }

    /// Compute product.
    constexpr ssize_t prod() const noexcept {
        ssize_t res = values[0];
        for (size_t dim = 1; dim < R; dim++)
            res *= values[dim];
        return res;
    }

    /// Join/concatenate with other.
    template <size_t S>
    constexpr auto join(const MultiIndex<S>& other) const noexcept {
        MultiIndex<R + S> res;
        std::copy(this->begin(), this->end(), res.begin());
        std::copy(other.begin(), other.end(), res.begin() + R);
        return res;
    }

    /// Omit dimensions.
    template <size_t S>
    constexpr auto omit(const MultiIndex<S>& which) const noexcept
            requires(S < R) {
        MultiIndex<R - S> kept;
        MultiIndex<S> omitted;
        auto itr1 = kept.begin();
        auto itr2 = omitted.begin();
        for (size_t dim = 0; dim < R; dim++) {
            if (!which.contains(dim))
                *itr1++ = values[dim];
            else
                *itr2++ = values[dim];
        }
        return std::make_pair(kept, omitted);
    }

    /// Assuming this is a set of sizes, call functor for each multi-index.
    template <typename Func>
    [[gnu::always_inline]] constexpr void for_each(
            Func&& func) const noexcept {
        MultiIndex k = {};
        size_t n = prod();
        while (n-- > 0) {
            std::invoke(std::forward<Func>(func), k);
            k.increment(*this);
        }
    }

  public:
    ssize_t values[R] = {};

  public:
    constexpr auto operator<=>(const MultiIndex& other) const noexcept {
        auto itr1 = this->begin();
        auto itr2 = other.begin();
        for (; itr2 < other.end(); ++itr1, ++itr2) {
            if (*itr1 < *itr2)
                return std::strong_ordering::less;
            if (*itr1 > *itr2)
                return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    constexpr bool operator==(const MultiIndex& other) const noexcept {
        auto itr1 = this->begin();
        auto itr2 = other.begin();
        for (; itr2 < other.end(); ++itr1, ++itr2)
            if (*itr1 != *itr2)
                return false;
        return true;
    }
};

template <>
struct MultiIndex<0> {};

template <std::integral... Ints>
MultiIndex(Ints... ints) -> MultiIndex<sizeof...(Ints)>;

} // namespace pre
