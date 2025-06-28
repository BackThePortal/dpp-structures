//
// Created by backo on 28/05/25.
//

#ifndef DPP_STRUCTURES_ENUMS_H
#define DPP_STRUCTURES_ENUMS_H

#include <utility>

namespace dpp_structures::internal {
    template<typename E>
    constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {

        return static_cast<typename std::underlying_type<E>::type>(e);
    }
}
#endif//DPP_STRUCTURES_ENUMS_H
