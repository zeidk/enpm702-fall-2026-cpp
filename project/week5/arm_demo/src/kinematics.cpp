/**
 * @file kinematics.cpp
 * @brief Definitions of the functions declared in kinematics.hpp.
 */

#include "kinematics.hpp"
#include "joint_limits.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>

/// Length of the first link, from the base to the elbow, in meters.
constexpr double link1_m{0.50};
/// Length of the second link, from the elbow to the wrist, in meters.
constexpr double link2_m{0.40};
/// Length of the third link, from the wrist to the tool, in meters.
constexpr double link3_m{0.30};

double convert_deg_to_rad(double deg) { return deg * std::numbers::pi / 180.0; }

// The default for limit is written in the header, never here. The comment
// keeps the value visible to whoever reads the definition.
void forward_kinematics(double q1, double q2, double q3,
                        double& x, double& y, double limit /* = max_deg */) {
    const double t1{convert_deg_to_rad(std::clamp(q1, -limit, limit))};
    const double t2{convert_deg_to_rad(std::clamp(q2, -limit, limit))};
    const double t3{convert_deg_to_rad(std::clamp(q3, -limit, limit))};

    // x and y are reference parameters: the function writes into the caller's
    // two variables instead of returning one value.
    x = link1_m * std::cos(t1) + link2_m * std::cos(t1 + t2) +
        link3_m * std::cos(t1 + t2 + t3);
    y = link1_m * std::sin(t1) + link2_m * std::sin(t1 + t2) +
        link3_m * std::sin(t1 + t2 + t3);
}

void print_pose(double x, double y, int precision /* = 3 */,
                std::string_view label /* = "tool" */) {
    std::cout << label << ": x = " << std::fixed << std::setprecision(precision)
              << x << " m, y = " << y << " m\n";
}
