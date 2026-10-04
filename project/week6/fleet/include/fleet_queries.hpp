#pragma once
/**
 * @file fleet_queries.hpp
 * @brief Questions the dispatcher asks about the fleet.
 *
 * @details Each function takes the fleet and returns a value, so each one can
 * be checked by a test with a known input and a known answer. The tests are in
 * @c ../../tests/test_fleet.cpp.
 */
#include <optional>
#include <utility>
#include <vector>

#include "robot.hpp"

/**
 * @brief The four robots used on every slide of L6.
 *
 * | id | battery | position | state |
 * |----|---------|----------|-------|
 * | 1  | 82.5 %  | (0, 0)   | idle  |
 * | 2  | 35.0 %  | (4, 1)   | busy  |
 * | 3  | 64.0 %  | (2, 3)   | idle  |
 * | 4  | 18.0 %  | (6, 2)   | idle  |
 */
std::vector<RobotStatus> make_demo_fleet();

/**
 * @brief Average of a list of battery levels.
 * @param battery_pct The battery levels, in percent.
 * @return Their average, or 0.0 for an empty list.
 */
double average_battery(const std::vector<double>& battery_pct);

/**
 * @brief Lowest and highest battery level in the fleet.
 * @param fleet The robots. Must not be empty.
 * @return The lowest level as @c first, the highest as @c second.
 */
std::pair<double, double> find_battery_range(const std::vector<RobotStatus>& fleet);

/// Three facts about the fleet, with names.
struct FleetSummary {
    double min_battery_pct;  ///< lowest battery level
    double max_battery_pct;  ///< highest battery level
    int busy_count;          ///< robots carrying out a task
};

/**
 * @brief Lowest and highest battery level, and how many robots are busy.
 * @param fleet The robots. Must not be empty.
 * @return The three values in a FleetSummary.
 */
FleetSummary summarize_fleet(const std::vector<RobotStatus>& fleet);

/**
 * @brief The first idle robot with enough battery.
 * @param fleet The robots, in the order to search them.
 * @param min_battery_pct The lowest battery level that may take a task.
 * @return Its id, or an empty optional when no robot qualifies.
 */
std::optional<int> find_idle_robot(const std::vector<RobotStatus>& fleet,
                                   double min_battery_pct);

/**
 * @brief How many robots are below a battery level.
 * @param fleet The robots.
 * @param limit_pct The battery level, in percent.
 * @return The number of robots with @c battery_pct below @p limit_pct.
 */
int count_low_battery(const std::vector<RobotStatus>& fleet, double limit_pct);

/**
 * @brief The idle robot with enough battery that is closest to a pickup point.
 * @param fleet The robots.
 * @param pickup Where the task starts.
 * @param min_battery_pct The lowest battery level that may take a task.
 * @return Its id, or an empty optional when no robot qualifies.
 */
std::optional<int> find_closest_idle(const std::vector<RobotStatus>& fleet,
                                     Position pickup, double min_battery_pct);
