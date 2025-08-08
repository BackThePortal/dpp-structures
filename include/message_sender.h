//
// Created by backo on 25/06/25.
//

#ifndef DPP_STRUCTURES_MESSAGE_SENDER_H
#define DPP_STRUCTURES_MESSAGE_SENDER_H

#include <dpp/dpp.h>
#include <vector>
#include <string>
#include "feature.h"

namespace dpp_structures {




	class message_sender : feature {
	public:
		static constexpr int CHARACTER_LIMIT = 2000 - 1;

		class paragraph_too_long : std::exception {};

	private:
		// unused
		std::queue<dpp::message> queue;

	public:
		static std::vector<std::string> slice(const std::string& text);


#if DPP_CORO

		dpp::task<std::pair<int, std::optional<dpp::error_info>>> co_send(const std::string& text,
																		  dpp::snowflake channel_id);

		dpp::task<std::pair<int, std::optional<dpp::error_info>>>
		co_send(const std::vector<std::string>& messages_content, dpp::snowflake channel_id);

		dpp::task<std::pair<int, std::optional<dpp::error_info>>> co_send(const std::vector<dpp::message*>& messages);

#endif
	};
}// namespace dpp_structures


#endif//DPP_STRUCTURES_MESSAGE_SENDER_H
