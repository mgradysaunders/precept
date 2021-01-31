/*-*- C++ -*-*/
#pragma once

namespace pre {

namespace concepts {

// clang-format off

template <typename T>
concept forward_link = 
    (requires (T& link) { { *link.value_ptr() } -> reference; } ||
     requires (T& link) { { &link.value } -> pointer; }) &&
     requires (T& link) {
        { *link.next } -> std::convertible_to<T&>;
    };

// clang-format on

} // namespace concepts

template <concepts::forward_link Link>
constexpr auto* link_get_value_ptr(Link& link) noexcept {
    constexpr bool has_value_ptr = requires(Link& link) {
        { *link.value_ptr() } -> concepts::reference;
    };
    if constexpr (has_value_ptr)
        return link.value_ptr();
    else 
        return &link.value;
}

template <typename>
struct LinkIterator;

/// A link iterator helper.
template <concepts::forward_link Link>
struct LinkIterator<Link*> {
  public:
    // clang-format off

    static constexpr bool is_const = std::is_const_v<Link>;

    static constexpr bool is_bidirectional = requires (Link& link) {
        { *link.prev } -> std::convertible_to<Link&>;
    };

    static constexpr bool is_ignorable = requires (Link& link) {
        { link.should_ignore() } -> std::convertible_to<bool>;
    };

    // clang-format on

  public:
    using value_type =
            std::decay_t<decltype(*link_get_value_ptr(std::declval<Link&>()))>;

    using reference =
            std::conditional_t<is_const, const value_type&, value_type&>;

    using pointer =
            std::conditional_t<is_const, const value_type*, value_type*>;

    using difference_type = std::ptrdiff_t;

    using iterator_category = std::conditional_t<
            is_bidirectional,
            std::bidirectional_iterator_tag,
            std::forward_iterator_tag>;

  public:
    constexpr LinkIterator() noexcept = default;

    constexpr LinkIterator(Link* l) noexcept : link(l) {
        if constexpr (is_ignorable)
            for (; link && link->should_ignore(); link = link->next) {
            }
    }

    constexpr LinkIterator& operator++() noexcept {
        link = link->next;
        if constexpr (is_ignorable)
            for (; link && link->should_ignore(); link = link->next) {
            }
        return *this;
    }

    constexpr LinkIterator& operator--() noexcept requires is_bidirectional {
        link = link->prev;
        if constexpr (is_ignorable)
            for (; link && link->should_ignore(); link = link->prev) {
            }
        return *this;
    }

    constexpr LinkIterator operator++(int) noexcept {
        const LinkIterator copy = *this;
        operator++();
        return copy;
    }

    constexpr LinkIterator operator--(int) noexcept requires is_bidirectional {
        const LinkIterator copy = *this;
        operator--();
        return copy;
    }

    constexpr reference operator*() noexcept {
        return *link_get_value_ptr(*link);
    }

    constexpr pointer operator->() noexcept {
        return link_get_value_ptr(*link);
    }

    constexpr bool operator==(LinkIterator<const Link*> other) const noexcept {
        return link == other.link;
    }

    constexpr bool operator!=(LinkIterator<const Link*> other) const noexcept {
        return link != other.link;
    }

    /// Implicit cast as bool (non-null check).
    constexpr operator bool() const noexcept {
        return link != nullptr;
    }

    /// Implicit cast as const.
    constexpr operator LinkIterator<const Link*>() const noexcept
            requires(!is_const) {
        return {link};
    }

  public:
    Link* link = nullptr;
};

template <typename>
struct LinkList;

/// A link list helper.
///
/// This structure provides container access to a doubly-linked
/// list, but does not manage any memory. It is left to the user to
/// allocate and deallocate links.
///
template <concepts::forward_link Link>
struct LinkList<Link*> {

    /// \name Container typedefs
    /** \{ */

    typedef size_t size_type;

    typedef std::ptrdiff_t difference_type;

    typedef LinkIterator<Link*> iterator;

    typedef LinkIterator<const Link*> const_iterator;

    /** \} */

  public:

    /// \name Container API
    /** \{ */

    constexpr difference_type size() const noexcept {
        return std::distance(begin(), end());
    }

    constexpr bool empty() const noexcept {
        return begin() == end();
    }

    constexpr iterator begin() noexcept {
        return {head};
    }

    constexpr const_iterator begin() const noexcept {
        return {head};
    }

    constexpr const_iterator cbegin() const noexcept {
        return begin();
    }

    constexpr iterator end() noexcept {
        return {};
    }

    constexpr const_iterator end() const noexcept {
        return {};
    }

    constexpr const_iterator cend() const noexcept {
        return end();
    }

    /** \} */

  public:
    /// \name Utilities
    /** \{ */

    constexpr Link* link_before(Link* link) noexcept {
        if constexpr (iterator::is_bidirectional)
            if (link)
                return link->prev;
        Link* curr = head;
        for (; curr; curr = curr->next)
            if (curr->next == link)
                return curr;
        return nullptr;
    }

    /// Prepend link with \f$ O(1) \f$ complexity.
    constexpr void prepend(Link* link) noexcept {
        if (link) {
            link->next = head;
            if constexpr (iterator::is_bidirectional) {
                link->prev = nullptr;
                if (head)
                    head->prev = link;
            }
            head = link;
        }
    }

    /// Append link with \f$ O(n) \f$ complexity.
    constexpr void append(Link* link) noexcept {
        if (link) {
            link->next = nullptr;
            if constexpr (iterator::is_bidirectional)
                link->prev = nullptr;
            if (head == nullptr) {
                head = link;
            }
            else {
                Link* tail = link_before(nullptr);
                tail->next = link;
                if constexpr (iterator::is_bidirectional)
                    link->prev = tail;
            }
        }
    }

    /// Remove link at position.
    constexpr Link* remove(const_iterator pos) noexcept {
        Link* link = const_cast<Link*>(pos.link);
        if (link) {
            if constexpr (iterator::is_bidirectional) {
                if (link->prev)
                    link->prev->next = link->next;
                if (link->next)
                    link->next->prev = link->prev;
            }
            else {
                Link* prev = link_before(link);
                if (prev)
                    prev->next = link->next;
            }
            if (head == link)
                head = link->next;
        }
        return link;
    }

    template <typename Value>
    constexpr bool contains(const Value& value) const {
        return std::find(begin(), end(), value) != end();
    }

    /** \} */

  public:
    Link* head = nullptr;
};

} // namespace pre
