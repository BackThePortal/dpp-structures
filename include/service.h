//
// Created by backo on 17/06/25.
//

#ifndef DPP_STRUCTURES_SERVICE_H
#define DPP_STRUCTURES_SERVICE_H

#include "internal/string_literal.h"
#include "internal/exec.h"
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

    class service : feature {
    public:
        struct execution_result {
            std::string output;
            int exit_code;
        };

    private:
        // dpp::utility::exec already does this
        //static constexpr std::string std_merge{"2>&1"};

        void exec(const arguments& args = {}, std::function<void(execution_result)> callback = {});

    public:
        const std::string executable_path;

        explicit service(std::string executable_path);

        dpp::task<execution_result> run(arguments args = {});
    };
}// namespace services

#endif//DPP_STRUCTURES_SERVICE_H
