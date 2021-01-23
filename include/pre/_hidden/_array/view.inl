/*-*- C++ -*-*/
#pragma once

namespace pre {

/// Slice for array views.
struct Slice {
    constexpr Slice() noexcept = default;

    constexpr Slice(ssize_t k) noexcept : from(k), to(k + 1) {
    }

    constexpr Slice(ssize_t a, ssize_t b) noexcept : from(a), to(b) {
    }

    /// Index to slice from.
    ssize_t from = 0;

    /// Index to slice to (non-inclusive).
    ssize_t to = 0;

  private:
    [[gnu::always_inline]] constexpr void apply(
            auto*& first, ssize_t& size, ssize_t& skip) noexcept {
        if (from < 0)
            from += size + 1;
        if (to < 0)
            to += size + 1;
        ssize_t diff = to - from;
        if (diff < 0) {
            --from;
            --to;
        }
        first += skip * from;
        size = diff < 0 ? -diff : diff;
        skip = diff < 0 ? -skip : skip;
    }

    template <typename, size_t>
    friend struct ArrayView;
};

namespace concepts {

template <typename T>
concept integral_or_slice =
        std::integral<std::decay_t<T>> || std::same_as<std::decay_t<T>, Slice>;

} // namespace concepts

/// Slice index convenience type.
enum SliceInt : ssize_t {};

/// Slice index `operator|` to form slice.
constexpr Slice operator|(SliceInt a, SliceInt b) noexcept {
    return {ssize_t(a), ssize_t(b)};
}

constexpr Slice operator|(SliceInt a, std::integral auto b) noexcept {
    return {ssize_t(a), ssize_t(b)};
}

constexpr Slice operator|(std::integral auto a, SliceInt b) noexcept {
    return {ssize_t(a), ssize_t(b)};
}

template <typename Value, size_t Rank>
struct ArrayView_initializer_list {
    using type = std::initializer_list<
            typename ArrayView_initializer_list<Value, Rank - 1>::type>;
};

template <typename Value>
struct ArrayView_initializer_list<Value, 0> {
    using type = Value;
};

/// An array view.
template <typename Value, size_t Rank>
struct ArrayView {
  public:
    /// Initializer list.
    using initializer_list =
            typename ArrayView_initializer_list<Value, Rank>::type;

    /// Iterator.
    struct iterator {
      public:
        typedef std::ptrdiff_t difference_type;

        typedef std::
                conditional_t<Rank == 1, Value, ArrayView<Value, Rank - 1>>
                        value_type;

        typedef std::
                conditional_t<Rank == 1, Value&, ArrayView<Value, Rank - 1>>
                        reference;

        typedef std::conditional_t<Rank == 1, Value*, void> pointer;

        typedef std::random_access_iterator_tag iterator_category;

      private:
        constexpr iterator() noexcept = default;

        [[gnu::nonnull]] constexpr iterator(
                Value* first,
                const ssize_t* sizes,
                const ssize_t* skips) noexcept
            : first_(first), sizes_(sizes), skips_(skips) {
        }

      public:
        constexpr iterator& operator++() noexcept {
            first_ += *skips_;
            return *this;
        }

        constexpr iterator& operator--() noexcept {
            first_ -= *skips_;
            return *this;
        }

        constexpr iterator operator++(int) noexcept {
            const iterator res = *this;
            operator++();
            return res;
        }

        constexpr iterator operator--(int) noexcept {
            const iterator res = *this;
            operator--();
            return res;
        }

        constexpr iterator& operator+=(difference_type count) noexcept {
            first_ += *skips_ * count;
            return *this;
        }

        constexpr iterator operator+(difference_type count) const noexcept {
            return iterator(*this) += count;
        }

        friend constexpr iterator operator+(
                difference_type count, iterator itr) noexcept {
            return itr + count;
        }

        constexpr iterator& operator-=(difference_type count) noexcept {
            first_ -= *skips_ * count;
            return *this;
        }

        constexpr iterator operator-(difference_type count) const noexcept {
            return iterator(*this) -= count;
        }

        constexpr difference_type operator-(iterator other) const noexcept {
            return (first_ - other.first_) / *other.skips_;
        }

        constexpr auto operator<=>(const iterator& other) const noexcept {
            if (*skips_ > 0) {
                if (first_ < other.first_)
                    return std::strong_ordering::less;
                if (first_ > other.first_)
                    return std::strong_ordering::greater;
            }
            else {
                if (first_ > other.first_)
                    return std::strong_ordering::less;
                if (first_ < other.first_)
                    return std::strong_ordering::greater;
            }
            return std::strong_ordering::equal;
        }

