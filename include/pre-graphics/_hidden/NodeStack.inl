/*-*- C++ -*-*/
#pragma once

#include <vector>
#include <pre/memory>

namespace pre {

namespace tree_helpers {

template <typename Node, bool IsDynamic>
class NodeStack;

template <typename Node>
class NodeStack<Node, false> {
  public:
    constexpr explicit NodeStack(size_t, const Node* root = nullptr) noexcept {
        push(root);
    }
    constexpr void push(const Node* node) noexcept {
        if (node)
            nodes_.push(const_cast<Node*>(node));
    }
    constexpr void push_if(bool cond, const Node* node) noexcept {
        if (cond && node)
            nodes_.push(const_cast<Node*>(node));
    }
    constexpr Node* pop() noexcept {
        return nodes_.pop();
    }
    constexpr bool empty() noexcept {
        return nodes_.empty();
    }

  private:
    StaticStack<Node*, 64> nodes_;
};

template <typename Node>
class NodeStack<Node, true> {
  public:
    explicit NodeStack(size_t optimal_depth, const Node* root = nullptr) {
        nodes_.reserve(2 * optimal_depth);
        push(root);
    }
    void push(const Node* node) {
        if (node)
            nodes_.push_back(const_cast<Node*>(node));
    }
    void push_if(bool cond, const Node* node) {
        if (cond && node)
            nodes_.push_back(const_cast<Node*>(node));
    }
    Node* pop() {
        Node* node = nodes_.back();
        nodes_.pop_back();
        return node;
    }
    bool empty() noexcept {
        return nodes_.empty();
    }

  private:
    std::vector<Node*> nodes_;
};

} // namespace tree_helpers

} // namespace pre
