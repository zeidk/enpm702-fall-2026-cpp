/**
 * @file member_output.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        Mistake: operator<< as a Member Function.
 * @author Zeid Kootbally
 *
 * @details A Position whose operator<< is a member function. The object is
 * then the left operand, so only position << std::cout compiles. The line
 * std::cout << end is commented out: it does not compile.
 * Run it: 702run week9_operators member_output
 */
#include <iostream>

#include "parts.hpp"

namespace member_output {
struct Position {
  double x{0.0};
  double y{0.0};
  std::ostream& operator<<(std::ostream& out) const;  // a member: wrong
};

std::ostream& Position::operator<<(std::ostream& out) const {
  out << '(' << x << ", " << y << ')';
  return out;
}

void run() {
  const Position end{300.0, 400.0};
  // std::cout << end << '\n';  // does not compile
  end << std::cout;  // compiles: end.operator<<(std::cout)
  std::cout << '\n';
}
}  // namespace member_output
