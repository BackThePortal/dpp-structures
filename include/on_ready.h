//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_ON_READY_H
#define DPP_STRUCTURES_ON_READY_H

#include <dpp/dpp.h>
#include "listener.h"


namespace dpp_structures {

    template<bool RunOnce = true>
    class on_ready : public NEW_LISTENER(ready_t, on_ready) {
    private:
        dpp::task<void> callback(const dpp::ready_t& event) override {
            if constexpr (RunOnce) {
                if (dpp::run_once<struct dummy>()) co_await this->start(event);
            } else co_await this->start();
            co_return;
        };

    protected:
        /**
         *
         * @see command_registerer
         */
        virtual dpp::task<void> start(dpp::ready_t t) = 0;
    };
}

#endif //DPP_STRUCTURES_ON_READY_H
