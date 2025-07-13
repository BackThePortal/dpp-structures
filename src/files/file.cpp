//
// Created by backo on 3/7/2024.
//

#include "file.h"
#include <utility>

using namespace dpp_structures;

/*
 * If a file is opened in a function, it must be closed at the end of the same function.
 */

json_file_base::json_file_base(std::string path) : path(std::move(path)) {}

void json_file_base::forget() { this->json = {}; }


void i_json_file::read() {
    this->file.open(this->path, std::ios_base::in);
    this->json = nlohmann::json::parse(this->file);
    this->file.close();
}

std::optional<nlohmann::json> i_json_file::operator()(const std::string& key, const std::optional<std::string>& _default) {
    return this->get_obj(key, _default);
}

std::optional<std::string> i_json_file::get_string(const std::string& key, const std::optional<std::string>& _default) {
    return this->get_obj(key, _default);
}

std::optional<nlohmann::json> i_json_file::get_obj(const std::string& key, const std::optional<std::string>& _default) {
    this->read();


    std::optional<nlohmann::json> ret = std::nullopt;
    if (this->json.contains(key) || _default != std::nullopt) {
        ret = this->json.value(key, _default.value_or(""));
    }

    this->forget();
    return ret;
}

i_json_file::i_json_file(const std::string& path) : json_file_base(path) {}

io_json_file::io_json_file(const std::string& path) : i_json_file(path) {}

void io_json_file::save() {
    this->file.open(this->path, std::ios_base::out);
    std::cout << "Saving json\n";
    const auto string = std::string(this->json.dump(2, ' ', false, nlohmann::detail::error_handler_t::replace));
    std::cout << string << '\n';

    this->file << string;
    this->file.close();
}


//io_file config("config.json");
//i_file env("env.json");
