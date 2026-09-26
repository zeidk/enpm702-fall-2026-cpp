/**
 * @file joint_limits.cpp
 * @brief Definition of clamp_joint.
 */

#include "joint_limits.hpp"

#include <algorithm>

double clamp_joint(double deg) { return std::clamp(deg, -max_deg, max_deg); }
