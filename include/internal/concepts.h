//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_CONCEPTS_H
#define DPP_STRUCTURES_CONCEPTS_H

#include <concepts>

namespace dpp_structures::internal {
    template<class T, class U>
    concept derived = std::is_base_of<U, T>::value;
}
#endif//DPP_STRUCTURES_CONCEPTS_H