        constexpr bool operator==(const iterator&) const noexcept = default;

        constexpr reference operator*() noexcept {
            if constexpr (Rank == 1)
                return *first_;
            else
                return {first_, sizes_ + 1, skips_ + 1};
        }

      private:
        Value* first_ = nullptr;
        const ssize_t* sizes_ = nullptr;
        const ssize_t* skips_ = nullptr;

        friend struct ArrayView;
    };

  public:
    constexpr ArrayView() noexcept = default;

    template <concepts::random_access_range_of<Value> Range>
    constexpr ArrayView(Range& range) requires(Rank == 1) {
        first = &*std::ranges::begin(range);
        sizes[0] = std::ranges::size(range);
        skips[0] = 1;
    }

    constexpr ArrayView(Value* ptr, ssize_t size0, ssize_t skip0 = 1) noexcept
            requires(Rank == 1) {
        first = ptr;
        sizes[0] = size0;
        skips[0] = skip0;
    }

    [[gnu::nonnull]] constexpr ArrayView(
            Value* ptr, const ssize_t* psizes, const ssize_t* pskips) noexcept
        : first(ptr) {
        std::copy(psizes, psizes + Rank, &sizes[0]);
        std::copy(pskips, pskips + Rank, &skips[0]);
    }

    constexpr ArrayView(Value* ptr, const MultiIndex<Rank>& sz) noexcept
        : first(ptr), sizes(sz) {
        // Default implementation.
        skips[Rank - 1] = 1;
        for (size_t dim = Rank - 1; dim > 0; --dim)
            skips[dim - 1] = skips[dim] * sizes[dim];
    }

    constexpr ArrayView(
            Value* ptr,
            const MultiIndex<Rank>& sz,
            const MultiIndex<Rank>& sk) noexcept
        : first(ptr), sizes(sz), skips(sk) {
    }

    constexpr ArrayView(const ArrayView&) noexcept = default;

    constexpr ArrayView(ArrayView&&) noexcept = default;

  public:
    constexpr size_t rank() noexcept {
        return Rank;
    }

    /// Size (same as `sizes[0]`).
    constexpr ssize_t size() noexcept {
        return sizes[0];
    }

    /// Is empty?
    ///
    /// \note
    /// This returns true if any size is zero, so that a many-dimensional
    /// view that ultimately contains no elements is considered empty.
    ///
    constexpr bool empty() noexcept {
        return sizes.prod() == 0;
    }

    /// Is square? (All sizes equal?)
    constexpr bool is_square() noexcept requires(Rank > 1) {
        for (size_t k = 1; k < Rank; k++)
            if (sizes[k] != sizes[0])
                return false;
        return true;
    }

    constexpr iterator begin() noexcept {
        return {first, &sizes[0], &skips[0]};
    }

    constexpr iterator end() noexcept {
        return begin() + sizes[0];
    }

  public:
    /// \name Assignment
    /** \{ */

    constexpr ArrayView& operator=(const ArrayView&) = default;

    constexpr ArrayView& operator=(ArrayView&&) = default;

    constexpr ArrayView& operator=(const Value& value) noexcept {
        for (auto itr = begin(); itr != end(); ++itr)
            *itr = value;
        return *this;
    }

    constexpr ArrayView& operator=(initializer_list list) noexcept {
        ASSERT(size() == ssize_t(list.size()));
        auto itr1 = list.begin();
        auto itr2 = begin();
        for (; itr2 != end(); ++itr1, ++itr2)
            *itr2 = *itr1;
        return *this;
    }

    template <typename Func>
    constexpr ArrayView& operator=(LazyArray<Func, Rank>&& lazy) noexcept {
        ASSERT(sizes == lazy.sizes);
        MultiIndex<Rank> k = {};
        size_t n = sizes.prod();
        while (n-- > 0) {
            operator[](k) = std::forward<LazyArray<Func, Rank>>(lazy)(k);
            k.increment(sizes);
        }
        return *this;
    }

    /** \} */

  public:
    /// \name Indexing
    ///
    /// Views support indexing with the ordinary `operator[]` with an
    /// integer argument, such that the values can be accessed with the
    /// same syntax as built-in arrays, for example `v[0][1][2]`. It is
    /// also possible to use `operator()` instead, like `v(0, 1, 2)`.
    ///
    /// _Slicing_ refers to the process of forming a range or subset
    /// of a view in one or more arbitrary dimensions. For example, if `v`
    /// is a rank-1 view and we want to form a smaller rank-1 view that
    /// only contains values from index 2 (inclusive) up to index 5
    /// (non-inclusive), then what we are looking for is `v[Slice(2, 5)]`.
    /// If we want to slice column 1 of a rank-2 view `m` that is 4 rows
    /// by 3 columns, then we would use `operator()` with a slice in the
    /// first dimension `m(Slice(0, 4), 1)`. It is also possible to
    /// slice along more than one axis at once, to obtain a block in a
    /// matrix for instance.
    ///
    /// \note
    /// It necessary to use `operator()` when slicing along an axis that
    /// is not the last axis. Notice that `m[Slice(0, 4)][1]` does _not_
    /// return column 1 because it is actually two applications of
    /// `operator[]`.
    ///
    /** \{ */

