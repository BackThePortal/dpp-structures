//
// Created by backo on 6/7/2024.
//

#ifndef DPP_STRUCTURES_COMMAND_FACTORIES_H
#define DPP_STRUCTURES_COMMAND_FACTORIES_H

#include "command.h"
#include "file.h"
#include "utilities/text.h"

namespace dpp_structures {

    struct subcommand_group;

    template<typename KeysEnum>
    using getter_setter_callback_t = std::function<dpp::task<void>(dpp::slashcommand_t, KeysEnum)>;

    template<class KeysEnum>
    subcommand_group getter_setter_command_group(const std::string& _name, dpp::command_option_type type, KeysEnum key,
                                                 const std::string& get_descr, const std::string& set_descr,
                                                 const getter_setter_callback_t<KeysEnum>& get_callback,
                                                 const getter_setter_callback_t<KeysEnum>& set_callback) {
        /*
        : [_name](const dpp::slashcommand_t& event, KeysEnum& key) {
              event.reply("Value " + italic(_name) + " is " + (std::string) config(key) + ".");
          };
         */
        /*
        : [_name](const dpp::slashcommand_t& event, KeysEnum& key) {
              config.set(key, std::get<std::string>(event.get_parameter(_name)));
              event.reply("Value " + italic(_name) + " has been set to " + (std::string) config(key) + ".");
          };
    */
        return {_name,
                _name + " setter",
                {{"get",
                  get_descr,

                  [key, get_callback](dpp::slashcommand_t event) -> dpp::task<void> {
                      co_await get_callback(event, key);
                      co_return;
                  }, {},},
                 {"set", set_descr, [key, set_callback](dpp::slashcommand_t event) -> dpp::task<void> {
                     co_await set_callback(event, key);
                     co_return;
                 }, {{type, _name, _name, true}},}}};
    }
}
#endif//DPP_STRUCTURES_COMMAND_FACTORIES_H
