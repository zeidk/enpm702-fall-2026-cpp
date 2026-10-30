/**
 * @file category.hpp
 * @brief CATEGORY(e): the value category of an expression, as a word.
 * @author Zeid Kootbally
 *
 * @details The same helper as in playground/src/moves.cpp. decltype((e)), with
 * two pairs of parentheses, is T& for an lvalue, T&& for an xvalue and T for a
 * prvalue. decltype does not evaluate e: CATEGORY(count++) leaves count as it
 * was, and CATEGORY(std::move(path)) moves nothing.
 *
 * CATEGORY is a macro, so a comma outside parentheses splits its argument.
 * Wrap such an expression in one more pair: CATEGORY((Pose{1.0, 2.0, 0.0})).
 * Extra parentheses do not change a category.
 */
#pragma once

#include <iomanip>
#include <iostream>
#include <string_view>
#include <type_traits>

/// "lvalue" when T is T&, "xvalue" when T is T&&, "prvalue" otherwise.
template <typename T>
constexpr std::string_view category() {
  if constexpr (std::is_lvalue_reference_v<T>) {
    return "lvalue";
  } else if constexpr (std::is_rvalue_reference_v<T>) {
    return "xvalue";
  } else {
    return "prvalue";
  }
}

// A macro, because decltype must see the expression itself, not a copy of it.
#define CATEGORY(e) category<decltype((e))>()

/// Prints one line: the expression as written, then its category.
inline void show(std::string_view expression, std::string_view value_category) {
  std::cout << "  " << std::left << std::setw(42) << expression << value_category
            << '\n';
}

// SHOW(count + 1) prints "count + 1" and "prvalue" on one line.
#define SHOW(e) show(#e, CATEGORY(e))
