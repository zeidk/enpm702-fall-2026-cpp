/**
 * @file undefined.hpp
 * @brief L8: the code of the reading page Rule of Five and Rule of Zero
 *        that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details undefined.cpp is always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer. AddressSanitizer reports a double free as it
 * happens; its LeakSanitizer reports leaks when the program ends. The rest of
 * week8_appendix is compiled without the checks.
 *
 * Each run() is listed with run_alone set, so a full run skips it:
 * @code
 * 702run week8_appendix double_free  # [Reading: double_free] Double Free, Measured
 * @endcode
 */
#pragma once

namespace undefined {

/// [Reading: no_destructor] Without the Destructor: three Vehicle objects that never delete.
namespace no_destructor {
void run();
}

/// [Reading: double_free] Double Free, Measured: a copied Vehicle deletes its Sensor twice.
namespace double_free {
void run();
}

/// [Reading: move_bug] Discussion: Who Deletes What? A move assignment that leaves
/// the source owning.
namespace move_bug {
void run();
}

/// [Reading: no_move] Without the Moves: a Vehicle with a destructor and no
/// moves, so std::move copies it, and the copy deletes the Sensor twice.
namespace no_move {
void run();
}

}  // namespace undefined
