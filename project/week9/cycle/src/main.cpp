/**
 * @file main.cpp
 * @brief Two headers that need each other, built with forward declarations
 *        (L9, the cyclic include): target week9_cycle.
 * @author Zeid Kootbally
 *
 * @details A B links to an A, and link adds the B to the A's list: a two-way
 * association. The program prints "built". The version that includes each
 * header from the other, and its errors, is in a comment in ../CMakeLists.txt.
 * Run it: 702run week9_cycle
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#include <iostream>
#include <memory>

#include "a.hpp"
#include "b.hpp"

int main() {
  auto a{std::make_shared<A>()};
  B b;
  b.link(a);  // a now lists b
  std::cout << "built\n";
}
