//
// Created by backo on 6/7/2024.
//

#include "command_factories.h"
#include "command.h"
#include "utilities/text.h"

using namespace dpp_structures;

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
              {},
              [key, get_callback](const dpp::slashcommand_t& event) { get_callback(event, key); }},
             {"set", set_descr, {{type, _name, _name, true}}, [key, set_callback](const dpp::slashcommand_t& event) {
                 set_callback(event, key);
             }}}};
}
