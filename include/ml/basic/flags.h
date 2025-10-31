/**
 * @file flags.h
 * @brief Bitmask handling definitions for My Language.
 * @details Defines bitmask operator overloads and utility functions.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

#include <type_traits>

namespace ml::basic {

/**
 * @struct enable_bitmask_operators flags.h
 * @brief Trait to enable bitmask operators for enum classes.
 * @details Specialize this template for an enum class to enable bitmask
 * operators.
 */
template <typename E> struct enable_bitmask_operators {
  static constexpr bool enable = false;
};

/**
 * @brief Enables bitmask operators for the specified enum class.
 */
#define ENABLE_BITMASKS(ClassName)                                             \
  template <> struct enable_bitmask_operators<ClassName> {                     \
    static constexpr bool enable = true;                                       \
  };

// Bitwise OR
template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator|(E lhs, E rhs) {
  using underlying = typename std::underlying_type<E>::type;
  return static_cast<E>(static_cast<underlying>(lhs) |
                        static_cast<underlying>(rhs));
}

// Bitwise AND
template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator&(E lhs, E rhs) {
  using underlying = typename std::underlying_type<E>::type;
  return static_cast<E>(static_cast<underlying>(lhs) &
                        static_cast<underlying>(rhs));
}

// Bitwise XOR
template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator^(E lhs, E rhs) {
  using underlying = typename std::underlying_type<E>::type;
  return static_cast<E>(static_cast<underlying>(lhs) ^
                        static_cast<underlying>(rhs));
}

// Bitwise NOT
template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator~(E rhs) {
  using underlying = typename std::underlying_type<E>::type;
  return static_cast<E>(~static_cast<underlying>(rhs));
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator|=(E &lhs, E rhs) {
  lhs = lhs | rhs;
  return lhs;
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator&=(E &lhs, E rhs) {
  lhs = lhs & rhs;
  return lhs;
}

template <typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E &>::type
operator^=(E &lhs, E rhs) {
  lhs = lhs ^ rhs;
  return lhs;
}

/**
 * @brief Checks if a specific flag is set in the given flags.
 * @tparam E The enum class type.
 * @param flags The combined flags.
 * @param flag The specific flag to check.
 * @return True if the flag is set, false otherwise.
 */
template <typename E> bool hasFlag(E flags, E flag) {
  return (flags & flag) == flag;
}

/**
 * @brief Adds a specific flag to the given flags.
 * @tparam E The enum class type.
 * @param flags The combined flags.
 * @param flag The specific flag to add.
 * @return The updated flags with the added flag.
 */
template <typename E> E addFlag(E flags, E flag) { return flags |= flag; }

/**
 * @brief Removes a specific flag from the given flags.
 * @tparam E The enum class type.
 * @param flags The combined flags.
 * @param flag The specific flag to remove.
 * @return The updated flags with the removed flag.
 */
template <typename E> E removeFlag(E flags, E flag) { return flags &= ~flag; }

} // namespace ml::basic
