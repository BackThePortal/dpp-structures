//
// Created by backo on 28/07/25.
//

#ifndef SCARECROW_MACRO_SERIALIZERS_H
#define SCARECROW_MACRO_SERIALIZERS_H

#include "variant.h"

#define JSON_ENUM_2(EnumName, V1, V2)                                                                                  \
	enum class EnumName { V1, V2 };                                                                                    \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1}, {EnumName::V2, #V2}})

#define JSON_ENUM_3(EnumName, V1, V2, V3)                                                                              \
	enum class EnumName { V1, V2, V3 };                                                                                \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1}, {EnumName::V2, #V2}, {EnumName::V3, #V3}})

#define JSON_ENUM_4(EnumName, V1, V2, V3, V4)                                                                          \
	enum class EnumName { V1, V2, V3, V4 };                                                                            \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName,                                                                             \
								 {{EnumName::V1, #V1}, {EnumName::V2, #V2}, {EnumName::V3, #V3}, {EnumName::V4, #V4}})

#define JSON_ENUM_5(EnumName, V1, V2, V3, V4, V5)                                                                      \
	enum class EnumName { V1, V2, V3, V4, V5 };                                                                        \
	NLOHMANN_JSON_SERIALIZE_ENUM(                                                                                      \
			EnumName,                                                                                                  \
			{{EnumName::V1, #V1}, {EnumName::V2, #V2}, {EnumName::V3, #V3}, {EnumName::V4, #V4}, {EnumName::V5, #V5}})

#define JSON_ENUM_6(EnumName, V1, V2, V3, V4, V5, V6)                                                                  \
	enum class EnumName { V1, V2, V3, V4, V5, V6 };                                                                    \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1},                                                       \
											{EnumName::V2, #V2},                                                       \
											{EnumName::V3, #V3},                                                       \
											{EnumName::V4, #V4},                                                       \
											{EnumName::V5, #V5},                                                       \
											{EnumName::V6, #V6}})

#define JSON_ENUM_7(EnumName, V1, V2, V3, V4, V5, V6, V7)                                                              \
	enum class EnumName { V1, V2, V3, V4, V5, V6, V7 };                                                                \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1},                                                       \
											{EnumName::V2, #V2},                                                       \
											{EnumName::V3, #V3},                                                       \
											{EnumName::V4, #V4},                                                       \
											{EnumName::V5, #V5},                                                       \
											{EnumName::V6, #V6},                                                       \
											{EnumName::V7, #V7}})

#define JSON_ENUM_8(EnumName, V1, V2, V3, V4, V5, V6, V7, V8)                                                          \
	enum class EnumName { V1, V2, V3, V4, V5, V6, V7, V8 };                                                            \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1},                                                       \
											{EnumName::V2, #V2},                                                       \
											{EnumName::V3, #V3},                                                       \
											{EnumName::V4, #V4},                                                       \
											{EnumName::V5, #V5},                                                       \
											{EnumName::V6, #V6},                                                       \
											{EnumName::V7, #V7},                                                       \
											{EnumName::V8, #V8}})

#define JSON_ENUM_9(EnumName, V1, V2, V3, V4, V5, V6, V7, V8, V9)                                                      \
	enum class EnumName { V1, V2, V3, V4, V5, V6, V7, V8, V9 };                                                        \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1},                                                       \
											{EnumName::V2, #V2},                                                       \
											{EnumName::V3, #V3},                                                       \
											{EnumName::V4, #V4},                                                       \
											{EnumName::V5, #V5},                                                       \
											{EnumName::V6, #V6},                                                       \
											{EnumName::V7, #V7},                                                       \
											{EnumName::V8, #V8},                                                       \
											{EnumName::V9, #V9}})

#define JSON_ENUM_10(EnumName, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10)                                                \
	enum class EnumName { V1, V2, V3, V4, V5, V6, V7, V8, V9, V10 };                                                   \
	NLOHMANN_JSON_SERIALIZE_ENUM(EnumName, {{EnumName::V1, #V1},                                                       \
											{EnumName::V2, #V2},                                                       \
											{EnumName::V3, #V3},                                                       \
											{EnumName::V4, #V4},                                                       \
											{EnumName::V5, #V5},                                                       \
											{EnumName::V6, #V6},                                                       \
											{EnumName::V7, #V7},                                                       \
											{EnumName::V8, #V8},                                                       \
											{EnumName::V9, #V9},                                                       \
											{EnumName::V10, #V10}})

#define JSON_ENUM_GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

/**
 * Macro to automatically add to_json and from_json functions for an enum in which the enum value names are the same as the
 * strings in the JSON.
 */
#define DEFINE_JSON_ENUM(...)                                                                                          \
	JSON_ENUM_GET_MACRO(__VA_ARGS__, JSON_ENUM_10, JSON_ENUM_9, JSON_ENUM_8, JSON_ENUM_7, JSON_ENUM_6, JSON_ENUM_5,    \
						JSON_ENUM_4, JSON_ENUM_3, JSON_ENUM_2)(__VA_ARGS__)


#define CASE_CONV_JSON_TO(v1) dpp_structures::json::serialize(nlohmann_json_t.v1, nlohmann_json_j, std::string(#v1));

#define CASE_CONV_JSON_FROM(v1)                                                                                        \
	dpp_structures::json::deserialize(nlohmann_json_t.v1, nlohmann_json_j, std::string(#v1));
/**
 * Converts between camelCase for JSON keys and snake_case for member names.
*/
#define DEFINE_JSON_STRUCT(Type, ...)                                                                                  \
	inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) {                                \
		std::cout << "to_json " << #Type << '\n';                                                                      \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(CASE_CONV_JSON_TO, __VA_ARGS__))                                      \
	}                                                                                                                  \
	inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) {                              \
		std::cout << "from_json " << #Type << '\n';                                                                    \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(CASE_CONV_JSON_FROM, __VA_ARGS__))                                    \
	}


#define DEFINE_JSON_STRUCT_FORWARD(Type)                                                                               \
	void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t);                                        \
	void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t);


#define DEFINE_JSON_EMPTY_STRUCT(Type)                                                                                 \
	inline void to_json(nlohmann::json&, const Type&) {}                                                               \
	inline void from_json(const nlohmann::json&, Type&) {}


#define SUPER_STRUCT_TO_JSON(BaseType)                                                                                 \
	nlohmann::to_json(nlohmann_json_j, static_cast<const BaseType&>(nlohmann_json_t));

#define SUPER_STRUCT_FROM_JSON(BaseType) nlohmann::from_json(nlohmann_json_j, static_cast<BaseType&>(nlohmann_json_t));

#define DEFINE_JSON_STRUCT_DERIVED(Type, BaseType, ...)                                                                \
	inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) {                                \
		SUPER_STRUCT_TO_JSON(BaseType)                                                                                 \
		__VA_OPT__(NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(CASE_CONV_JSON_TO, __VA_ARGS__)))                          \
	}                                                                                                                  \
	inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) {                              \
		SUPER_STRUCT_FROM_JSON(BaseType)                                                                               \
		__VA_OPT__(NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(CASE_CONV_JSON_FROM, __VA_ARGS__)))                        \
	}


#define JSON_FIELD(x) json::field<#x, x>

#define DECLARE_BASIC_JSON_VARIANT_1(a1) json::variant<JSON_FIELD(a1)>
#define DECLARE_BASIC_JSON_VARIANT_2(a1, a2) json::variant<JSON_FIELD(a1), JSON_FIELD(a2)>
#define DECLARE_BASIC_JSON_VARIANT_3(a1, a2, a3) json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3)>
#define DECLARE_BASIC_JSON_VARIANT_4(a1, a2, a3, a4)                                                                   \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4)>
#define DECLARE_BASIC_JSON_VARIANT_5(a1, a2, a3, a4, a5)                                                               \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5)>
#define DECLARE_BASIC_JSON_VARIANT_6(a1, a2, a3, a4, a5, a6)                                                           \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6)>
#define DECLARE_BASIC_JSON_VARIANT_7(a1, a2, a3, a4, a5, a6, a7)                                                       \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7)>
#define DECLARE_BASIC_JSON_VARIANT_8(a1, a2, a3, a4, a5, a6, a7, a8)                                                   \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8)>
#define DECLARE_BASIC_JSON_VARIANT_9(a1, a2, a3, a4, a5, a6, a7, a8, a9)                                               \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9)>
#define DECLARE_BASIC_JSON_VARIANT_10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)                                         \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10)>
#define DECLARE_BASIC_JSON_VARIANT_11(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11)                                    \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11)>
#define DECLARE_BASIC_JSON_VARIANT_12(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)                               \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11), JSON_FIELD(a12)>
#define DECLARE_BASIC_JSON_VARIANT_13(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13)                          \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11), JSON_FIELD(a12),   \
				  JSON_FIELD(a13)>
#define DECLARE_BASIC_JSON_VARIANT_14(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14)                     \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11), JSON_FIELD(a12),   \
				  JSON_FIELD(a13), JSON_FIELD(a14)>
#define DECLARE_BASIC_JSON_VARIANT_15(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15)                \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11), JSON_FIELD(a12),   \
				  JSON_FIELD(a13), JSON_FIELD(a14), JSON_FIELD(a15)>
#define DECLARE_BASIC_JSON_VARIANT_16(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16)           \
	json::variant<JSON_FIELD(a1), JSON_FIELD(a2), JSON_FIELD(a3), JSON_FIELD(a4), JSON_FIELD(a5), JSON_FIELD(a6),      \
				  JSON_FIELD(a7), JSON_FIELD(a8), JSON_FIELD(a9), JSON_FIELD(a10), JSON_FIELD(a11), JSON_FIELD(a12),   \
				  JSON_FIELD(a13), JSON_FIELD(a14), JSON_FIELD(a15), JSON_FIELD(a16)>

#define GET_VARIANT_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME

#define DECLARE_BASIC_JSON_VARIANT(...)                                                                                \
	GET_VARIANT_MACRO(__VA_ARGS__, DECLARE_BASIC_JSON_VARIANT_16, DECLARE_BASIC_JSON_VARIANT_15,                       \
					  DECLARE_BASIC_JSON_VARIANT_14, DECLARE_BASIC_JSON_VARIANT_13, DECLARE_BASIC_JSON_VARIANT_12,     \
					  DECLARE_BASIC_JSON_VARIANT_11, DECLARE_BASIC_JSON_VARIANT_10, DECLARE_BASIC_JSON_VARIANT_9,      \
					  DECLARE_BASIC_JSON_VARIANT_8, DECLARE_BASIC_JSON_VARIANT_7, DECLARE_BASIC_JSON_VARIANT_6,        \
					  DECLARE_BASIC_JSON_VARIANT_5, DECLARE_BASIC_JSON_VARIANT_4, DECLARE_BASIC_JSON_VARIANT_3,        \
					  DECLARE_BASIC_JSON_VARIANT_2, DECLARE_BASIC_JSON_VARIANT_1)(__VA_ARGS__)


#endif//SCARECROW_MACRO_SERIALIZERS_H
