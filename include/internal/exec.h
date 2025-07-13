//
// Created by backo on 19/06/25.
//

#ifndef DPP_STRUCTURES_EXEC_H
#define DPP_STRUCTURES_EXEC_H

#include <string>
#include <vector>
#include <functional>

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

namespace dpp_structures::internal {
    void exec(const std::string& cmd, std::vector<std::string> parameters,
              std::function<void(int, const std::string&)> callback);
}// namespace dpp

#endif//DPP_STRUCTURES_EXEC_H
