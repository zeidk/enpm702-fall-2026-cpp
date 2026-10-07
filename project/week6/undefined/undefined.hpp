/**
 * @file undefined.hpp
 * @brief L6: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer, so the
 * dangling capture stops with a report that names the bug and the line. The
 * rest of week6_playground is compiled without it.
 *
 * Each slide that calls one of these is listed with run_alone set, so a full
 * run skips it:
 * @code
 * 702run week6_playground 35    # [Slide 35] Three Ways to Read
 * 702run week6_playground 64    # [Slide 64] A Dangling Capture
 * @endcode
 */
#pragma once

namespace undefined {

/// [Slide 35] Three Ways to Read: *idle on an empty optional.
namespace optional_star {
void run();
}

/// [Slide 64] A Dangling Capture: a lambda that captures a dead parameter by
/// reference.
namespace dangling_capture {
void run();
}

}  // namespace undefined
