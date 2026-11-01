/**
 * @file parts.hpp
 * @brief The parts of the L9 reading page Operator Overloading, on Read the
 *        Docs: one namespace and one run() per part.
 * @author Zeid Kootbally
 *
 * @details Each part has its own source file in ../src/, named after its key,
 * and its own namespace. Run a part by its key:
 * @code
 * 702run week9_operators print
 * @endcode
 */
#pragma once

#include <string_view>

namespace calls {          ///< What an Operator Overload Is
void run();
}
namespace print {          ///< operator<< for Position
void run();
}
namespace compare {        ///< operator< and operator> for Route
void run();
}
namespace member_output {  ///< Mistake: operator<< as a Member Function
void run();
}
namespace by_value {       ///< Mistake: Returning the Stream by Value
void run();
}
namespace no_const {       ///< Mistake: A Comparison Without const
void run();
}

/**
 * @brief Says where the code of a part is, when that code does not compile.
 *
 * Such code stays commented out, so the program builds. The run of its part
 * calls this function.
 *
 * @param file The file that holds the commented-out code, such as
 *             "operators/src/by_value.cpp".
 */
void print_commented_out(std::string_view file);
