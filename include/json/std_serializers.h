//
// Created by backo on 28/07/25.
//

#ifndef SCARECROW_STD_SERIALIZERS_H
#define SCARECROW_STD_SERIALIZERS_H

#include <dpp/dpp.h>

NLOHMANN_JSON_NAMESPACE_BEGIN

template<typename T>
struct adl_serializer<std::optional<T>> {
	static void to_json(json& j, const std::optional<T>& opt) {
		if (opt.has_value()) j = *opt;
		else
			j = nullptr;
	}

	static void from_json(const json& j, std::optional<T>& opt) {
		if (j.is_null()) {
			opt = std::nullopt;
		} else {
			opt = j.template get<T>();// same as above, but with
									  // adl_serializer<T>::from_json
		}
	}
};

template<typename T>
struct adl_serializer<std::unique_ptr<T>> {
	static void to_json(json& j, const std::unique_ptr<T>& ptr) {
		if (ptr) j = *ptr;
		else
			j = nullptr;
	}

	static void from_json(const json& j, std::unique_ptr<T>& ptr) { ptr.reset(new T(j.template get<T>())); }
};

NLOHMANN_JSON_NAMESPACE_END

#endif//SCARECROW_STD_SERIALIZERS_H
