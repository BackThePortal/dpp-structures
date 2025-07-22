//
// Created by backo on 18/07/25.
//

#ifndef SCARECROW_CASES_H
#define SCARECROW_CASES_H

#include <dpp/dpp.h>
#include "../internal/string_literal.h"

namespace dpp_structures {
	enum cases { CAMEL_CASE, SNAKE_CASE };

	template<cases From, cases To>
	std::string convert_case(const std::string& source);


	template<cases From, cases To, internal::string_literal Source>
	struct convert_case_impl;

	template<internal::string_literal Source>
	struct convert_case_impl<SNAKE_CASE, CAMEL_CASE, Source> {
		using return_type = internal::string_literal<Source.size() - Source.count('_') + 1>;
		static constexpr return_type execute() {
			return_type output(' ');

			int i = 0;
			for (const char* it = Source.data; it != Source.end(); ++it) {
				if (*it == '_') {
					++it;
					if (it != Source.end()) output.data[i] = (char) internal::to_upper(*it);
				} else
					output.data[i] = *it;

				++i;
			}
			return output;
		}
	};


	template<cases From, cases To, internal::string_literal Source>
	consteval convert_case_impl<From, To, Source>::return_type convert_case() {
		return convert_case_impl<From, To, Source>::execute();
	}


	template<cases From, cases To>
	nlohmann::json convert_case_json(const nlohmann::json& source);

	template<>
	std::string convert_case<CAMEL_CASE, SNAKE_CASE>(const std::string& source);


	template<>
	std::string convert_case<SNAKE_CASE, CAMEL_CASE>(const std::string& source);

	template<>
	nlohmann::json convert_case_json<CAMEL_CASE, SNAKE_CASE>(const nlohmann::json& source);

}// namespace dpp_structures
#endif//SCARECROW_CASES_H
