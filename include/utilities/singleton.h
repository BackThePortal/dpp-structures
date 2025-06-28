//
// Created by backo on 26/06/25.
//

#ifndef DPP_STRUCTURES_SINGLETON_H
#define DPP_STRUCTURES_SINGLETON_H

#include <type_traits>

namespace dpp_structures {
    template<typename T>
    class singleton {
    private:
        static T* instance;

    public:
        static T* get_instance() {
            if (!singleton<T>::instance) {
                singleton<T>::instance = new T();
            }
            return singleton<T>::instance;
        }
    };
}

template<typename T>
T* dpp_structures::singleton<T>::instance = nullptr;

#endif //DPP_STRUCTURES_SINGLETON_H
