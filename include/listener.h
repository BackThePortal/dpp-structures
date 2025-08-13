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
/**
 * Abstract class to create listeners.
 *
 * The listener will be automatically attached for all instances.
 *
 * @tparam Trigger Function that returns a pointer to the dpp::event_router_t object from the bot instance.
 */
template<auto Trigger> requires std::is_invocable_v<decltype(Trigger), dpp::cluster&> &&
                                internal::is_event_router<std::remove_pointer_t<std::invoke_result_t<decltype(Trigger), dpp::cluster&>>>::value
class listener : feature {
public:
    using event_router = std::remove_pointer_t<std::invoke_result_t<decltype(Trigger), dpp::cluster&>>;
    using event_type = internal::event_router_traits<event_router>::event_type;

private:
    event_router& get_router() {
        return *Trigger(*this->bot);
    }

    void attach_event() {
        this->get_router()([this](const event_type& event) -> dpp::task<void> {
            if (this->filter(event)) co_await this->callback(event);
            co_return;
        });
    }

    void setup() override {
        this->attach_event();
    }

public:
    using feature::bot;

    virtual bool filter(const event_type&) { return true; }

    virtual dpp::task<void> callback(const event_type&) = 0;

};

}// namespace listeners

/*
template<derived<dpp::event_dispatch_t> T>
listener(const std::function<dpp::event_router_t<T> &(dpp::cluster &)> &) -> listener<T>;
*/


#endif//DPP_STRUCTURES_LISTENER_H
