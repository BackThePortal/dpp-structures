//
// Created by backo on 18/07/25.
//
#include "utilities/cases.h"

using namespace dpp_structures;

template<>
std::string dpp_structures::convert_case<CAMEL_CASE, SNAKE_CASE>(const std::string& source) {
	std::string output;

	for (const char& c : source) {
		if (islower(c)) output += c;
		if (isupper(c)) {
			output += '_';
			output += (char) tolower(c);
		}
	}

	return output;
}

template<>
std::string dpp_structures::convert_case<SNAKE_CASE, CAMEL_CASE>(const std::string& source) {
	std::string output;

	for (auto it = source.begin(); it != source.end(); ++it) {
		if (*it == '_') {
			++it;
			if (it != source.end()) output += (char) toupper(*it);
		} else
			output += *it;
	}

	return output;
}
template<>
nlohmann::json dpp_structures::convert_case_json<CAMEL_CASE, SNAKE_CASE>(const nlohmann::json& source) {
	if (!source.is_structured()) return source;

	nlohmann::json out = nlohmann::json::array();

	if (source.is_array()) {
		for (auto it = source.begin(); it != source.end(); ++it) {
			out.push_back(convert_case<CAMEL_CASE, SNAKE_CASE>(it.value()));
		}
	} else {
		for (auto it = source.begin(); it != source.end(); ++it) {
			out[convert_case<CAMEL_CASE, SNAKE_CASE>(it.key())] = convert_case<CAMEL_CASE, SNAKE_CASE>(it.value());
		}
	}

	return out;
}