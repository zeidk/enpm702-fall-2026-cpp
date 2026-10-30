/**
 * @file route.hpp
 * @brief The waypoints a vehicle drives through: Vehicle::drive() takes a Route.
 * @author Zeid Kootbally
 *
 * @details Kept small in L8. A Route holds its Position objects by value, in a
 * std::vector.
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

 private:
  std::vector<Position> waypoints_;
};

}  // namespace fleet
