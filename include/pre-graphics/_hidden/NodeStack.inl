/*-*- C++ -*-*/
#pragma once

#include <vector>
#include <pre/memory>

namespace pre {

namespace tree_helpers {

constexpr size_t StaticStackSize = 64;

template <typename Node, bool IsDynamic>
struct NodeStack;

template <typename Node>
struct NodeStack<Node, false> {
    constexpr explicit NodeStack(size_t, const Node* root = nullptr) noexcept {
        push(root);
    }

    constexpr void push(const Node* node) noexcept {
        if (node)
            todo.push(const_cast<Node*>(node));
    }

    constexpr void push_if(bool cond, const Node* node) noexcept {
        if (cond && node)
            todo.push(const_cast<Node*>(node));
    }

    constexpr Node* pop() noexcept {
        return todo.pop();
    }

    constexpr bool empty() noexcept {
        return todo.empty();
    }

    StaticStack<Node*, StaticStackSize> todo;
};

template <typename Node>
struct NodeStack<Node, true> {
    explicit NodeStack(size_t optimal_depth, const Node* root = nullptr) {
        todo.reserve(2 * optimal_depth);
        push(root);
    }

    void push(const Node* node) {
        if (node)
            todo.push_back(const_cast<Node*>(node));
    }

    void push_if(bool cond, const Node* node) {
        if (cond && node)
            todo.push_back(const_cast<Node*>(node));
    }

    Node* pop() {
        Node* node = todo.back();
        todo.pop_back();
        return node;
    }

    bool empty() noexcept {
        return todo.empty();
    }

    std::vector<Node*> todo;
};

} // namespace tree_helpers

} // namespace pre
