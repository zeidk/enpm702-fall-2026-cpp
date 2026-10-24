/**
 * @file obstacle.hpp
 * @brief The autonomous car's types, shared by every file of week 8.
 * The same two structs as week 7 (L7).
 * @author Zeid Kootbally
 *
 * @details The ego vehicle drives along a two-lane road. Its tracker keeps one
 * Obstacle for each obstacle it sees. x runs along the road from
 * where the ego vehicle starts, y to the left of the center of its lane, and
 * heading is 0 along the road, counterclockwise.
 *
 * With g++ 13.3 on x86-64: sizeof(Pose) is 24 and sizeof(Obstacle) is 48, with
 * 4 bytes of padding after id and 7 after is_static.
 */
#pragma once

/// Where an obstacle is, and which way it faces.
struct Pose {
  double x{0.0};        ///< meters, along the road
  double y{0.0};        ///< meters, to the left of the lane center
  double heading{0.0};  ///< radians, 0 along the road
};

/// One obstacle the tracker sees: one row of L7's table of four obstacles.
struct Obstacle {
  int id{0};              ///< unique, positive
  Pose pose{};            ///< where it is now
  double speed{0.0};      ///< meters per second
  bool is_static{false};  ///< true when it does not move (the parked car)
};
