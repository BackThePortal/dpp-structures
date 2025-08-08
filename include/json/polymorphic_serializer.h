//
// Created by backo on 29/07/25.
//

#ifndef SCARECROW_POLYMORPHIC_SERIALIZER_H
#define SCARECROW_POLYMORPHIC_SERIALIZER_H

#include <dpp/dpp.h>
#include "utilities/cases.h"
#include "variant.h"
#include "std_serializers.h"

/*
#define CASE_CONV_JSON_TO(v1)                                                                                          \
nlohmann_json_j[::dpp_structures::convert_case<SNAKE_CASE, CAMEL_CASE>(#v1)] = nlohmann_json_t.v1;


#define CASE_CONV_JSON_FROM(v1)                                                                                        \
nlohmann_json_j.at(::dpp_structures::convert_case<SNAKE_CASE, CAMEL_CASE>(#v1)).get_to(nlohmann_json_t.v1);

*/

inline nlohmann::json get_safe(const nlohmann::json& json, const std::string& property) {
	return json.contains(property) ? json[property] : nullptr;
}

namespace dpp_structures::json {
	template<typename T>
	void serialize(const T& value, nlohmann::json& json, const std::string& property_name, bool convert_case = true) {
		std::cout << "\tserialize generic\n\t\tproperty: " << property_name << "\n\t\ttype: " << typeid(T).name()
				  << '\n';
		get_safe(json, convert_case ? ::dpp_structures::convert_case<SNAKE_CASE, CAMEL_CASE>(property_name)
									: property_name) = value;
	}

	template<typename... Fields>
	void serialize(const variant<Fields...>& value, nlohmann::json& json, const std::string& property_name) {
		std::cout << "\tserialize variant\n\t\tproperty: " << property_name << '\n';
		value.serialize(json);
	}

	template<typename T>
	void deserialize(T& value, const nlohmann::json& json, const std::string& property_name, bool convert_case = true) {
		std::cout << "\tdeserialize generic\n\t\tproperty: " << property_name << "\n\t\ttype: " << typeid(T).name()
				  << '\n';


		nlohmann::adl_serializer<T>::from_json(
				get_safe(json, convert_case ? ::dpp_structures::convert_case<SNAKE_CASE, CAMEL_CASE>(property_name)
											: property_name),
				value);
	}


	template<typename... Fields>
	void deserialize(variant<Fields...>& value, const nlohmann::json& json, const std::string& property_name) {
		std::cout << "\tdeserialize variant\n\t\tproperty: " << property_name
				  << "\n\t\tvariant alt index:" << value.index() << '\n';

		value = value.deserialize(json);
	}
}// namespace dpp_structures::json

#endif//SCARECROW_POLYMORPHIC_SERIALIZER_H
