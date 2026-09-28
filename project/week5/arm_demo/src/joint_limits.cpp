/**
 * @file joint_limits.cpp
 * @brief Definition of clamp_joint.
 */

#include "joint_limits.hpp"

#include <algorithm>

// The default for limit is written in the header, never here.
double clamp_joint(double deg, double limit /* = max_deg */) {
    return std::clamp(deg, -limit, limit);
}
