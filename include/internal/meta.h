//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_META_H
#define DPP_STRUCTURES_META_H

#include <concepts>
#include <utility>
#include <optional>
#include <dpp/dpp.h>

namespace dpp_structures::internal {
    template<typename...>
    inline constexpr bool all_unique = true;

    template<typename T, typename... Rest>
    inline constexpr bool all_unique<T, Rest...> = (!std::is_same_v<T, Rest> && ...) && all_unique<Rest...>;

    template<typename T, typename U>
    concept derived = std::is_base_of<U, T>::value;


    template<typename T, template<auto...> class Template>
    struct is_instance_of : std::false_type {
    };

    // Specialization: matches T if it's an instantiation of the Template
    template<template<auto...> class Template, auto... Args>
    struct is_instance_of<Template<Args...>, Template> : std::true_type {
    };

    namespace detail {

        template<bool Const = false, typename R = void, typename C = void, typename... Args>
        struct callable_info_detail {
            using class_type = C;
            using return_type = R;
            using arguments = std::tuple<Args...>;
            static constexpr bool is_const = Const;

            explicit callable_info_detail(auto) {}
        };

        // Non-const member function pointer
        template<typename R, typename C, typename... Args>
        callable_info_detail(R (C::*)(Args...))
        -> callable_info_detail<false, R, C, Args...>;

        // Const member function pointer
        template<typename R, typename C, typename... Args>
        callable_info_detail(R (C::*)(Args...) const)
        -> callable_info_detail<true, R, C, Args...>;

        // Free function pointer (treated like member of void)
        template<typename R, typename... Args>
        callable_info_detail(R (*)(Args...))
        -> callable_info_detail<false, R, void, Args...>;

        // Concepts for constraints
        template<typename T>
        concept MemberFunctionPointer = std::is_member_function_pointer_v<T>;

        template<typename T>
        concept FunctionPointer = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>>;

        template<typename T>
        concept Functor = !MemberFunctionPointer<T> && !FunctionPointer<T>;

        // Callable info helper specializations using requires

        template<typename T>
        struct callable_info_helper {

        };

        template<typename T> requires MemberFunctionPointer<T>
        struct callable_info_helper<T> {
            using type = decltype(callable_info_detail(std::declval<T>()));
        };

        template<typename T> requires FunctionPointer<T>
        struct callable_info_helper<T> {
            using type = decltype(callable_info_detail(std::declval<T>()));
        };

        template<typename T> requires Functor<T>
        struct callable_info_helper<T> {
            using type = decltype(callable_info_detail(std::declval<decltype(&T::operator())>()));
        };

        template<class T>
        struct member_type_helper;

        template<class C, class T>
        struct member_type_helper<T C::*> {
            using type = T;
        };
    } // namespace detail

    template<typename T>
    using callable_info = typename detail::callable_info_helper<T>::type;


    template<class T>
    struct member_type
            : detail::member_type_helper<typename std::remove_cvref<T>::type> {
    };

    template<class T>
    using member_type_t = member_type<T>::type;


    template<typename T>
    typename std::add_rvalue_reference<T>::type declval();

    template<typename T>
    struct is_optional : std::false_type {
    };

    template<typename T>
    struct is_optional<std::optional<T>> : std::true_type {
    };

    template<typename... Types>
    struct pack {
    };

    template<typename T, typename... Args>
    struct is_friend_constructible {
    private:
        template<typename U, typename = decltype(U(std::declval<Args>()...))>
        static std::true_type test(int) {
            return {};
        };

        template<typename>
        static std::false_type test(...) {
            return {};
        };

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template<typename T, typename... Args>
    constexpr bool is_friend_constructible_v = is_friend_constructible<T, Args...>::value;

    template<auto Value>
    using get_type_from_value = std::remove_cvref_t<decltype(Value)>;

    template<std::size_t N, typename... Ts>
    using get_type_from_pack = std::tuple_element_t<N, std::tuple<Ts...>>;

    /**
     * Get the first type of a type template parameter pack.
     */
    template<typename... Ts>
    using first_type = get_type_from_pack<0, Ts...>;

    /**
     * Get the first value of a non-type template parameter pack.
     */
    template<auto T, auto... Ts>
    constexpr auto first_value = T;

    // Library-specific meta utilities

    template<typename T>
    struct is_event_router : std::false_type {
    };

    template<typename T>
    struct is_event_router<dpp::event_router_t<T>> : std::true_type {
    };

    template<typename T>
    struct is_event_router<dpp::event_router_t<T>&> : std::true_type {
    };

    template<typename T>
    struct event_router_traits {
    };

    template<typename T>
    struct event_router_traits<dpp::event_router_t<T>> {
        using event_type = T;
    };

    template<typename T>
    struct event_router_traits<dpp::event_router_t<T>&> : event_router_traits<dpp::event_router_t<T>> {
    };

}// namespace dpp_structures::internal
#endif//DPP_STRUCTURES_META_H
