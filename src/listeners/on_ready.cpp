//
// Created by backo on 25/06/25.
//

#include "on_ready.h"

dpp_structures::on_ready::on_ready(bool runOnce) : run_once(runOnce) {}

dpp::task<void> dpp_structures::on_ready::callback(const dpp::ready_t& event) {
    if (!this->run_once || dpp::run_once<struct _>()) co_await this->start(event);
    co_return;
}
