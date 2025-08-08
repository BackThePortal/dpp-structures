//
// Created by backo on 30/07/25.
//

#ifndef SCARECROW_BITMASK_H
#define SCARECROW_BITMASK_H

#include "../internal/meta.h"
#include "../internal/enums.h"
#include <type_traits>
namespace dpp_structures {
	template<typename Enum>
		requires std::is_enum_v<Enum>
	class bitmask {
		using underlying_type = std::underlying_type_t<Enum>;

		underlying_type mask = 0;

		static constexpr underlying_type mask_value(Enum value) { return 1 << static_cast<underlying_type>(value); }

	public:
		constexpr bitmask() = default;
		template<std::same_as<Enum>... Ts> requires (sizeof...(Ts) != 0)
		constexpr bitmask(Ts&&... values) { // NOLINT(*-explicit-constructor)
			set(values...);
		}


		constexpr bool get(Enum value) {
			return this->mask & mask_value(value);
		}

		constexpr void set(Enum value) {
			this->mask |= mask_value(value);
		}

		template<std::same_as<Enum>... Ts> requires (sizeof...(Ts) != 0)
		constexpr void set(Ts&&... value) {
			(this->set(value), ...);
		}


		constexpr bool operator&(Enum value) {
			return this->get(value);
		}

		constexpr bool operator|=(Enum value) {
			return this->set(value);
		}


		constexpr bitmask<Enum> operator|(Enum value) {
			return {this->mask | mask_value(value)};
		}

		template<std::same_as<Enum>... Ts> requires (sizeof...(Ts) != 0)
		constexpr bool all(Ts&&... value) {
			return (this->get(value) && ...);
		}

		template<std::same_as<Enum>... Ts> requires (sizeof...(Ts) != 0)
		constexpr bool any(Ts&&... value) {
			return (this->get(value) || ...);
		}
	};

}// namespace dpp_structures
// TODO
#endif//SCARECROW_BITMASK_H
