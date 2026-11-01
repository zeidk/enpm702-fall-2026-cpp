/**
 * @file b.cpp
 * @brief The method of B (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details link calls A::add, and a call needs the full class, so this file
 * includes a.hpp. Without it, g++ 13.3 stops with
 * "invalid use of incomplete type ... 'class A'".
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#include "b.hpp"
#include "a.hpp"  // a call needs the full class
void B::link(const std::shared_ptr<A>& a) {
  a_ = a;
  a->add(*this);
}
