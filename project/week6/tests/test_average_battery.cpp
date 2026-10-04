/**
 * @file test_average_battery.cpp
 * @brief The first tests of L6, from the Testing section at the start of the
 *        lecture.
 *
 * @details Build: 702build week6_tests. Run: 702test week6.AverageBattery
 */
#include <gtest/gtest.h>

#include "fleet_queries.hpp"

// [Slide 7] A First Test
TEST(AverageBattery, FourRobots) {
    // (82.5 + 35.0 + 64.0 + 18.0) / 4 = 199.5 / 4 = 49.875
    EXPECT_NEAR(average_battery({82.5, 35.0, 64.0, 18.0}), 49.875, 1e-9);
}

// [Slide 7] A First Test
TEST(AverageBattery, EmptyFleet) {
    // An empty fleet has no average. The function returns 0.0 instead of
    // dividing by zero.
    EXPECT_NEAR(average_battery({}), 0.0, 1e-9);
}
