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

/*
// forward declaration
namespace bot_errors {
    void not_implemented_error(const dpp::slashcommand_t& event);
}
*/

namespace dpp_structures {

    class subcommand_group;

    typedef std::function<void(const dpp::slashcommand_t&)> callback_t;


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

        command(const std::string_view& _name, const std::string& _description, const dpp::snowflake& _application_id);

        command(const std::string_view& _name, const std::string& _description, const dpp::snowflake& _application_id,
                const std::vector<subcommand_group>& subcommand_groups, const std::vector<subcommand>& subcommands);

        command(const std::string_view& _name, const std::string& _description, const dpp::snowflake& _application_id,
                const std::vector<dpp::command_option>& options);

        //using slashcommand::slashcommand;
        using slashcommand::description;
        using slashcommand::name;
        using slashcommand::options;


        // The base command has no action if there are subcommands or subcommand groups.
        virtual dpp::task<void> callback(const dpp::slashcommand_t& event) = 0;
    };

    class commands_utils {
    public:

    };

}
#endif//DPP_STRUCTURES_COMMAND_H
