/**
 * @file print.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        operator<< for Position.
 * @author Zeid Kootbally
 *
 * @details Prints the two ends of maya_ride, the 500 m route of the lecture,
 * with the operator<< of position.hpp.
 * Run it: 702run week9_operators print
 */
#include <iostream>

#include "parts.hpp"
#include "position.hpp"

// [Slide 65] Operators Are Overloaded Functions: the operator<< of
// position.cpp, then the two lines of run() that make end and print it.
namespace print {
void run() {
  const fleet::Position start{0.0, 0.0};
  const fleet::Position end{300.0, 400.0};
  std::cout << end << '\n';
  std::cout << "maya_ride: " << start << " to " << end << '\n';
  operator<<(std::cout, end) << '\n';  // std::cout << end, by its name
}
}  // namespace print
