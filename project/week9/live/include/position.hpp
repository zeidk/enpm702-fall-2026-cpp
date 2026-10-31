/**
 * @file position.hpp
 * @brief L9, live coding: the struct Position, a point on the map in meters, given.
 * @author Zeid Kootbally
 *
 * @details Position is a struct, not a class: any x and any y make a position,
 * so there is no rule for a constructor or a setter to check (Core Guidelines C.2).
 */
#pragma once

namespace fleet {

/// A point on the map.
struct Position {
  double x{0.0};  ///< m, east
  double y{0.0};  ///< m, north
};

}  // namespace fleet
