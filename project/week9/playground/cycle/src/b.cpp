/**
 * @file b.cpp
 * @brief B's method. It calls A::set_b, and a call needs the full class, so
 *        b.cpp includes a.hpp.
 * @author Zeid Kootbally
 */
// [Appendix xxxix] A Forward Declaration Breaks the Cycle
#include "b.hpp"

#include "a.hpp"  // a call needs the full class

void B::link(A* a) {
  a_ = a;
  a->set_b(this);
}
