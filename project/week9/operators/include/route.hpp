/**
 * @file route.hpp
 * @brief Operator Overloading, the L9 reading on Read the Docs: the class
 *        Route of common/, with operator< and operator> by length.
 * @author Zeid Kootbally
 */
#pragma once

#include <vector>

#include "position.hpp"

namespace fleet {

/// The waypoints of one trip, in order.
class Route {
 public:
  /// Adds a waypoint at the end of the route.
  void add(Position waypoint);

  /// The length of the route, in m: the sum of the straight segments.
  [[nodiscard]] double length() const;

  /// True when this route is shorter than other.
  [[nodiscard]] bool operator<(const Route& other) const;

  /// True when this route is longer than other.
  [[nodiscard]] bool operator>(const Route& other) const;

 private:
  std::vector<Position> waypoints_;
};

}  // namespace fleet
