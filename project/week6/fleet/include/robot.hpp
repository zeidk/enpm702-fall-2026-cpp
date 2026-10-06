#pragma once
/**
 * @file robot.hpp
 * @brief The types every part of the fleet manager shares.
 * @author Zeid Kootbally
 *
 * @details The L6 slides build these two structs step by step in Section 1
 * (struct). This header holds the finished version, with a default for
 * every member.
 */

/**
 * @brief A point on the warehouse floor.
 *
 * Meters, in the warehouse frame: the origin is the charging dock, x points
 * along the aisles and y across them.
 */
struct Position {
    double x{0.0};  ///< meters along the aisles
    double y{0.0};  ///< meters across the aisles
};

/**
 * @brief What one robot reports to the dispatcher.
 */
struct RobotStatus {
    int id{0};                  ///< unique, positive
    double battery_pct{100.0};  ///< 0 to 100; a new robot starts charged
    Position position{};        ///< where the robot is now
    bool busy{false};           ///< true while it carries out a task
};
