//
// Created by backo on 12/06/25.
//

#ifndef DPP_STRUCTURES_LISTENER_H
#define DPP_STRUCTURES_LISTENER_H

#include "internal/meta.h"
#include "feature.h"

#define NEW_LISTENER(event_type, router_name)                                                                          \
    listener<dpp::event_type,                                                                                          \
             [](dpp::cluster& bot) -> dpp::event_router_t<dpp::event_type>& { return bot.router_name; }>


namespace dpp_structures {
    using namespace internal;

    template<typename T>
    using EventRouter = dpp::event_router_t<T>& (*)(dpp::cluster&);

    /**
     * Abstract class to create listeners.
     *
     * The listener will be automatically attached for all instances.
     *
     * @tparam T Type of the D++ event.
     * @tparam Trigger Function that returns the dpp::event_router_t object from the bot instance.
     * @see on_ready
     */
    template<derived<dpp::event_dispatch_t> T, EventRouter<T> Trigger>
    class listener : feature {
    private:
        void setup() override {
            Trigger(*this->bot)([this](const T& event) -> dpp::task<void> {
                if (this->filter(event)) co_await this->callback(event);
                co_return;
            });
        };

    protected:
        using feature::bot;

        virtual bool filter(const T& event) { return true; }

        virtual dpp::task<void> callback(const T&) = 0;
    };

}// namespace listeners

/*
template<derived<dpp::event_dispatch_t> T>
listener(const std::function<dpp::event_router_t<T> &(dpp::cluster &)> &) -> listener<T>;
*/


#endif//DPP_STRUCTURES_LISTENER_H
