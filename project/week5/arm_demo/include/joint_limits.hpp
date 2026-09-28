#pragma once

/**
 * @file joint_limits.hpp
 * @brief The joint limits of the arm, and the function that enforces them.
 */

/// The largest angle, in degrees, that any joint may be driven to.
constexpr double max_deg{170.0};

/// The limit of the shoulder, joint 1, in degrees.
constexpr double shoulder_max_deg{135.0};
/// The limit of the elbow, joint 2, in degrees.
constexpr double elbow_max_deg{90.0};
/// The limit of the wrist, joint 3, in degrees.
constexpr double wrist_max_deg{45.0};

/**
 * @brief Clamp a joint angle to a limit.
 * @param deg The angle in degrees. Any finite value.
 * @param limit The largest angle, in degrees, the joint may reach. Positive.
 * @return deg when it is between -limit and limit, otherwise the nearer of
 *         the two limits.
 */
[[nodiscard]] double clamp_joint(double deg, double limit = max_deg);
