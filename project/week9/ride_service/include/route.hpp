/**
 * @file route.hpp
 * @brief L9, ride service: the class Route, given. Vehicle::drive() takes a Route.
 * @author Zeid Kootbally
 *
 * @details A Route holds its Position objects by value, in a std::vector.
 */
#pragma once

#include <vector>

#include "position.hpp"

namespace fleet {

/// The waypoints of one trip, in order.
class Route {
 public:
  /**
   * @brief Adds a waypoint at the end of the route.
   * @param waypoint The point to add, in m on the map.
   */
  void add(Position waypoint);

  /**
   * @brief Returns the length of the route.
   * @return The sum of the straight segments between consecutive waypoints,
   *         in m; 0 for fewer than two waypoints.
   */
  [[nodiscard]] double length() const;

 private:
  std::vector<Position> waypoints_;  ///< in order; the Route owns them (by value)
};

}  // namespace fleet
