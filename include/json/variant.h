//
// Created by backo on 28/07/25.
//

#ifndef SCARECROW_VARIANT_H
#define SCARECROW_VARIANT_H

#include <dpp/dpp.h>
#include "internal/string_literal.h"
#include "internal/meta.h"
#include "utilities/cases.h"
#include <variant>

namespace dpp_structures::json {


	struct no_enum {};

	inline bool operator==(const no_enum&, const no_enum&) { return true; }

	template<typename Enum>
		requires std::is_scoped_enum_v<Enum> || std::is_same_v<Enum, no_enum>
	struct field_info {
		std::string name;
		Enum enum_value;
		std::size_t pos;
	};

	template<>
	struct field_info<no_enum> {
		std::string name;
		std::size_t pos;
	};


	template<typename EnumA, typename EnumB>
	inline bool operator==(const field_info<EnumA>&, const field_info<EnumB>&) {
		return false;
	}

	template<typename Enum>
	inline bool operator==(const field_info<Enum>& a, const field_info<Enum>& b) {
		return a.pos == b.pos && a.enum_value == b.enum_value && a.name == b.name;
	}


	template<typename Type, typename Enum, std::size_t NameLength>
		requires std::is_scoped_enum_v<Enum> || std::is_same_v<Enum, no_enum>
	struct field_info_c {
		using type = Type;
		internal::string_literal<NameLength> name;
		Enum enum_value;
		std::size_t pos;

		operator field_info<Enum>() { return {name.data, enum_value, pos}; }
	};

	template<typename Type, std::size_t NameLength>
	struct field_info_c<Type, no_enum, NameLength> {
		using type = Type;
		internal::string_literal<NameLength> name;
		std::size_t pos;

		operator field_info<no_enum>() { return {name.data, pos}; }
	};


	template<typename Type, typename Enum, std::size_t NameLength>
	inline constexpr bool operator==(const field_info_c<Type, Enum, NameLength>& a,
									 const field_info_c<Type, Enum, NameLength>& b) {
		return a.pos == b.pos && a.enum_value == b.enum_value && a.name == b.name;
	}


	template<internal::string_literal TName, typename Type, auto TEnumValue = no_enum()>
	struct field {
		using type = Type;
		static constexpr internal::string_literal Name = TName;

		static constexpr bool is_tagged = !std::is_same_v<internal::get_type_from_value<TEnumValue>, no_enum>;
		using enum_type = internal::get_type_from_value<TEnumValue>;
		static_assert(std::is_scoped_enum_v<enum_type> || std::is_same_v<enum_type, no_enum>);

		static constexpr auto EnumValue = TEnumValue;
	};


	template<typename T>
	struct is_field_base {
		template<internal::string_literal TName, typename Type, auto TEnumValue>
		static std::true_type test(const field<TName, Type, TEnumValue>*) {
			return {};
		};

		static std::false_type test(...) { return {}; };

		static constexpr bool value = decltype(test(std::declval<T*>()))::value;
	};

	template<typename T>
	concept FieldTemplate = is_field_base<T>::value;


	template<FieldTemplate... Fields>
	class variant : public std::variant<typename Fields::type...> {
	public:
		using std::variant<typename Fields::type...>::variant;
		using std_variant = std::variant<typename Fields::type...>;

		using first_field = std::tuple_element<0, std::tuple<Fields...>>::type;
		using enum_type = first_field::enum_type;

	private:
		static consteval bool all_tagged() { return (Fields::is_tagged && ...); }
		static consteval bool none_tagged() { return (!Fields::is_tagged && ...); }

		static constexpr bool all_enums_same = (std::is_same_v<enum_type, typename Fields::enum_type> && ...);

		static_assert(all_enums_same);

	public:
		static constexpr bool is_tagged = all_tagged();
		using enum_type_or_int = std::conditional_t<is_tagged, enum_type, int>;

		static constexpr std::size_t Size = sizeof...(Fields);
		static const std::string names[Size];
		static constexpr std::string_view names_v[Size] = {(std::string_view) Fields::Name...};
		static constexpr enum_type enum_values[Size] = {Fields::EnumValue...};

		static std::size_t find_field_index(enum_type_or_int enum_value)
			requires is_tagged
		{
			int index = 0;
			for (const auto& item : enum_values) {
				if (item == enum_value) break;
				++index;
			}

			return index == Size ? -1 : index;
		}

		static std::size_t find_field_index(const std::string& name) {
			int index = 0;
			for (const auto& item : names) {
				if (item == name) break;
				++index;
			}

			return index == Size ? -1 : index;
		}

		template<std::size_t N>
		static consteval std::size_t find_field_index_c(const char (&str)[N]) {
			return find_field_index_c(internal::string_literal<N>(str));
		}

		template<std::size_t Length>
		static consteval std::size_t find_field_index_c(internal::string_literal<Length> name) {
			int index;
			for (index = 0; index < Size; ++index) {
				if (names_v[index] == name) break;
			}

			return index == Size ? -1 : index;
		}

		template<internal::string_literal Name>
		static consteval auto find_field_c() {
			constexpr auto index = find_field_index_c(Name);
			return find_field_c<index>();
		}


		static consteval std::size_t find_field_index_c(enum_type enum_value)
			requires is_tagged
		{
			int index;
			for (index = 0; index < Size; ++index) {
				if (enum_values[index] == enum_value) break;
			}

			return index == Size ? -1 : index;
		};

