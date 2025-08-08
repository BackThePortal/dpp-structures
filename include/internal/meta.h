//
// Created by backo on 4/8/2024.
//

#ifndef DPP_STRUCTURES_META_H
#define DPP_STRUCTURES_META_H

#include <concepts>
#include <utility>
#include <optional>

namespace dpp_structures::internal {
	template<typename...>
	inline constexpr bool all_unique = true;

	template<typename T, typename... Rest>
	inline constexpr bool all_unique<T, Rest...> = (!std::is_same_v<T, Rest> && ...) && all_unique<Rest...>;

	template<typename T, typename U>
	concept derived = std::is_base_of<U, T>::value;


	template<typename T, template<auto...> class Template>
	struct is_instance_of : std::false_type {};

	// Specialization: matches T if it's an instantiation of the Template
	template<template<auto...> class Template, auto... Args>
	struct is_instance_of<Template<Args...>, Template> : std::true_type {};

	namespace detail {

		template<bool Const = false, typename R = void, typename C = void, typename... Args>
		struct member_function_pointer_info_detail {
			using class_type = C;
			using return_type = R;
			using arguments = std::tuple<Args...>;
			static constexpr bool is_const = Const;
			explicit member_function_pointer_info_detail(auto){}

		};
		template<typename R, typename C, typename... Args>
		member_function_pointer_info_detail(R (C::**)(Args...))
				-> member_function_pointer_info_detail<false, R, C, Args...>;
		template<typename R, typename C, typename... Args>
		member_function_pointer_info_detail(R (C::**)(Args...) const)
				-> member_function_pointer_info_detail<true, R, C, Args...>;
	}// namespace detail

	template<typename mfp>
	using member_function_pointer_info = decltype(detail::member_function_pointer_info_detail((mfp*) {}));




	template<typename T>
	typename std::add_rvalue_reference<T>::type declval();

	template<typename T>
	struct is_optional : std::false_type {};

	template<typename T>
	struct is_optional<std::optional<T>> : std::true_type {};

	template<typename... Types>
	struct pack {};

	template<typename T, typename... Args>
	struct is_friend_constructible {
	private:
		template<typename U, typename = decltype(U(std::declval<Args>()...))>
		static std::true_type test(int) {
			return {};
		};

		template<typename>
		static std::false_type test(...) {
			return {};
		};

	public:
		static constexpr bool value = decltype(test<T>(0))::value;
	};

	template<typename T, typename... Args>
	constexpr bool is_friend_constructible_v = is_friend_constructible<T, Args...>::value;

	template<auto Value>
	using get_type_from_value = std::remove_cvref_t<decltype(Value)>;

	template<std::size_t N, typename... Ts>
	using get_type_from_pack = std::tuple_element_t<N, std::tuple<Ts...>>;

	/**
	 * Get the first type of a type template parameter pack.
	 */
	template<typename... Ts>
	using first_type = get_type_from_pack<0, Ts...>;

	/**
	 * Get the first value of a non-type template parameter pack.
	 */
	template<auto T, auto... Ts>
	constexpr auto first_value = T;

}// namespace dpp_structures::internal
#endif//DPP_STRUCTURES_META_H
