/**
 * @file main.cpp
 * @brief The fleet manager: the program the L6 slides build, in one piece.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_fleet. It prints the fleet, a summary, the
 * robot chosen for one task, and the commands sent to two robots.
 */
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "dispatcher.hpp"
#include "fleet_queries.hpp"
#include "robot.hpp"
#include "stats.hpp"

/**
 * @brief Runs the fleet manager on the demo fleet.
 *
 * Prints the fleet, a summary of it, the robot chosen for a task at (5, 5),
 * and the result of three commands, one of them unknown.
 *
 * @return 0.
 */
int main() {
    const std::vector<RobotStatus> fleet{make_demo_fleet()};
    const std::string rule(44, '-');
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(44, '=') << "\n  Fleet Manager\n" << std::string(44, '=') << '\n';

    std::cout << "\nFleet\n" << rule << '\n';
    for (const auto& robot : fleet) {
        std::cout << "  robot " << robot.id << " : " << std::setw(6) << robot.battery_pct << " %  at ("
                  << robot.position.x << ", " << robot.position.y << ")  "
                  << (robot.busy ? "busy" : "idle") << '\n';
    }

    std::cout << "\nSummary\n" << rule << '\n';
    auto [min_pct, max_pct, busy_count] = summarize_fleet(fleet);
    std::vector<double> levels{};
    for (const auto& robot : fleet) {
        levels.push_back(robot.battery_pct);
    }
    std::cout << "  lowest battery  : " << std::setw(6) << min_pct << " %\n";
    std::cout << "  highest battery : " << std::setw(6) << max_pct << " %\n";
    std::cout << "  average battery : " << std::setw(6) << average_of(levels) << " %\n";
    std::cout << "  busy            : " << std::setw(3) << busy_count << '\n';
    std::cout << "  below 40 %      : " << std::setw(3) << count_low_battery(fleet, 40.0) << '\n';

    std::cout << "\nTask at (5, 5)\n" << rule << '\n';
    const Position pickup{5.0, 5.0};
    const double min_battery_pct{40.0};
    const std::optional<int> id{find_closest_idle(fleet, pickup, min_battery_pct)};
    if (id) {
        log_message("task assigned to robot " + std::to_string(*id));
    } else {
        log_message("no robot can take the task");
    }

    std::cout << "\nCommands\n" << rule << '\n';
    const CommandTable commands{make_command_table()};
    dispatch(commands, "dock", 4);
    dispatch(commands, "pause", 2);
    if (!dispatch(commands, "reboot", 3)) {
        log_message("unknown command: reboot");
    }
}
