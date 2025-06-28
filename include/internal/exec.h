//
// Created by backo on 19/06/25.
//

#ifndef DPP_STRUCTURES_EXEC_H
#define DPP_STRUCTURES_EXEC_H

#include <array>
#include <dpp/dpp.h>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

/**
 * The following function is a modified version from the original
 * taken from the D++ library.
 * It has been modified to be able to return the exit code.
 *
 * D++, A Lightweight C++ library for Discord
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright 2021 Craig Edwards and D++ contributors
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

namespace dpp::utility {
    void exec(const std::string& cmd, std::vector<std::string> parameters,
              std::function<void(int, const std::string&)> callback) {
        auto t = std::thread([cmd, parameters = std::move(parameters), callback = std::move(callback)]() {
            try {
                dpp::utility::set_thread_name("async_exec");
                std::array<char, 128> buffer;
                std::vector<std::string> my_parameters = parameters;
                std::string result;
                std::stringstream cmd_and_parameters;

                cmd_and_parameters << cmd;

                for (auto& parameter : my_parameters) { cmd_and_parameters << " " << std::quoted(parameter); }

                /* Capture stderr */
                cmd_and_parameters << " 2>&1";

                int exit_code = -1;

                std::unique_ptr<FILE, std::function<void(FILE*)>> pipe(
                        popen(cmd_and_parameters.str().c_str(), "r"),
                        [&exit_code](FILE* file) { exit_code = pclose(file); });

                if (!pipe) { return; }

                while (fgets(buffer.data(), (int) buffer.size(), pipe.get()) != nullptr) { result += buffer.data(); }

                if (callback) { callback(exit_code, result); }
            } catch (...) { /* There is nowhere to log this... */
            }
        });
        t.detach();
    }
}// namespace dpp

#endif//DPP_STRUCTURES_EXEC_H
