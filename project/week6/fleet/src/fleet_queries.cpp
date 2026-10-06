/**
 * @file fleet_queries.cpp
 * @brief Definitions for fleet_queries.hpp.
 * @author Zeid Kootbally
 */
#include "fleet_queries.hpp"

#include <algorithm>
#include <cmath>

std::vector<RobotStatus> make_demo_fleet() {
    return {{1, 82.5, {0.0, 0.0}, false},
            {2, 35.0, {4.0, 1.0}, true},
            {3, 64.0, {2.0, 3.0}, false},
            {4, 18.0, {6.0, 2.0}, false}};
}

std::pair<double, double> find_battery_range(const std::vector<RobotStatus>& fleet) {
    double min_pct{fleet.front().battery_pct};
    double max_pct{fleet.front().battery_pct};
    for (const auto& robot : fleet) {
        if (robot.battery_pct < min_pct) { min_pct = robot.battery_pct; }
        if (robot.battery_pct > max_pct) { max_pct = robot.battery_pct; }
    }
    return {min_pct, max_pct};
}

FleetSummary summarize_fleet(const std::vector<RobotStatus>& fleet) {
    FleetSummary summary{fleet.front().battery_pct, fleet.front().battery_pct, 0};
    for (const auto& robot : fleet) {
        if (robot.battery_pct < summary.min_battery_pct) { summary.min_battery_pct = robot.battery_pct; }
        if (robot.battery_pct > summary.max_battery_pct) { summary.max_battery_pct = robot.battery_pct; }
        if (robot.busy) { ++summary.busy_count; }
    }
    return summary;
}

std::optional<int> find_idle_robot(const std::vector<RobotStatus>& fleet,
                                   double min_battery_pct) {
    for (const auto& robot : fleet) {
        if (!robot.busy && robot.battery_pct >= min_battery_pct) {
            return robot.id;
        }
    }
    return std::nullopt;
}

int count_low_battery(const std::vector<RobotStatus>& fleet, double limit_pct) {
    // Captured by reference: the lambda is used here and now (Core Guidelines F.52).
    auto count = std::count_if(fleet.begin(), fleet.end(), [&limit_pct](const RobotStatus& robot) {
        return robot.battery_pct < limit_pct;
    });
    return static_cast<int>(count);
}

std::optional<int> find_closest_idle(const std::vector<RobotStatus>& fleet,
                                     Position pickup, double min_battery_pct) {
    std::vector<RobotStatus> candidates{};
    for (const auto& robot : fleet) {
        if (!robot.busy && robot.battery_pct >= min_battery_pct) {
            candidates.push_back(robot);
        }
    }
    if (candidates.empty()) {
        return std::nullopt;
    }
    // A projection: compare the robots by their distance to the pickup point.
    auto closest = std::ranges::min_element(candidates, {}, [pickup](const RobotStatus& robot) {
        return std::hypot(robot.position.x - pickup.x, robot.position.y - pickup.y);
    });
    return closest->id;
}