    constexpr decltype(auto) operator[](MultiIndex<Rank> k) const noexcept {
        return (*(first + k.linearize(skips)));
    }

    template <concepts::integral_or_slice P>
    constexpr decltype(auto) operator[](P p) noexcept {
        if constexpr (std::integral<std::decay_t<P>>) {
            if constexpr (std::signed_integral<std::decay_t<P>>)
                if (p < 0)
                    p += sizes[0];
            if constexpr (Rank == 1)
                return (*(begin() + p));
            else
                return *(begin() + p);
        }
        else {
            ArrayView<Value, Rank> res = *this;
            Slice slice = p;
            slice.apply(res.first, res.sizes[0], res.skips[0]);
            return res;
        }
    }

    template <concepts::integral_or_slice P, concepts::integral_or_slice... Q>
    constexpr decltype(auto) operator()(P p, Q&&... q) noexcept {
        constexpr size_t IndexCount = 1 + sizeof...(Q);
        constexpr size_t SliceCount =
                (size_t(std::same_as<std::decay_t<P>, Slice>) + ... +
                 size_t(std::same_as<std::decay_t<Q>, Slice>));
        if constexpr (SliceCount == 0) {
            if constexpr (sizeof...(Q) == 0) {
                return operator[](p);
            }
            else {
                return operator[](p).operator()(std::forward<Q>(q)...);
            }
        }
        else {
            Slice slices[] = {p, std::forward<Q>(q)...};
            constexpr bool is_slice[] = {
                    !std::integral<std::decay_t<P>>,
                    !std::integral<std::decay_t<Q>>...};
            ArrayView<Value, Rank - IndexCount + SliceCount> res;
            res.first = first;
            for (size_t dim = 0, off = 0; dim < IndexCount; dim++) {
                if (is_slice[dim]) {
                    res.sizes[off] = sizes[dim];
                    res.skips[off] = skips[dim];
                    slices[dim].apply(
                            res.first, res.sizes[off], res.skips[off]);
                    ++off;
                }
                else {
                    ssize_t pos = slices[dim].from;
                    if (pos < 0)
                        pos += sizes[dim];
                    res.first += skips[dim] * pos;
                }
            }
            std::copy(
                    &sizes[0] + IndexCount, //
                    &sizes[0] + Rank, &res.sizes[0] + SliceCount);
            std::copy(
                    &skips[0] + IndexCount, //
                    &skips[0] + Rank, &res.skips[0] + SliceCount);
            return res;
        }
    }

    /** \} */

  public:
    /// \name Lazyification
    /** \{ */

    /// Form lazy temporary object.
    [[gnu::always_inline]] constexpr auto lazy() noexcept {
        const auto copy = *this;
        return LazyArray(
                [=](auto k) constexpr noexcept { return copy[k]; }, sizes);
    }

    /// Dereference operator to form lazy array.
    [[gnu::always_inline]] constexpr auto operator*() noexcept {
        return lazy();
    }

    /** \} */

  public:
    /// \name Utilities
    /** \{ */

    constexpr void get_each(std::output_iterator<Value> auto itr) {
        sizes.for_each([&](auto k) constexpr noexcept {
            *itr++ = operator[](k);
        });
    }

    constexpr void set_each(std::input_iterator auto itr) {
        sizes.for_each([&](auto k) constexpr noexcept {
            operator[](k) = *itr++;
        });
    }

    template <typename Other>
    constexpr void swap_each(ArrayView<Other, Rank> other) noexcept {
        ASSERT(sizes == other.sizes);
        auto& lhs = *this;
        auto& rhs = other;
        sizes.for_each([&](auto k) constexpr noexcept {
            std::swap(lhs[k], rhs[k]);
        });
    }

    constexpr bool any() noexcept {
        for (auto each : *this)
            if constexpr (Rank == 1) {
                if (each)
                    return true;
            }
            else {
                if (each.any())
                    return true;
            }
        return false;
    }

    constexpr bool all() noexcept {
        for (auto each : *this)
            if constexpr (Rank == 1) {
                if (!each)
                    return false;
            }
            else {
                if (!each.all())
                    return false;
            }
        return true;
    }

