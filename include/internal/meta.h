//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_META_H
#define DPP_STRUCTURES_META_H

#include <concepts>

namespace dpp_structures::internal {
    template<class T, class U>
    concept derived = std::is_base_of<U, T>::value;

    template<typename T>
    typename std::add_rvalue_reference<T>::type declval();

    template<typename... Types>
    struct pack {
    };

    template<typename T, typename... Args>
    struct is_friend_constructible {
    private:
        template<typename U, typename = decltype(U(std::declval<Args>()...))>
        static std::true_type test(int) { return {}; };

        template<typename>
        static std::false_type test(...) { return {}; };

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template<typename T, typename... Args>
    constexpr bool is_friend_constructible_v = is_friend_constructible<T, Args...>::value;
}
#endif//DPP_STRUCTURES_META_H
