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

	template<auto Function, bool EphemeralThink = false, typename... Args>
		requires std::is_member_function_pointer_v<decltype(Function)> &&
				 (std::is_invocable_v<decltype(Function),
									  typename internal::member_function_pointer_info<decltype(Function)>::class_type,
									  dpp::slashcommand_t, Args...>) &&
				 FeatureSubclass<typename internal::member_function_pointer_info<decltype(Function)>::class_type>
	dpp::task<void> call_feature(dpp::slashcommand_t event, Args... args) {
		co_await event.co_thinking(EphemeralThink);


		using class_type = typename internal::member_function_pointer_info<decltype(Function)>::class_type;
		co_await std::invoke(Function, class_type::get_instance(), event, args...);
	}


}// namespace dpp_structures

#endif//SCARECROW_FEATURE_ADAPTER_H
