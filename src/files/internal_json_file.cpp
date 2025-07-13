//
// Created by backo on 4/8/2024.
//

#include "internal_json_file.h"

#include <utility>

bool dpp_structures::internal_file_key_info::is_same_type(dpp_structures::internal_file_key_info::value_type type,
                                                          const nlohmann::json& json) {
    std::cout << ((int)type) << ", " << json << " of type " << (json.type_name()) << '\n';

    switch (type) {
        case value_type::SNOWFLAKE:
            return json.type() == nlohmann::json::value_t::string ||
                   json.type() == nlohmann::json::value_t::number_unsigned;
        case value_type::STRING:
            return json.type() == nlohmann::json::value_t::string;
        case value_type::INT:
            return json.type() == nlohmann::json::value_t::number_integer ||
                   json.type() == nlohmann::detail::value_t::number_unsigned;
    }

    return false;
}


dpp_structures::internal_file_value::internal_file_value(std::string key,
                                                         dpp_structures::internal_file_key_info::value_type type,
                                                         dpp::snowflake snowflake)
        : internal_file_key_info(std::move(key), type), snowflake(snowflake) {}

dpp_structures::internal_file_value::internal_file_value(std::string key,
                                                         dpp_structures::internal_file_key_info::value_type type,
                                                         int _int)
        : internal_file_key_info(std::move(key), type), _int(_int) {}

dpp_structures::internal_file_value::internal_file_value(std::string key,
                                                         dpp_structures::internal_file_key_info::value_type type,
                                                         std::string* stringPtr)
        : internal_file_key_info(std::move(key), type), string(stringPtr) {}


dpp_structures::internal_file_value::~internal_file_value() {
    if (this->type == internal_file_key_info::value_type::STRING)
        delete this->string;
}


dpp_structures::internal_file_value dpp_structures::internal_file_value::from_json(const nlohmann::json& json, const std::string& key,
                                                                   dpp_structures::internal_file_key_info::value_type type) {

    ASSERT_TYPE(is_same_type(type, json))
    std::cout << "assert pass\n";

    switch (type) {
        case internal_file_key_info::value_type::SNOWFLAKE: {
            dpp::snowflake snowflake;
            if (json.type() == nlohmann::json::value_t::string) snowflake = json.get<std::string>();
            else if (json.type() == nlohmann::json::value_t::number_unsigned)
                snowflake = json.get<unsigned int>();
            else
                throw internal_file_wrong_type();

            return {key, type, snowflake};
        }
        case internal_file_key_info::value_type::STRING:
            if (json.type() != nlohmann::detail::value_t::string) throw internal_file_wrong_type();

            return {key, type, new std::string(json.get<std::string>())};
        case internal_file_key_info::value_type::INT:
            if (json.type() != nlohmann::detail::value_t::number_integer &&
                json.type() != nlohmann::detail::value_t::number_unsigned)
                throw internal_file_wrong_type();

            return {key, type, json.get<int>()};
        default:
            throw internal_file_wrong_type();
    }
};
