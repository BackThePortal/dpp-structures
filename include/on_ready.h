//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_ON_READY_H
#define DPP_STRUCTURES_ON_READY_H

#include <dpp/dpp.h>
#include "listener.h"


namespace dpp_structures {


    class on_ready : public listener<[](dpp::cluster& bot) { return &bot.on_ready; }> {
    private:
        dpp::task<void> callback(const dpp::ready_t& event) override {
            if (!this->run_once || dpp::run_once<struct dummy>()) co_await this->start(event);
            co_return;
        };
    public:
        explicit on_ready(bool runOnce);

    protected:
        const bool run_once;
        /**
         *
         * @see command_registerer
         */
        virtual dpp::task<void> start(dpp::ready_t) = 0;
    };
}

#endif //DPP_STRUCTURES_ON_READY_H
