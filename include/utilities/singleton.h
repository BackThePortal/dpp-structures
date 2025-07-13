//
// Created by backo on 26/06/25.
//

#ifndef DPP_STRUCTURES_SINGLETON_H
#define DPP_STRUCTURES_SINGLETON_H

#include <type_traits>
#include <exception>
#include "../internal/meta.h"

namespace dpp_structures {

    enum class SingletonInstanceType {
        POINTER,
        REFERENCE
    };

    class singleton_object_not_constructed_error : std::exception {
    };

    /*
     * TODO: make friendship-respectable is_constructible class. although new T(...) clearly fails if it is not
     *  constructible with the given arguments
     */

    template<typename T>
    class singleton_base {
    private:
        static T* instance;
    public:
        static T* get_instance() {
            if (singleton_base<T>::instance != nullptr) {
                return singleton_base<T>::instance;
            } else {
                if constexpr (internal::is_friend_constructible_v<T>) {
                    return singleton_base<T>::instance = new T();
                } else {
                    throw singleton_object_not_constructed_error();
                }
            }
        }

        /*
         * A first argument is forced, so the call to get_instance() isn't ambiguous
         */
        template<typename FirstArg, typename ...RestArgs>
        // requires std::is_constructible<T, FirstArg, RestArgs...>::value
        static T* get_instance(FirstArg first_arg, RestArgs... restArgs) {
            if (!singleton_base<T>::instance) {
                singleton_base<T>::instance = new T(first_arg, restArgs...);
            }
            return singleton_base<T>::instance;
        }

    };

    template<typename T, SingletonInstanceType instanceType = SingletonInstanceType::POINTER>
    class singleton;

    template<typename T>
    class singleton<T, SingletonInstanceType::POINTER> : public singleton_base<T> {
    public:
        using singleton_base<T>::get_instance;
    };

    template<typename T>
    class singleton<T, SingletonInstanceType::REFERENCE> : singleton_base<T> {
    private:
        static T* instance;

    public:
        static inline T& get_instance() {
            return *singleton_base<T>::get_instance();
        }

        template<typename FirstArg, typename ...RestArgs>
        static inline T& get_instance(FirstArg first_arg, RestArgs... restArgs) {
            return *singleton_base<T>::get_instance(first_arg, restArgs...);
        }
    };

    template<typename T>
    T* singleton_base<T>::instance = nullptr;

#define FRIEND_SINGLETON(...) \
friend ::dpp_structures::internal::is_friend_constructible<__VA_ARGS__>; \
friend ::dpp_structures::singleton_base<__VA_ARGS__>;
}


#endif //DPP_STRUCTURES_SINGLETON_H
