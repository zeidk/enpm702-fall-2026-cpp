/**
 * @file b.hpp
 * @brief A class B that refers back to its A: the other side of the two-way
 *        association (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details a_ is a std::weak_ptr<A>, so this header needs only the name A: a
 * forward declaration. The #include "a.hpp" is in b.cpp.
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#pragma once
#include <memory>
class A;  // the name only
class B {
 public:
  void link(const std::shared_ptr<A>& a);
 private:
  std::weak_ptr<A> a_;
};
