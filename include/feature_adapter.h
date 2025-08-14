//
// Created by backo on 06/08/25.
//

#ifndef SCARECROW_FEATURE_ADAPTER_H
#define SCARECROW_FEATURE_ADAPTER_H


#include "feature.h"
#include "internal/meta.h"

namespace dpp_structures {

    template<typename T>
    concept FeatureSubclass = std::is_base_of_v<feature, T>;

    template<typename Feature> requires FeatureSubclass<Feature> || std::is_same_v<Feature, void>
    dpp::task<void> pre_call_feature(dpp::slashcommand_t event, bool ephemeralThink) {
        co_await event.co_thinking(ephemeralThink);

        co_return;
    };


    /**
     *
     * @tparam Function A pointer to member function.
     * @tparam EphemeralThink Whether the 'thinking' message should be shown only to the user that triggered the command.
     * @param event
     * @param args Arguments passed to the member function, apart from the event.
     * @return
     */
    template<auto Function, bool EphemeralThink = false, typename... Args>
    requires std::is_member_function_pointer_v<decltype(Function)> &&
             // Member function pointer is callable using feature instance, event and args passed
             (std::is_invocable_v<decltype(Function),
                     typename internal::callable_info<decltype(Function)>::class_type,
                     dpp::slashcommand_t, Args...>) &&
             // The class pointed by the member function pointer is a subclass of feature.
             FeatureSubclass<typename internal::callable_info<decltype(Function)>::class_type>
    dpp::task<void> call_feature(dpp::slashcommand_t event, Args... args) {
        using class_type = typename internal::callable_info<decltype(Function)>::class_type;

        co_await dpp_structures::pre_call_feature<class_type>(event, EphemeralThink);


        co_await std::invoke(Function, class_type::get_instance(), event, args...);

        co_return;
    }


}// namespace dpp_structures

#endif//SCARECROW_FEATURE_ADAPTER_H
