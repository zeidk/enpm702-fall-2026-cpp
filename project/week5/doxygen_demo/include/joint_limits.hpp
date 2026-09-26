#pragma once

/**
 * @file joint_limits.hpp
 * @brief The joint limit of the arm, and the function that enforces it.
 */

/// The largest angle, in degrees, that any joint may be driven to.
constexpr double max_deg{170.0};

/**
 * @brief Clamp a joint angle to the arm's limit.
 * @param deg The angle in degrees. Any finite value.
 * @return deg when it is between -max_deg and max_deg, otherwise the nearer
 *         of the two limits.
 */
[[nodiscard]] double clamp_joint(double deg);