    constexpr ssize_t argmin() noexcept requires(Rank == 1) {
        return std::min_element(begin(), end()) - begin();
    }

    constexpr ssize_t argmax() noexcept requires(Rank == 1) {
        return std::max_element(begin(), end()) - begin();
    }

    constexpr auto sum() noexcept requires(Rank == 1) {
        std::decay_t<Value> res = 0;
        for (Value each : *this)
            res += each;
        return res;
    }

    constexpr auto prod() noexcept requires(Rank == 1) {
        std::decay_t<Value> res = 1;
        for (Value each : *this)
            res *= each;
        return res;
    }

    constexpr ssize_t rows() noexcept requires(Rank == 2) {
        return sizes[0];
    }

    constexpr ssize_t cols() noexcept requires(Rank == 2) {
        return sizes[1];
    }

    constexpr ArrayView<Value, 1> row(ssize_t p) noexcept requires(Rank == 2) {
        return (*this)[p];
    }

    constexpr ArrayView<Value, 1> col(ssize_t p) noexcept requires(Rank == 2) {
        return (*this)(Slice(0, rows()), p);
    }

    constexpr void swap_rows(ssize_t k0, ssize_t k1) noexcept
            requires(Rank == 2) {
        if (k0 != k1)
            row(k0).swap_each(row(k1));
    }

    constexpr void swap_cols(ssize_t k0, ssize_t k1) noexcept
            requires(Rank == 2) {
        if (k0 != k1)
            col(k0).swap_each(col(k1));
    }

    constexpr ArrayView<Value, 1> diag(ssize_t p = 0) noexcept
            requires(Rank == 2) {
        ssize_t size0 = sizes[0], size1 = sizes[1];
        ssize_t skip0 = skips[0], skip1 = skips[1];
        if (!(p > -size1 && p < +size0))
            return ArrayView<Value, 1>{};
        if (p < 0) {
            p = -p;
            std::swap(size0, size1);
            std::swap(skip0, skip1);
        }
        return ArrayView<Value, 1>(
                first + skip0 * p, std::min(size0 - p, size1), skip0 + skip1);
    }

    constexpr ArrayView transpose() noexcept requires(Rank == 2) {
        ArrayView res = *this;
        std::swap(res.sizes[0], res.sizes[1]);
        std::swap(res.skips[0], res.skips[1]);
        return res;
    }

    constexpr auto trace() noexcept requires(Rank == 2) {
        return diag().sum();
    }

    /** \} */

  public:
    /// Implict cast as const.
    constexpr operator ArrayView<const Value, Rank>() const noexcept {
        return {first, &sizes[0], &skips[0]};
    }

  public:
    Value* first = nullptr;
    MultiIndex<Rank> sizes = {};
    MultiIndex<Rank> skips = {};

  public:
    /// Read from `std::basic_istream`.
    ///
    /// Format is `[v0,v1,...]`.
    /// Sets `std::ios_base::failbit` on error.
    ///
    template <concepts::istream Stream>
    friend Stream& operator>>(Stream& stream, ArrayView arr) {
        using Char = typename Stream::char_type;
        using CharTraits = typename Stream::traits_type;
        auto consume = [&](char what) {
            Char ch;
            if (!(stream >> ch) ||
                !CharTraits::eq(ch, CharTraits::to_char_type(what))) {
                stream.setstate(std::ios_base::failbit);
                return false;
            }
            return true;
        };
        if (consume('[')) {
            if (!arr.empty()) {
                auto itr = arr.begin();
                stream >> *itr++;
                while (itr != arr.end()) {
                    if (!consume(','))
                        return stream;
                    stream >> *itr++;
                }
            }
            consume(']');
        }
        return stream;
    }

    /// Write into `std::basic_ostream`.
    ///
    /// Format is `[v0,v1,...]`.
    ///
    template <concepts::ostream Stream>
    friend Stream& operator<<(Stream& stream, ArrayView arr) {
        stream << '[';
        if (!arr.empty()) {
            auto itr = arr.begin();
            stream << *itr++;
            while (itr != arr.end()) {
                stream << ',';
                stream << *itr++;
            }
        }
        stream << ']';
        return stream;
    }
};

template <typename T, std::integral U>
ArrayView(T*, U) -> ArrayView<T, 1>;

template <std::ranges::random_access_range Range>
ArrayView(Range&) -> ArrayView<std::ranges::range_value_t<Range>, 1>;

template <std::ranges::random_access_range Range>
ArrayView(const Range&)
        -> ArrayView<const std::ranges::range_value_t<Range>, 1>;

} // namespace pre

#include "view_operators.inl"
