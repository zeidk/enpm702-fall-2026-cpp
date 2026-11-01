/**
 * @file main.cpp
 * @brief Two headers that need each other, built with forward declarations
 *        (L9, the cyclic include): target week9_cycle.
 * @author Zeid Kootbally
 *
 * @details A B links to an A, and link makes the A point back at the B: a
 * two-way association. The program prints "built". The version that includes each
 * header from the other, and its errors, is in a comment in ../CMakeLists.txt.
 * Run it: 702run week9_cycle
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#include <iostream>

#include "a.hpp"
#include "b.hpp"

int main() {
  A a;
  B b;
  b.link(&a);  // a and b now point at each other
  std::cout << "built\n";
}
