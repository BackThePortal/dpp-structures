//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_INTERNAL_FILE_H
#define DPP_STRUCTURES_INTERNAL_FILE_H

#include "internal/concepts.h"
#include "internal/enums.h"
#include "file.h"

#include <array>
#include <string>
#include <type_traits>
#include <utility>

namespace dpp_structures {

    struct internal_file_key_info {
        enum class value_type {
            SNOWFLAKE, STRING, INT
        };
        std::string key;
        value_type type;
    };

    struct internal_file_value : internal_file_key_info {
        union {
            dpp::snowflake snowflake;
            int _int;
            std::string* string;
        };

        internal_file_value(std::string key, value_type type, dpp::snowflake snowflake)
                : internal_file_key_info(std::move(key), type), snowflake(snowflake) {}

        internal_file_value(std::string key, value_type type, int _int)
                : internal_file_key_info(std::move(key), type), _int(_int) {}

        internal_file_value(std::string key, value_type type, std::string* stringPtr)
                : internal_file_key_info(std::move(key), type), string(stringPtr) {}

        virtual ~internal_file_value();

        inline operator std::string&() const {// NOLINT(*-explicit-constructor)
            assert(this->type == internal_file_key_info::value_type::STRING);
            return *this->string;
        }

        inline operator int() const {// NOLINT(*-explicit-constructor)
            assert(this->type == internal_file_key_info::value_type::INT);
            return this->_int;
        }

        inline operator dpp::snowflake() const {// NOLINT(*-explicit-constructor)
            assert(this->type == internal_file_key_info::value_type::SNOWFLAKE);
            return this->snowflake;
        }
    };

    class internal_file_wrong_type : std::exception {
    };

    /**
     * Class to edit an internal JSON file with fixed keys and their types. Useful for easy editing of configuration files
     * and getting tokens.
     *
     *
     * If constexpr maps and strings were a thing, you'd pass some kind of JSON schema and it'd be all type safe,
     * but we don't live in such universe.
     * @tparam FileClass Restrict usage of this file to read or write.
     * @tparam KeysEnum Keys of the JSON file.
     */
    template<typename FileClass, typename KeysEnum> requires std::is_enum_v<KeysEnum> &&
                                                             std::is_integral_v<typename std::underlying_type<KeysEnum>::type> &&
                                                             std::is_base_of_v<json_file_base, FileClass>
    class internal_json_file : FileClass {
    private:
        internal_file_key_info get_key_info(KeysEnum key) { return this->keys.at(to_underlying(key)); }

#define SET_OVERLOAD(_name, _type)                                                                                     \
    void _set(nlohmann::json& json, const internal_file_key_info& key_info, _type value) {                             \
        if (key_info.type != internal_file_key_info::value_type::_name) throw internal_file_wrong_type();              \
        json = value;                                                                                                  \
    }

        SET_OVERLOAD(STRING, std::string)

        SET_OVERLOAD(SNOWFLAKE, dpp::snowflake)

        SET_OVERLOAD(INT, int)


    public:
        using KeysArray = std::array<internal_file_key_info, to_underlying(KeysEnum::KEYS_LENGTH)>;

        const KeysArray& keys;

        /**
         *
         * @param path Path of the file relative to the executable.
         * @param keys An array that describes the JSON's keys in exactly the same order as KeysEnum.
         */
        explicit internal_json_file(const std::string& path, const KeysArray& keys) : FileClass(path), keys(keys) {}

        internal_file_value get(KeysEnum key) {
            static_assert(FileClass::actions & READ);

            internal_file_key_info key_info = this->get_key_info(key);

            nlohmann::json obj = this->get_obj(key_info.key);

            switch (key_info.type) {
                case internal_file_key_info::value_type::SNOWFLAKE:
                    return {key_info.key, key_info.type, dpp::snowflake(obj.get<std::string>())};
                case internal_file_key_info::value_type::STRING:
                    return {key_info.key, key_info.type, new std::string(obj.get<std::string>())};
                case internal_file_key_info::value_type::INT:
                    return {key_info.key, key_info.type, obj.get<int>()};
                default:
                    assert(false);
            }
        }

        internal_file_value operator()(KeysEnum key) { return this->get(key); }

        template<typename T>
        internal_file_value set(KeysEnum key, T value) {
            static_assert(FileClass::actions & (READ | WRITE));

            internal_file_key_info key_info = this->get_key_info(key);

            nlohmann::json obj = this->get_obj(key_info.key);

            this->_set(obj, key_info, value);

            return this->get(key);
        }
    };
}
#endif//DPP_STRUCTURES_INTERNAL_FILE_H
