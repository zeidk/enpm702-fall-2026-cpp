/**
 * @file undefined.hpp
 * @brief L5: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer, so a run names the bug and the line. The rest
 * of week5_playground and week5_appendix is compiled without them, so their
 * addresses and timings are the ordinary ones.
 *
 * Each run() is listed with run_alone set, so a full run skips it:
 * @code
 * 702run week5_playground 31    # [Slide 31] Missing Returns
 * 702run week5_appendix xix     # [Appendix xix] Stack Overflow
 * @endcode
 */
#pragma once

namespace undefined {

/// [Slide 31] Missing Returns: get_sign(0) falls off the end.
namespace missing_return {
void run();
}

/// [Slide 55] Returning a Local: a reference to a destroyed local.
namespace returning_local {
void run();
}

/// [Slide 56] Returning a Parameter: a reference to a temporary that is gone.
namespace returning_parameter {
void run();
}

/// [Slide 58] Returning the Address of a Local: a pointer to a destroyed local.
namespace returning_address {
void run();
}

/// [Appendix xix] Stack Overflow: 21! does not fit in a long long, then
/// dig() has no base case.
namespace stack_overflow {
void run();
}

}  // namespace undefined
