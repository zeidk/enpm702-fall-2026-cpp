/**
 * @file undefined.hpp
 * @brief L8: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer. AddressSanitizer reports a double free as it
 * happens; its LeakSanitizer reports leaks when the program ends. The rest of
 * week8_playground is compiled without the checks.
 *
 * Each run() is listed with run_alone set, so a full run skips it:
 * @code
 * 702run week8_playground 64    # [Slide 64] Double Free, Measured
 * @endcode
 */
#pragma once

namespace undefined {

/// [Slide 62] Without the Destructor: three Track objects that never delete.
namespace no_destructor {
void run();
}

/// [Slide 64] Double Free, Measured: a copied Track deletes its Obstacle twice.
namespace double_free {
void run();
}

/// [Slide 69] Discussion: Who Deletes What? A move assignment that leaves the source owning.
namespace move_bug {
void run();
}

}  // namespace undefined
