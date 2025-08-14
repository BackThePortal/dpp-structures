#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
//
// Created by backo on 22/06/25.
//

#ifndef DPP_STRUCTURES_DYNAMIC_VALUE_H
#define DPP_STRUCTURES_DYNAMIC_VALUE_H

#include <dpp/dpp.h>

namespace dpp_structures {

    /**
     * Use this type when a value can be either constant or dependant on an internal_json_file.
     *
     * @tparam ConfigKeys Enum of the keys you used for your configuration file in internal_json_file.
     * @tparam T Type of the value.
     */
    template<typename ConfigKeys, typename T>
    class dynamic_value {
    protected:
        union {
            T value;
            ConfigKeys config_key;
        };

    public:
        bool is_dynamic;

        dynamic_value(T value) : value(value) { this->is_dynamic = false; }

        dynamic_value(ConfigKeys config_key) : config_key(config_key) { this->is_dynamic = true; }

        /**
         * Override this function and use your internal_json_file instance to get the value from it if the object is
         * dynamic.
         * Make sure the config key actually points to a value of type T.
         */
        virtual T get() const = 0;

        operator T() const { return this->get(); }
    };


} // dpp_structures

#endif //DPP_STRUCTURES_DYNAMIC_VALUE_H

#pragma clang diagnostic pop