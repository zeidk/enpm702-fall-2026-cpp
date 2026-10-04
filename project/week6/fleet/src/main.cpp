/**
 * @file main.cpp
 * @brief The fleet manager: the program the L6 slides build, in one piece.
 *
 * @details Build target: @c week6_fleet. It prints the fleet, a summary, the
 * robot chosen for one task, and the commands sent to two robots. Every
 * function it calls is checked by the tests in @c ../../tests/test_fleet.cpp.
 */
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "dispatcher.hpp"
#include "fleet_queries.hpp"
#include "robot.hpp"
#include "stats.hpp"

int main() {
    const std::vector<RobotStatus> fleet{make_demo_fleet()};

    std::cout << "== Fleet ==\n";
    for (const auto& r : fleet) {
        std::cout << "robot " << r.id << ": " << r.battery_pct << " % at (" << r.position.x
                  << ", " << r.position.y << ")" << (r.busy ? ", busy\n" : ", idle\n");
    }

    std::cout << "== Summary ==\n";
    auto [min_pct, max_pct, busy_count] = summarize_fleet(fleet);
    std::vector<double> levels{};
    for (const auto& r : fleet) {
        levels.push_back(r.battery_pct);
    }
    std::cout << "battery from " << min_pct << " % to " << max_pct << " %, average "
              << average_of(levels) << " %\n";
    std::cout << busy_count << " busy, " << count_low_battery(fleet, 40.0) << " below 40 %\n";

    std::cout << "== Task at (5, 5) ==\n";
    const Position pickup{5.0, 5.0};
    const double min_battery_pct{40.0};
    const std::optional<int> id{find_closest_idle(fleet, pickup, min_battery_pct)};
    if (id) {
        log_message("task assigned to robot " + std::to_string(*id));
    } else {
        log_message("no robot can take the task");
    }

    std::cout << "== Commands ==\n";
    const CommandTable commands{make_command_table()};
    dispatch(commands, "dock", 4);
    dispatch(commands, "pause", 2);
    if (!dispatch(commands, "reboot", 3)) {
        log_message("unknown command: reboot");
    }
}
