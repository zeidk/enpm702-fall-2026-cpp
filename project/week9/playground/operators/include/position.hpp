/**
 * @file position.hpp
 * @brief Operator Overloading, the L9 reading on Read the Docs: the struct
 *        Position of ../../include/position.hpp, with an operator<< that prints it.
 * @author Zeid Kootbally
 *
 * @details operator<< is declared in namespace fleet, next to Position, so
 * that std::cout << p finds it from any namespace.
 */
#pragma once

#include <ostream>

namespace fleet {

/// A point on the map.
struct Position {
  double x{0.0};  ///< m, east
  double y{0.0};  ///< m, north
};

/// Prints p as (x, y) and returns the stream, so that calls can be chained.
std::ostream& operator<<(std::ostream& out, const Position& p);

}  // namespace fleet
