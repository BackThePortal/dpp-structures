//
// Created by backo on 28/3/2024.
//

#ifndef DPP_STRUCTURES_COMMAND_H
#define DPP_STRUCTURES_COMMAND_H

#include <dpp/dpp.h>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

#include "internal/string_literal.h"
#include "internal/meta.h"
#include "feature.h"
/*
// forward declaration
namespace bot_errors {
    void not_implemented_error(const dpp::slashcommand_t& event);
}
*/

namespace dpp_structures {

    class subcommand_group;

    typedef std::function<dpp::task<void>(const dpp::slashcommand_t&)> callback_t;


    namespace next {

        template<internal::string_literal Name, internal::string_literal Description>
        class subcommand {
            virtual dpp::task<void> callback(dpp::slashcommand_t event) = 0;
        };

        template<internal::string_literal Name, internal::string_literal Description, typename Derived>
        class command : feature {

        protected:

        public:
            using subcommand_t = dpp::task<void> (Derived::*)(dpp::slashcommand_t);

            static consteval subcommand_t get_subcommand(std::size_t n) {
                return Derived::subcommands[n];
            }


            virtual dpp::task<void> callback(dpp::slashcommand_t event) = 0;
        };


        class test_command : command<"test", "Some description", test_command> {

            class something_subcommand : subcommand<"something", "Another description"> {
                dpp::task<void> callback(dpp::slashcommand_t event) override {
                    co_return;
                }
            };

            dpp::task<void> something(dpp::slashcommand_t event) {

            }
            dpp::task<void> other(dpp::slashcommand_t event) {

            }
        public:
            static constexpr subcommand_t subcommands[] = {&test_command::something, &test_command::other};
        };
    }


    class command;

    class subcommand final : protected dpp::command_option {
    public:
        subcommand() = default;

        // TODO: warn if default callback is used
        subcommand(const std::string& name, const std::string& description, callback_t callback,
                   const std::vector<command_option>& options);

        using command_option::description;
        using command_option::name;
        using command_option::options;

        callback_t callback;

        // allow conversion to command_option inside command
        friend command;
        friend subcommand_group;
    };

    class subcommand_group final : protected dpp::command_option {
    public:
        subcommand_group() = default;

        subcommand_group(const std::string& name, const std::string& description,
                         const std::vector<subcommand>& subcommands);

        using command_option::description;
        using command_option::name;

        std::map<std::string, subcommand> subcommands_map{};

        // allow conversion to command_option inside command
        friend command;
        friend subcommand;
    };

    class command : public dpp::slashcommand {
        friend std::ostream& operator<<(std::ostream& os, const command& obj) {
            return os << obj.name << " subcommands_map: " << obj.subcommands_map.size()
                      << " subcommand_groups_map: " << obj.subcommand_groups_map.size() << '\n';
        }

    public:
        //explicit command::command(dpp::cluster& bot);
        std::map<std::string, subcommand> subcommands_map{};
        std::map<std::string, subcommand_group> subcommand_groups_map{};

        command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                dpp::permission permissions = dpp::p_use_application_commands);

        command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                const std::vector<subcommand_group>& subcommand_groups, const std::vector<subcommand>& subcommands,
                dpp::permission permissions = dpp::p_use_application_commands);

        command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                const std::vector<dpp::command_option>& options,
                dpp::permission permissions = dpp::p_use_application_commands);

        //using slashcommand::slashcommand;
        using slashcommand::description;
        using slashcommand::name;
        using slashcommand::options;


        // The base command has no action if there are subcommands or subcommand groups.
        virtual dpp::task<void> callback(dpp::slashcommand_t event) = 0;
    };


}
#endif//DPP_STRUCTURES_COMMAND_H
