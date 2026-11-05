/**
 * @file calls.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        What an Operator Overload Is.
 * @author Zeid Kootbally
 *
 * @details Each pair of lines prints the same text twice: once with the
 * operator, once by calling the operator function by its name.
 * Run it: 702run week9_operators calls
 */
#include <iostream>
#include <string>

#include "parts.hpp"

namespace calls {
void run() {
  const std::string kind{"robo"};
  const std::string vehicle{"taxi"};
  std::cout << kind + vehicle << '\n';            // a free function of <string>
  std::cout << operator+(kind, vehicle) << '\n';  // the same call, by its name
  std::cout << 500 << '\n';                       // a member of std::ostream
  std::cout.operator<<(500) << '\n';              // the same call, by its name
  std::cout << "m\n";                             // a free function of <ostream>
  operator<<(std::cout, "m\n");                   // the same call, by its name
}
}  // namespace calls
