/**
 * @file undefined.hpp
 * @brief L9: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer, so each run stops with a report. The rest of
 * week9_playground is compiled without the checks.
 *
 * Each slide is listed with run_alone set, so a full run skips it:
 * @code
 * 702run week9_playground 76    # [Slide 76] Deleting through a Base Pointer
 * @endcode
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace undefined {

/// The slides of this file, each with run_alone set.
std::vector<Slide> slides();

}  // namespace undefined
