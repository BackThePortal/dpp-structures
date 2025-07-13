//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_INTERNAL_FILE_H
#define DPP_STRUCTURES_INTERNAL_FILE_H

#include "internal/meta.h"
#include "internal/enums.h"
#include "file.h"

#include <array>
#include <string>
#include <type_traits>
#include <utility>

#define ASSERT_TYPE(expr) if(! expr) throw internal_file_wrong_type();

namespace dpp_structures {
    class internal_file_wrong_type : std::exception {
    };

    class internal_file_not_found : std::exception {
    };


    struct internal_file_key_info {
        enum class value_type {
            SNOWFLAKE, STRING, INT
        };
        std::string key;
        value_type type;

        template<typename T>
        [[nodiscard]] static inline bool is_same_type([[maybe_unused]] value_type type) { return false; };


        [[nodiscard]] static bool is_same_type(value_type type, const nlohmann::json& json);

        [[nodiscard]] inline bool is_same_type(const nlohmann::json& json) {
            return is_same_type(this->type, json);
        }

        template<typename T>
        [[nodiscard]] bool is_same_type() const { return is_same_type<T>(this->type); }


    };

    template<>
    inline bool internal_file_key_info::is_same_type<std::string>(value_type type) {
        return type == value_type::STRING;
    }

    template<>
    inline bool internal_file_key_info::is_same_type<dpp::snowflake>(value_type type) {
        return type == value_type::SNOWFLAKE;
    }

    template<>
    inline bool internal_file_key_info::is_same_type<long long>(value_type type) {
        return type == value_type::INT;
    }


    struct internal_file_value : internal_file_key_info {


        union {
            dpp::snowflake snowflake;
            int _int;
            std::string* string;
        };

        internal_file_value(std::string key, value_type type, dpp::snowflake snowflake);

        internal_file_value(std::string key, value_type type, int _int);

        internal_file_value(std::string key, value_type type, std::string* stringPtr);

        static internal_file_value from_json(const nlohmann::json& json, const std::string& key, value_type type);

        inline operator std::string() const {// NOLINT(*-explicit-constructor)
            ASSERT_TYPE(this->is_same_type<std::string>());
            return *this->string;
        }

        inline operator int() const {// NOLINT(*-explicit-constructor)
            ASSERT_TYPE(this->is_same_type<int>());
            return this->_int;
        }

        inline operator dpp::snowflake() const {// NOLINT(*-explicit-constructor)
            ASSERT_TYPE(this->is_same_type<dpp::snowflake>());
            return this->snowflake;
        }

        virtual ~internal_file_value();
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
                                                             std::is_same_v<typename std::underlying_type<KeysEnum>::type, std::size_t> &&
                                                             std::is_base_of_v<json_file_base, FileClass>
    class internal_json_file : FileClass {
    private:
        internal_file_key_info get_key_info(KeysEnum key) { return this->keys.at(internal::to_underlying(key)); }

    public:
        using keys_array = std::array<internal_file_key_info, internal::to_underlying(KeysEnum::KEYS_LENGTH)>;

        keys_array keys;

        /**
         *
         * @param path Path of the file relative to the executable.
         * @param keys An array that describes the JSON's keys in exactly the same order as KeysEnum.
         */
        explicit internal_json_file(std::string path, const std::array<internal_file_key_info, internal::to_underlying(
                KeysEnum::KEYS_LENGTH)>& keys) : FileClass(path), keys(keys) {}

        [[nodiscard]] internal_file_value get(KeysEnum key) {
            static_assert(FileClass::actions & READ);

            internal_file_key_info key_info = this->get_key_info(key);

            std::cout << "key: " << key_info.key << '\n';


            std::optional<nlohmann::json> obj = this->get_obj(key_info.key);

            if (obj == std::nullopt) throw internal_file_not_found();

            return internal_file_value::from_json(obj.value(), key_info.key, key_info.type);
        }

        internal_file_value operator()(KeysEnum key) { return this->get(key); }

        template<typename T>
        internal_file_value set(KeysEnum key, T value) {
            static_assert(FileClass::actions & (READ | WRITE));

            internal_file_key_info key_info = this->get_key_info(key);

            ASSERT_TYPE(key_info.is_same_type<T>())

            std::optional<nlohmann::json> obj = this->get_obj(key_info.key);

            FileClass::set(key_info.key, value);

            return this->get(key);
        }
    };
}// namespace dpp_structures
#endif//DPP_STRUCTURES_INTERNAL_FILE_H
