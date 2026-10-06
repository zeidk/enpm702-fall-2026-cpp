#pragma once

/**
 * @file kinematics.hpp
 * @brief Angle conversion, forward kinematics and pose printing for the arm.
 */

#include <string_view>

#include "joint_limits.hpp"  // for max_deg

/**
 * @brief Convert an angle from degrees to radians.
 * @param deg The angle in degrees. Any finite value.
 * @return The same angle in radians.
 */
[[nodiscard]] double convert_deg_to_rad(double deg);

/**
 * @brief Compute where the tool of the three-joint planar arm ends up.
 *
 * Each angle is clamped to @p limit before it is used, so an angle past the
 * limit moves the arm to the limit instead of failing.
 *
 * @param q1 Shoulder angle in degrees, measured from the x axis.
 * @param q2 Elbow angle in degrees, relative to the previous link.
 * @param q3 Wrist angle in degrees, relative to the previous link.
 * @param x Set to the tool's x coordinate in meters, measured from the base at (0, 0).
 * @param y Set to the tool's y coordinate in meters, measured from the base at (0, 0).
 * @param limit The largest angle, in degrees, any joint may reach.
 */
void forward_kinematics(double q1, double q2, double q3,
                        double& x, double& y, double limit = max_deg);

/**
 * @brief Print a tool position on one line.
 * @param x The x coordinate in meters.
 * @param y The y coordinate in meters.
 * @param precision How many digits to print after the decimal point.
 * @param label The name printed in front of the coordinates.
 */
void print_pose(double x, double y, int precision = 3,
                std::string_view label = "tool");
