/**
 * @file b.hpp
 * @brief A class B that refers back to its A: the other side of the two-way
 *        association (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details a_ is a pointer to A, so this header needs only the name A: a
 * forward declaration. b.cpp includes a.hpp, because link calls a method of A.
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#pragma once

class A;  // the name only

class B {
 public:
  void link(A* a);

 private:
  A* a_{nullptr};
};
