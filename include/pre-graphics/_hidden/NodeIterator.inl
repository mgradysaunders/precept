/*-*- C++ -*-*/
#pragma once

namespace pre {

namespace tree_helpers {

template <typename Traits, bool IsConst>
struct NodeIterator {
  public:
    using Node = typename Traits::Node;

    typedef std::decay_t<decltype(Traits::value(nullptr))> value_type;

    typedef std::conditional_t<IsConst, const value_type&, value_type&>
            reference;

    typedef std::conditional_t<IsConst, const value_type*, value_type*>
            pointer;

    typedef std::ptrdiff_t difference_type;

    typedef std::forward_iterator_tag iterator_category;

  public:
    constexpr NodeIterator() noexcept = default;

    template <typename Ptr>
    constexpr NodeIterator(Ptr ptr) noexcept : node(ptr) {
        for (; node and not Traits::has_value(const_cast<Node*>(node));
             node = node->next) {
        }
    }

    constexpr NodeIterator& operator++() noexcept {
        node = node->next;
        for (; node and not Traits::has_value(const_cast<Node*>(node));
             node = node->next) {
        }
        return *this;
    }

    constexpr NodeIterator operator++(int) noexcept {
        NodeIterator copy = *this;
        operator++();
        return copy;
    }

    constexpr reference operator*() noexcept {
        return Traits::value(const_cast<Node*>(node));
    }

    constexpr pointer operator->() noexcept {
        return &Traits::value(const_cast<Node*>(node));
    }

    constexpr operator bool() const noexcept {
        return node != nullptr;
    }

    constexpr operator NodeIterator<Traits, true>() const noexcept {
        return node;
    }

    template <typename Other>
    constexpr bool operator==(const Other& other) const noexcept {
        return node == other.node;
    }

    template <typename Other>
    constexpr bool operator!=(const Other& other) const noexcept {
        return node != other.node;
    }

  public:
    std::conditional_t<IsConst, const Node*, Node*> node = {};
};

} // namespace tree_helpers

} // namespace pre
