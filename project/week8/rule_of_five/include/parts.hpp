/**
 * @file parts.hpp
 * @brief The parts of the L8 reading page Rule of Five and Rule of Zero, on
 *        Read the Docs: one namespace and one run() per part.
 * @author Zeid Kootbally
 *
 * @details Each part has its own source file in ../src/, named after its key,
 * and its own namespace, so that its class Vehicle does not clash with the
 * Vehicle of another part. Run a part by its key:
 * @code
 * 702run week8_rule_of_five five
 * @endcode
 */
#pragma once

namespace which_one {      ///< Which Function Runs
void run();
}
namespace zero {           ///< What the Compiler Writes Now
void run();
}
namespace raw_owner {      ///< A Class That Owns Memory
void run();
}
namespace no_destructor {  ///< Without the Destructor: a leak
void run();
}
namespace double_free {    ///< Copying an Owner: a double free
void run();
}
namespace five {           ///< The Copies, The Moves, All Five, Run
void run();
}
namespace move_noexcept {  ///< Why the Moves Are noexcept
void run();
}
namespace no_move {        ///< Without the Moves: a double free
void run();
}
namespace move_bug {       ///< Discussion: Who Deletes What? A double free
void run();
}
namespace move_only {      ///< = default and = delete
void run();
}
namespace file_owner {     ///< Beyond Memory: A File
void run();
}
