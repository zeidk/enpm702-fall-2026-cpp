/**
 * @file route.cpp
 * @brief The methods of Route.
 * @author Zeid Kootbally
 */
#include "route.hpp"

#include <cmath>
#include <cstddef>

void Route::add(Position waypoint) { waypoints_.push_back(waypoint); }

double Route::length() const {
  double total{0.0};
  for (std::size_t i{1}; i < waypoints_.size(); ++i) {
    total += std::hypot(waypoints_[i].x - waypoints_[i - 1].x,
                        waypoints_[i].y - waypoints_[i - 1].y);
  }
  return total;
}
