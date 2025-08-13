//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_ON_SLASH_COMMAND_H
#define DPP_STRUCTURES_ON_SLASH_COMMAND_H

#include "listener.h"

namespace dpp_structures {
    using on_slashcommand = listener<[](dpp::cluster& bot) { return &bot.on_slashcommand; }>;
}
#endif //DPP_STRUCTURES_ON_SLASH_COMMAND_H
