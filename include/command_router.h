//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_COMMAND_ROUTER_H
#define DPP_STRUCTURES_COMMAND_ROUTER_H

#include <dpp/dpp.h>
#include <vector>
#include "command.h"
#include "internal/meta.h"
#include "listener.h"
#include "utilities/singleton.h"

namespace dpp_structures {
    template<typename T>
    concept DerivedFromCommand = std::derived_from<T, command>;

    template<DerivedFromCommand... Commands> // TODO: require constructor(bot)
    class command_router : listener<&dpp::cluster::on_slashcommand>,
                           public singleton<command_router<Commands...>> {
    public:
        std::unordered_map<std::string, command*> command_map;

    private:
        command_router() = default;

        template<DerivedFromCommand Command, DerivedFromCommand... Rest>
        std::vector<dpp::slashcommand*> register_one() {
            Command* command_ptr = new Command(*this->bot); // This isn't deleted, it stays in command_map

            command_map[command_ptr->name] = command_ptr;

            std::vector<dpp::slashcommand*> commands_vector{dynamic_cast<dpp::slashcommand*>(command_ptr)};


            if constexpr (sizeof...(Rest) != 0) {
                for (auto& rest_command_ptr : this->register_one<Rest...>()) {
                    commands_vector.push_back(rest_command_ptr);
                }
            }

            return commands_vector;
        }

        FRIEND_SINGLETON(command_router<Commands...>)


    public:
        dpp::task<void> callback(const dpp::slashcommand_t& event) {
            co_await this->route_command(event);
        }

        std::vector<dpp::slashcommand> register_all() {
            std::vector<dpp::slashcommand> v;

            for (auto& command_ptr : this->register_one<Commands...>()) {
                v.push_back(*command_ptr);
            }

            return v;
        }

        dpp::task<void> route_command(const dpp::slashcommand_t& event) {
            auto command_interaction = event.command.get_command_interaction();

            std::cout << event.command.get_command_name() << std::endl;// outputs "rules"


            auto ref = command_map.find(event.command.get_command_name());
            if (ref != command_map.end()) {
                auto& command = *ref->second;
                std::cout << "command found" << std::endl;
                std::cout << command.name << std::endl;

                auto command_data_options = event.command.get_command_interaction().options;
                std::cout << "options " << command_data_options.size() << '\n';
                for (const auto& opt : command_data_options) {
                    std::cout << '\t' << opt.name << " - " << (int) opt.type << '\n';
                }

                const auto& command_path =
                        get_subcommand_path_types(event.command.get_command_interaction()).second;
                std::cout << "command path" << '\n';
                for (const auto& pair : command_path) { std::cout << "\t" << pair.first << std::endl; }

                if (command_path.empty()) {
                    std::cout << "command is single" << std::endl;
                    co_await command.callback(event);
                    co_return;
                }

                const subcommand_group* group = nullptr;
                for (const auto& [name, type] : command_path) {
                    if (type == dpp::co_sub_command_group) {
                        std::cout << "subcommand group is " << name << std::endl;

                        auto it = command.subcommand_groups_map.find(name);
                        if (it != command.subcommand_groups_map.end()) {
                            group = &it->second;
                        } else
                            std::cout << "subcommand group not found" << std::endl;
                    } else if (type == dpp::co_sub_command) {
                        std::cout << "subcommand is " << name << std::endl;

                        const auto& map = group == nullptr ? command.subcommands_map : group->subcommands_map;

                        auto it = map.find(name);
                        if (it != map.end()) {
                            const auto& subcommand = it->second;
                            co_await subcommand.callback(event);
                            co_return;
                        } else
                            std::cout << "subcommand not found" << std::endl;
                    } else
                        break;
                }
            }
            event.from()->creator->log(dpp::ll_error, "Unable to route command: " + event.command.get_command_name());
            co_return;
        };

        static std::vector<std::string> get_subcommand_path(const dpp::command_interaction& interaction) {
            std::vector v = {interaction.name};

            for (const auto& option : interaction.options) {
                if (option.type == dpp::co_sub_command || option.type == dpp::co_sub_command_group) {
                    v.push_back(option.name);
                } else
                    break;
            }

            return v;
        };

        static std::pair<std::string, std::list<std::pair<std::string, dpp::command_option_type>>>
        get_subcommand_path_types(const dpp::command_interaction& interaction) {
            std::list<std::pair<std::string, dpp::command_option_type>> list;

            if (!interaction.options.empty()) {
                auto command_data_option = interaction.options[0];
                switch (command_data_option.type) {
                    case dpp::co_sub_command_group:
                        list.emplace_back(command_data_option.name, dpp::co_sub_command_group);
                        list.emplace_back(command_data_option.options[0].name, dpp::co_sub_command);
                        break;
                    case dpp::co_sub_command:
                        list.emplace_back(command_data_option.name, dpp::co_sub_command);
                        break;
                    default:
                        break;
                }
            }

            return {interaction.name, list};
        };

    };

}
#endif //DPP_STRUCTURES_COMMAND_ROUTER_H
