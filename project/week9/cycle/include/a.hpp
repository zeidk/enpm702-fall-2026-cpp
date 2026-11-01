/**
 * @file a.hpp
 * @brief A class A that keeps a list of B objects: one side of a two-way
 *        association (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details bs_ holds pointers to B, so this header needs only the name B: a
 * forward declaration. a.cpp does not include b.hpp either: add stores the
 * address of a B and calls none of its methods.
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#pragma once
#include <vector>
class B;  // the name only
class A {
 public:
  void add(const B& b);
 private:
  std::vector<const B*> bs_;
};
