/**
 * @file by_value.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        Mistake: Returning the Stream by Value.
 * @author Zeid Kootbally
 *
 * @details An operator<< that returns std::ostream, not std::ostream&. It is
 * commented out: it does not compile, because a stream cannot be copied.
 * Run it: 702run week9_operators by_value
 */
#include <iostream>

#include "parts.hpp"

namespace by_value {
struct Position {
  double x{0.0};
  double y{0.0};
};

// std::ostream operator<<(std::ostream& out, const Position& p) {  // no &
//   out << '(' << p.x << ", " << p.y << ')';
//   return out;  // does not compile: this line copies the stream
// }

void run() { print_commented_out("operators/src/by_value.cpp"); }
}  // namespace by_value
