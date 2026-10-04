/**
 * @file test_fleet.cpp
 * @brief Tests for the fleet manager in ../fleet.
 *
 * @details Every test uses the demo fleet from the slides:
 *
 * | id | battery | position | state |
 * |----|---------|----------|-------|
 * | 1  | 82.5 %  | (0, 0)   | idle  |
 * | 2  | 35.0 %  | (4, 1)   | busy  |
 * | 3  | 64.0 %  | (2, 3)   | idle  |
 * | 4  | 18.0 %  | (6, 2)   | idle  |
 *
 * Build: 702build week6_tests. Run them all: 702test week6.
 */
#include <gtest/gtest.h>

#include <optional>
#include <string>
#include <vector>

#include "dispatcher.hpp"
#include "fleet_queries.hpp"
#include "stats.hpp"

// --- Section 2: Several Results, or None ------------------------------------

TEST(FleetQueries, BatteryRange) {
    auto [lo, hi] = find_battery_range(make_demo_fleet());
    EXPECT_DOUBLE_EQ(lo, 18.0);  // robot 4
    EXPECT_DOUBLE_EQ(hi, 82.5);  // robot 1
}

TEST(FleetQueries, Summary) {
    const FleetSummary s{summarize_fleet(make_demo_fleet())};
    EXPECT_DOUBLE_EQ(s.min_battery_pct, 18.0);
    EXPECT_DOUBLE_EQ(s.max_battery_pct, 82.5);
    EXPECT_EQ(s.busy_count, 1);  // robot 2
}

TEST(FleetQueries, IdleRobotFound) {
    // Robot 1 is idle with 82.5 %, and comes first.
    const std::optional<int> id{find_idle_robot(make_demo_fleet(), 50.0)};
    ASSERT_TRUE(id.has_value());  // stop here if empty: *id below would be undefined
    EXPECT_EQ(*id, 1);
}

TEST(FleetQueries, IdleRobotNone) {
    // No robot has 90 %.
    EXPECT_FALSE(find_idle_robot(make_demo_fleet(), 90.0).has_value());
}

TEST(FleetQueries, IdleRobotSkipsBusy) {
    // Robot 2 (35 %, busy) is skipped; robot 4 (18 %) is too low; robot 3 qualifies.
    std::vector<RobotStatus> fleet{make_demo_fleet()};
    fleet[0].busy = true;  // take robot 1 out
    EXPECT_EQ(find_idle_robot(fleet, 30.0).value_or(-1), 3);
}

// --- Section 3: Function Templates ------------------------------------------

TEST(Stats, ClampValue) {
    EXPECT_EQ(clamp_value(130, 0, 100), 100);              // T is int
    EXPECT_DOUBLE_EQ(clamp_value(104.2, 0.0, 100.0), 100.0);  // T is double
    EXPECT_EQ(clamp_value(50, 0, 100), 50);
}

TEST(Stats, AverageOf) {
    EXPECT_NEAR(average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0}), 49.875, 1e-9);
    EXPECT_NEAR(average_of(std::vector<double>{}), 0.0, 1e-9);
}

// --- Section 4: Lambdas -----------------------------------------------------

TEST(FleetQueries, CountLowBattery) {
    EXPECT_EQ(count_low_battery(make_demo_fleet(), 40.0), 2);  // robots 2 and 4
    EXPECT_EQ(count_low_battery(make_demo_fleet(), 10.0), 0);
}

TEST(FleetQueries, ClosestIdle) {
    // Pickup at (5, 5). Robot 4 is closest (3.16 m) but has 18 %. Of the idle
    // robots with at least 40 %, robot 3 (3.61 m) is closer than robot 1 (7.07 m).
    EXPECT_EQ(find_closest_idle(make_demo_fleet(), {5.0, 5.0}, 40.0).value_or(-1), 3);
    EXPECT_EQ(find_closest_idle(make_demo_fleet(), {5.0, 5.0}, 10.0).value_or(-1), 4);
    EXPECT_FALSE(find_closest_idle(make_demo_fleet(), {5.0, 5.0}, 90.0).has_value());
}

// --- Section 5: Other Callables ---------------------------------------------

TEST(Dispatcher, KnownCommandRuns) {
    int called_with{0};
    CommandTable table{};
    // The handler captures called_with by reference, so the test can see the call.
    table["dock"] = [&called_with](int id) { called_with = id; };
    EXPECT_TRUE(dispatch(table, "dock", 4));
    EXPECT_EQ(called_with, 4);
}

TEST(Dispatcher, UnknownCommandIsRefused) {
    const CommandTable table{make_command_table()};
    EXPECT_FALSE(dispatch(table, "reboot", 3));
    EXPECT_EQ(table.count("reboot"), 0u);  // find, not operator[]: nothing inserted
}
