//
// Created by backo on 28/3/2024.
//

#include "command.h"
//#include "errors/code_errors.h"

using namespace dpp_structures;

subcommand::subcommand(const std::string& name, const std::string& description, callback_t callback,
                       const std::vector<command_option>& options = {})
	: command_option(dpp::co_sub_command, name, description), callback(std::move(callback)) {
	this->options = options;
}

subcommand_group::subcommand_group(const std::string& name, const std::string& description,
								   const std::vector<subcommand>& subcommands)
	: command_option(dpp::co_sub_command_group, name, description) {
	for (auto& subcommand: subcommands) {
		this->options.push_back(subcommand);
		this->subcommands_map[subcommand.name] = subcommand;
	}
}


command::command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                 dpp::permission permissions)
	: slashcommand(std::string(name), description, application_id) {
    this->set_default_permissions(permissions);
    this->default_member_permissions = permissions;
}

command::command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                 const std::vector<subcommand_group>& subcommand_groups, const std::vector<subcommand>& subcommands,
                 dpp::permission permissions)
	: command(std::string(name), description, application_id, permissions) {
	for (auto& subcommand: subcommands) {
		this->options.push_back(subcommand);
		this->subcommands_map[subcommand.name] = subcommand;
	}

	for (const auto& group: subcommand_groups) {
		this->options.push_back(group);
		this->subcommand_groups_map[group.name] = group;
	}
}

command::command(const std::string_view& name, const std::string& description, const dpp::snowflake& application_id,
                 const std::vector<dpp::command_option>& options, dpp::permission permissions)
	: command(std::string(name), description, application_id, permissions) {
	this->options = options;
}

/*
command::command(dpp::cluster& bot) : dpp::slashcommand(std::string(this->name()), std::string(command::description),
                                                        bot.me.id) {

}
*/
