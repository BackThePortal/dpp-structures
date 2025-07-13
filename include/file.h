//
// Created by backo on 3/7/2024.
//

#ifndef DPP_STRUCTURES_FILE_H
#define DPP_STRUCTURES_FILE_H

#include <dpp/dpp.h>

namespace dpp_structures {

    enum file_actions {
        READ = 1 << 0, WRITE = 1 << 1
    };


    class json_file_base {
    protected:
        std::fstream file;
        nlohmann::json json;

    public:
        std::string path;

        explicit json_file_base(std::string path);

        // Empty json.
        void forget();

        json_file_base() = default;
    };


    class i_json_file : public json_file_base {
    protected:
        void read();

    public:
        static constexpr int actions = READ;

        explicit i_json_file(const std::string& path);

        i_json_file() = default;

        std::optional<nlohmann::json> operator()(const std::string& key, const std::optional<std::string>& _default = std::nullopt);

        [[nodiscard]] std::optional<std::string> get_string(const std::string& key, const std::optional<std::string>& _default = std::nullopt);

        [[nodiscard]] std::optional<nlohmann::json> get_obj(const std::string& key, const std::optional<std::string>& _default = std::nullopt);
    };


    class io_json_file : public i_json_file {
    protected:
        void save();

    public:
        static constexpr int actions = READ | WRITE;

        explicit io_json_file(const std::string& path);

        template<typename T>
        void set(const std::string& key, const T& value) {
            this->read();
            this->json[key] = value;
            this->save();
            this->forget();
        }
    };
}
// extern io_file config;
// extern i_file env;


#endif//DPP_STRUCTURES_FILE_H
