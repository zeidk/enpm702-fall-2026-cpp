/**
 * @file a.hpp
 * @brief A class A that refers to one B: one side of a two-way association
 *        (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details b_ is a pointer to B, so this header needs only the name B: a
 * forward declaration. a.cpp does not include b.hpp either: set_b stores the
 * address of a B and calls none of its methods.
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#pragma once

class B;  // the name only

class A {
 public:
  void set_b(B* b);

 private:
  B* b_{nullptr};
};
