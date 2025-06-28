//
// Created by backo on 25/06/25.
//

#include "message_sender.h"
#include "utilities/text.h"


using namespace dpp_structures;


std::vector<std::string> message_sender::slice(const std::string& text) {
    std::vector<std::string> paragraphs = split_string(text, "\n");

    std::vector<std::string> messages{""};

    size_t last_length = 0;
    for (const auto& current_paragraph : paragraphs) {
        if (current_paragraph.size() > message_sender::CHARACTER_LIMIT) throw message_sender::paragraph_too_long();

        if (last_length + current_paragraph.size() > CHARACTER_LIMIT) {
            last_length = 0;
            messages.emplace_back("");
        }

        last_length += current_paragraph.size();
        messages.back().append(current_paragraph + '\n');
    }

    return messages;
}

dpp::task<std::pair<int, std::optional<dpp::error_info>>> message_sender::co_send(const std::string& text, dpp::snowflake channel_id) {
    co_return co_await this->co_send( message_sender::slice(text), channel_id);
}

dpp::task<std::pair<int, std::optional<dpp::error_info>>>
message_sender::co_send(const std::vector<std::string>& messages_content, dpp::snowflake channel_id) {
    std::vector<dpp::message*> messages;
    for (const auto& message_content : messages_content) {
        messages.push_back(new dpp::message(channel_id, message_content));
    }

    co_return co_await this->co_send(messages);
}


dpp::task<std::pair<int, std::optional<dpp::error_info>>> message_sender::co_send(const std::vector<dpp::message*>& messages) {
    int i = 0;
    for (const auto& message : messages) {
        auto confirmation = co_await this->bot->co_message_create(*message);
        if (confirmation.is_error()) co_return {i, confirmation.get_error()};
        delete message;
        ++i;
    }

    co_return {i, std::nullopt};
}
