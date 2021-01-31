/*-*- C++ -*-*/
#pragma once

namespace pre {

/// A type list.
template <typename... T>
struct type_list {
    template <size_t N>
    using type = typename std::tuple_element<N, std::tuple<T...>>::type;
};

/// A type list concatenator.
template <typename T, typename U>
struct type_list_cat {
    using type = type_list<T, U>;
};

template <typename T, typename... U>
struct type_list_cat<T, type_list<U...>> {
    using type = type_list<T, U...>;
};

template <typename... T, typename U>
struct type_list_cat<type_list<T...>, U> {
    using type = type_list<T..., U>;
};

template <typename... T, typename... U>
struct type_list_cat<type_list<T...>, type_list<U...>> {
    using type = type_list<T..., U...>;
};

/// An indexed type.
template <size_t N, typename T>
struct indexed_type {
    static constexpr size_t index = N;
    using type = T;
};

/// An indexed type list.
template <size_t N, typename... T>
struct indexed_type_list;

template <typename... T>
using indexed_type_list_t = typename indexed_type_list<0, T...>::type;

template <size_t N, typename T, typename... U>
struct indexed_type_list<N, T, U...> {
    using type = typename type_list_cat<
            indexed_type<N, T>,
            typename indexed_type_list<N + 1, U...>::type>::type;
};

template <size_t N, typename T>
struct indexed_type_list<N, T> {
    using type = type_list<indexed_type<N, T>>;
};

template <>
struct indexed_type_list<0> {
    using type = type_list<>;
};

/// A callable indexed type list.
template <typename T>
struct callable_indexed_type_list;

template <typename T>
using callable_indexed_type_list_t =
        typename callable_indexed_type_list<T>::type;

template <typename T, typename... Args>
struct callable_indexed_type_list<T(Args...)> {
    using type = indexed_type_list_t<Args...>;
};

template <typename T, typename... Args>
struct callable_indexed_type_list<T (*)(Args...)> {
    using type = indexed_type_list_t<Args...>;
};

template <typename T, typename Class, typename... Args>
struct callable_indexed_type_list<T (Class::*)(Args...)> {
    using type = indexed_type_list_t<Args...>;
};

template <typename T, typename Class, typename... Args>
struct callable_indexed_type_list<T (Class::*)(Args...) const> {
    using type = indexed_type_list_t<Args...>;
};

template <concepts::functor T>
struct callable_indexed_type_list<T>
    : callable_indexed_type_list<decltype(&T::operator())> {};

template <typename T>
struct callable_indexed_type_list<T&> : callable_indexed_type_list<T> {};

template <typename T>
struct callable_indexed_type_list<T&&> : callable_indexed_type_list<T> {};

} // namespace pre
