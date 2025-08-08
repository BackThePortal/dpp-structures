//
// Created by backo on 29/05/25.
//

#ifndef DPP_STRUCTURES_STRING_LITERAL_H
#define DPP_STRUCTURES_STRING_LITERAL_H


#include <algorithm>
#include <string_view>

namespace dpp_structures::internal {
	/**
 * https://dev.to/sgf4/strings-as-template-parameters-c20-4joh
 */
	template<std::size_t N>
	struct string_literal {
		char data[N]{};

		/**
		 * Includes NULL character.
		 */
		static constexpr std::size_t Length = N;

		consteval explicit string_literal(char c) {
			std::fill_n(this->data, N - 1, c);
			this->data[N - 1] = '\0';
		}

		consteval string_literal(const char (&str)[N]) {
			std::copy_n(str, N, this->data);
		}// NOLINT(*-explicit-constructor)


		consteval bool operator==(const string_literal<N> str) const {
			return std::equal(str.data, str.data + N, this->data);
		}

		template<std::size_t N2>
		consteval bool operator==(const string_literal<N2> s) const {
			return false;
		}

		template<std::size_t N2>
		consteval string_literal<N + N2 - 1> operator+(const string_literal<N2> str) const {
			char newchar[N + N2 - 1]{};
			std::copy_n(this->data, N - 1, newchar);
			std::copy_n(str.data, N2, newchar + N - 1);
			return newchar;
		}

		consteval char operator[](std::size_t n) const { return this->data[n]; }

		/**
		 * Does NOT include NULL character.
		 * @return
		 */
		[[nodiscard]] consteval std::size_t size() const { return N - 1; }



		constexpr operator std::string_view() const {// NOLINT(*-explicit-constructor)
			return std::string_view(this->data, N - 1);
		}

		[[nodiscard]] consteval int count(char c) const {
			int n = 0;
			for (int i = 0; i < N; ++i) {
				if (this->data[i] == c) ++n;
			}
			return n;
		}

		[[nodiscard]] constexpr const char* begin() { return this->data; }

		[[nodiscard]] constexpr const char* end() const { return this->data + N; }
	};

	template<std::size_t s1, std::size_t s2>
	consteval auto operator+(string_literal<s1> fs, const char (&str)[s2]) {
		return fs + string_literal<s2>(str);
	}

	template<std::size_t s1, std::size_t s2>
	consteval auto operator+(const char (&str)[s2], string_literal<s1> fs) {
		return string_literal<s2>(str) + fs;
	}

	template<std::size_t s1, std::size_t s2>
	consteval auto operator==(string_literal<s1> fs, const char (&str)[s2]) {
		return fs == string_literal<s2>(str);
	}

	template<std::size_t s1, std::size_t s2>
	consteval auto operator==(const char (&str)[s2], string_literal<s1> fs) {
		return string_literal<s2>(str) == fs;
	}

	constexpr char to_upper(char c) {
		return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 'a' + 'A') : c;
	}

	constexpr char to_lower(char c) {
		return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
	}

	template<std::size_t N>
	constexpr std::array<char, N> to_upper(const char (&input)[N]) {
		std::array<char, N> result = {};
		for (std::size_t i = 0; i < N; ++i) {
			result[i] = to_upper(input[i]);
		}
		return result;
	}

	template<std::size_t N>
	constexpr std::array<char, N> to_lower(const char (&input)[N]) {
		std::array<char, N> result = {};
		for (std::size_t i = 0; i < N; ++i) {
			result[i] = to_lower(input[i]);
		}
		return result;
	}
}// namespace dpp_structures::internal

#endif//DPP_STRUCTURES_STRING_LITERAL_H
