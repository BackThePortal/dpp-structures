//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_FUNCTIONS_H
#define DPP_STRUCTURES_FUNCTIONS_H

#include <dpp/dpp.h>

namespace dpp_structures {

    template<typename Class, typename ReturnType, typename...Args>
    ReturnType apply(Class* c, ReturnType (Class::*func)(Args...), Args... args) {
        return (c->*func)(args...);
    }

    template<typename Class, typename ReturnType, typename...Args>
    dpp::task<ReturnType> apply(Class* c, dpp::task<ReturnType> (Class::*func)(Args...), Args... args) {
        co_return co_await (c->*func)(args...);
    }
}

#endif //DPP_STRUCTURES_FUNCTIONS_H
