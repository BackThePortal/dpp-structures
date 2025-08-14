//
// Created by backo on 12/06/25.
//

#ifndef DPP_STRUCTURES_LISTENER_H
#define DPP_STRUCTURES_LISTENER_H

#include "internal/meta.h"
#include "feature.h"



namespace dpp_structures {
    using namespace internal;


    /**
     * Abstract class to create listeners. The listener will be automatically attached for all instances.
     *
     * @tparam EventRouter A pointer to a member field of dpp::cluster of type dpp::event_router_t.
     */
    template<auto EventRouter> requires std::is_member_pointer_v<decltype(EventRouter)> &&
                                        internal::is_event_router<internal::member_type_t<decltype(EventRouter)>>::value
    class listener : feature {
    public:
        /// @example on_slash_command, on_message_create, etc.
        using event_router = internal::member_type_t<decltype(EventRouter)>;
        /// @example dpp::slashcommand_t, dpp::message_create_t, etc.
        using event_type = internal::event_router_traits<event_router>::event_type;

    private:
        event_router& get_router() {
            return this->bot->*EventRouter;
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