		template<FieldTemplate Field>
		static consteval std::size_t find_field_index_c() {
			return find_field_index_c(Field::Name);
		}

		template<std::size_t N>
		using index_alternative_t = internal::get_type_from_pack<N, Fields...>;

		template<std::size_t index>
		static consteval auto find_field_c() {
			using alternative = index_alternative_t<index>;
			constexpr auto name = alternative::Name;
			if constexpr (is_tagged)
				return field_info_c<typename alternative::type, enum_type, name.Length>{name, enum_values[index],
																						index};
			else
				return field_info_c<typename alternative::type, enum_type, name.Length>{name, index};
		};

		static field_info<enum_type> find_field(std::size_t index) {
			if constexpr (is_tagged) return {names[index], enum_values[index], index};
			else
				return {names[index], index};
		}

		template<enum_type EnumValue>
		static consteval auto find_field_c()
			requires is_tagged
		{
			constexpr auto index = find_field_index_c(EnumValue);
			return find_field_c<index>();
		}

		static field_info<enum_type> find_field(enum_type enum_value)
			requires is_tagged
		{
			return find_field(find_field_index(enum_value));
		}

	private:
		static consteval bool check_all_names_different() {
			for (int i = 0; i < Size; ++i) {
				for (int j = 0; j < Size; ++j) {
					if (i != j && names_v[i] == names_v[j]) return false;
				}
			}
			return true;
		}

		static consteval bool check_all_enum_values_different() {
			for (int i = 0; i < Size; ++i) {
				for (int j = 0; j < Size; ++j) {
					if (i != j && enum_values[i] == enum_values[j]) return false;
				}
			}
			return true;
		}

		static constexpr bool all_types_different = internal::all_unique<typename Fields::type...>;

		static_assert(is_tagged || all_types_different);
		static_assert(check_all_names_different());
		static_assert(!is_tagged || check_all_enum_values_different());

		template<FieldTemplate Field>
		static bool try_parse_field(const nlohmann::json& object, variant& out) {
			const std::string_view name = Field::Name;
			std::string property = convert_case<SNAKE_CASE, CAMEL_CASE>(std::string(name));
			static constexpr int index = find_field_index_c<Field>();

			if (object.contains(property)) {
				out = variant{std::in_place_index<index>, object[property].get<typename Field::type>()};
				return true;
			}
			return false;
		}
		template<enum_type EnumValue, bool Tagged>
		struct alternative_helper;

		template<enum_type EnumValue>
		struct alternative_helper<EnumValue, true> {
			using type = internal::get_type_from_pack<find_field_index_c(EnumValue), Fields...>::type;
		};

		// Fallback to void or static_assert if not tagged
		template<enum_type EnumValue>
		struct alternative_helper<EnumValue, false> {
			// static_assert(false, ...) would fire always, so return dummy
			using type = void;
		};

	public:
		template<enum_type EnumValue>
		using alternative_t = typename alternative_helper<EnumValue, is_tagged>::type;

		template<internal::string_literal Name>
		using name_alternative_t = internal::get_type_from_pack<find_field_index_c(Name), Fields...>;


		[[nodiscard]] field_info<enum_type> current_field() const { return find_field(this->index()); }

		[[nodiscard]] enum_type enum_value() const { return this->current_field().enum_value; }

		template<enum_type EnumValue>
		[[nodiscard]] alternative_t<EnumValue> get() const {
			return std::get<find_field_index_c(EnumValue)>(*this);
		}

		template<std::size_t Index>
		[[nodiscard]] std::variant_alternative_t<Index, std_variant> get() const {
			return std::get<std::variant_alternative_t<Index, std_variant>>(*this);
		}


		[[nodiscard]] static variant<Fields...> deserialize(const nlohmann::json& object) {
			variant result;
			bool matched = (try_parse_field<Fields>(object, result) || ...);

			if (!matched) { throw std::runtime_error("No matching field in JSON object"); }

			return result;
		}

		void serialize(nlohmann::json& parent) const {
			field_info<enum_type> field_info = find_field(this->index());

			std::visit([&field_info,
						&parent](auto&& obj) { parent[convert_case<SNAKE_CASE, CAMEL_CASE>(field_info.name)] = obj; },
					   *this);
		}
	};

	template<FieldTemplate... Fields>
	const std::string variant<Fields...>::names[Size] = {(std::string) Fields::Name.data...};


	namespace test {
		enum class a { BBB, CCC, ZZZ };

		using variant_test = variant<field<"glyphType", long long, a::BBB>, field<"glyphSymbol", long long, a::CCC>>;
		using variant_test2 = variant<field<"glyphType", long long>, field<"glyphSymbol", double>>;

		static_assert(variant_test::find_field_index_c("glyphType") == 0);
		static_assert(variant_test::find_field_index_c(a::BBB) == 0);
		static_assert(variant_test::find_field_index_c(a::CCC) == 1);
		static_assert(variant_test::find_field_c<0>().name == "glyphType");
		static_assert(variant_test::find_field_c<0>().enum_value == a::BBB);
		static_assert(variant_test::find_field_c<1>() == variant_test::find_field_c<a::CCC>());

		static_assert(variant_test2::find_field_index_c("glyphType") == 0);
		static_assert(variant_test2::find_field_c<"glyphType">().pos == 0);
		static_assert(std::is_same_v<variant_test2::enum_type, no_enum>);

	}// namespace test


}// namespace dpp_structures::json


#endif//SCARECROW_VARIANT_H
