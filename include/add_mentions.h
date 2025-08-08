//
// Created by backo on 04/08/25.
//

#ifndef SCARECROW_ADD_MENTIONS_H
#define SCARECROW_ADD_MENTIONS_H

#include <dpp/dpp.h>
#include <string_view>
#include <string>
#include <set>
/*
namespace dpp_structures {


	struct future_mention {
		enum mention_type : bool {
			CHANNEL,
			ROLE_OR_USER,
		};

		mention_type type;
		std::string::const_iterator start;
		std::string_view name;
		std::variant<std::string_view , std::string::const_iterator> a;

		future_mention(std::string::const_iterator start)
			: start(start), type(*start == '@' ? ROLE_OR_USER : CHANNEL) {}

		void set_end(std::string::const_iterator end) {
			this->name = std::string_view{this->start, end};
		}



		[[nodiscard]] std::size_t size() const {
			return this->name.size();
		}
	};

	/**
		 * Finds any substrings in the form of #abc-def or \@abcdef and replaces them with Discord mentions. The replacement
		 * is done finding all # and @ after a space. Spaces in names can be inserted using square bracket syntax.
		 * Examples: #debate, \@Moderator, @[non binary].
		 * Role and channel names have to be reasonable. Special symbols might cause unexpected results.
		 *
		 * @param text
		 * @return

	static std::string add_mentions(std::string& text, dpp::cluster* bot) {

		// Characters that end a mention, unless square brackets are used.
		static std::set<char> end_chars = {'.', '\n', '!', '?', ' '};

		mention_type mention_type;


		std::string_view name;
		std::string::const_iterator start = text.end();

		char prev = '\0';

		// Iterating using both iterators and indexes because `text` is modified during the loop.
		auto it = text.begin();
		for (int i = 0; i < text.size(); ++it, prev = text[i++]) {
			std::string::iterator current = it;

			if (start == text.end()) {
				// Not inside a mention

				if ((prev == '\0' || prev == ' ')) {
					if (*current == '#') {
						mention_type = CHANNEL;
						start = it;
					} else if (*current == '@') {
						mention_type = ROLE_OR_USER;
						start = it;
					}
				}
			} else {
				// Inside a mention
				if (end_chars.contains(*current)) { name = std::string_view{start, current}; }
			}
		}

		text.er
	}
}// namespace dpp_structures
*/
#endif//SCARECROW_ADD_MENTIONS_H
