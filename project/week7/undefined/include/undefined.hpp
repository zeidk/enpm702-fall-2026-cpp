/**
 * @file undefined.hpp
 * @brief L7: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer. AddressSanitizer reports a double delete or a
 * read of freed memory as it happens; its LeakSanitizer reports leaks when the
 * program ends. The rest of week7_playground and week7_appendix is compiled
 * without the checks.
 *
 * Each run() is listed with run_alone set, so a full run skips it:
 * @code
 * 702run week7_playground 78    # [Slide 78] The Cycle, Measured
 * @endcode
 */
#pragma once

namespace undefined {

/// [Appendix ii] Double Delete, Measured: two lists delete the same Obstacle.
namespace double_delete {
void run();
}

/// [Appendix iii] Dangling Read, Measured: one list deletes, the other reads.
namespace dangling_read {
void run();
}

/// [Slide 78] The Cycle, Measured: a tractor and its trailer that own each other.
namespace shared_cycle {
void run();
}

}  // namespace undefined
