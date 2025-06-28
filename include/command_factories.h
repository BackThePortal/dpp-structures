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
    using getter_setter_callback_t = std::function<dpp::task<void>(const dpp::slashcommand_t&, KeysEnum)>;

    template<class KeysEnum>
    subcommand_group getter_setter_command_group(const std::string& _name, dpp::command_option_type type, KeysEnum key,
                                                 const std::string& get_descr, const std::string& set_descr,
                                                 const getter_setter_callback_t<KeysEnum>& _get_callback,
                                                 const getter_setter_callback_t<KeysEnum>& _set_callback);
}
#endif//DPP_STRUCTURES_COMMAND_FACTORIES_H
