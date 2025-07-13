//
// Created by backo on 30/06/25.
//
#include "internal/exec.h"
#include <array>
#include <iomanip>
#include <memory>
#include <thread>
#include <dpp/utility.h>
#include <iostream>

void dpp_structures::internal::exec(const std::string& cmd, std::vector<std::string> parameters,
          std::function<void(int, const std::string&)> callback) {
    auto t = std::thread([cmd, parameters = std::move(parameters), callback = std::move(callback)]() {
        try {
            dpp::utility::set_thread_name("async_exec");
            std::array<char, 128> buffer;
            std::vector<std::string> my_parameters = parameters;
            std::string result;
            std::stringstream cmd_and_parameters;

            cmd_and_parameters << cmd;

            for (auto& parameter : my_parameters) { cmd_and_parameters << " " << parameter; }

            /* Capture stderr */
            cmd_and_parameters << " 2>&1";

            int exit_code = -1;

            std::string final_command = cmd_and_parameters.str();
            std::cout << "executing command: " << final_command << std::endl;
            std::unique_ptr<FILE, std::function<void(FILE*)>> pipe(
                    popen(final_command.c_str(), "r"),
                    [&exit_code](FILE* file) { exit_code = pclose(file); });

            if (!pipe) { return; }

            while (fgets(buffer.data(), (int) buffer.size(), pipe.get()) != nullptr) { result += buffer.data(); }

            if (callback) { callback(exit_code, result); }
        } catch (...) { /* There is nowhere to log this... */
        }
    });
    t.detach();
}