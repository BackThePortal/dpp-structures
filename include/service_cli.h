//
// Created by backo on 17/06/25.
//

#ifndef DPP_STRUCTURES_SERVICE_CLI_H
#define DPP_STRUCTURES_SERVICE_CLI_H

#include "internal/string_literal.h"
#include "feature.h"
#include <map>
#include <utility>

namespace dpp_structures {
    struct arguments {
        enum options_args_type {
            EQUALS = '=', SPACE = ' '
        };

        std::string main{};
        std::vector<std::string> flags{};
        std::map<std::string, std::string> options{};
        options_args_type args_type;

        [[nodiscard]] std::string join() const;

        [[nodiscard]] std::vector<std::string> list() const;
    };

    class service_cli {
    public:
        struct execution_output {
            std::string output;
            int exit_code;
        };

        template<typename ErrorsEnum> requires std::is_enum_v<ErrorsEnum>
        struct execution_error {
            ErrorsEnum error_type;
        };

        template<typename T, typename ErrorsEnum>
        struct execution_result : execution_output {
            std::variant<T, execution_error<ErrorsEnum>> result;

            explicit execution_result(const execution_output& output) :
                    execution_output(output), result(execution_error<ErrorsEnum>{ErrorsEnum::UNKNOWN_ERROR}) {
            }

            execution_result(const execution_output& output, T value) : execution_output(output),
                                                                        result(std::move(value)) {}

            execution_result(const execution_output& output, execution_error<ErrorsEnum> error) : execution_output(
                    output), result(std::move(error)) {}

            [[nodiscard]] bool is_error() const {
                return std::holds_alternative<execution_error<ErrorsEnum>>(this->result);
            }

            T get() const {
                return std::get<T>(this->result);
            }

            execution_error<ErrorsEnum> get_error() {
                return std::get<execution_error<ErrorsEnum>>(this->result);
            }
        };

    private:
        // dpp::utility::exec already does this
        //static constexpr std::string std_merge{"2>&1"};

        void exec(const arguments& args = {}, std::function<void(execution_output)> callback = {});

    public:
        const std::string executable_path;

        explicit service_cli(std::string executable_path);

        dpp::task<execution_output> run(arguments args = {});
    };
}// namespace services

#endif//DPP_STRUCTURES_SERVICE_CLI_H
