/**
 * @file classes.cpp
 * @brief L8 Section 1, Classes and Objects: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * The slides of this section use a first version of CruiseControl, with no
 * constructor and no check. It is defined here, in namespace first_version.
 * The full class is in ../../common/include/cruise_control.hpp.
 */
#include <iostream>
#include <memory>

#include "sections.hpp"

namespace classes {

// [Slide 7] Class, and [Slide 8] The Parts of a Class
namespace first_version {
class CruiseControl {
 public:
  void set_target_speed(double speed);
  double get_target_speed() const;

 private:
  double speed_limit_{13.4};  // m/s
  double target_speed_{0.0};  // m/s
};

// No check yet: the first version takes any speed.
void CruiseControl::set_target_speed(double speed) { target_speed_ = speed; }

double CruiseControl::get_target_speed() const { return target_speed_; }
}  // namespace first_version

using first_version::CruiseControl;

// [Slide 10] State and Behavior
namespace two_objects {
void run() {
  CruiseControl ego_cruise;
  CruiseControl test_cruise;
  ego_cruise.set_target_speed(12.0);
  test_cruise.set_target_speed(10.0);
  std::cout << ego_cruise.get_target_speed() << ' '
            << test_cruise.get_target_speed() << '\n';  // 12 10
  std::cout << sizeof(CruiseControl) << '\n';            // 16
}
}  // namespace two_objects

// [Slide 13] On the Stack and on the Heap
namespace stack_and_heap {
void run() {
  CruiseControl ego_cruise;  // on the stack
  ego_cruise.set_target_speed(12.0);
  std::cout << ego_cruise.get_target_speed() << '\n';  // 12

  auto test_cruise{std::make_unique<CruiseControl>()};  // on the heap
  test_cruise->set_target_speed(10.0);
  std::cout << test_cruise->get_target_speed() << '\n';  // 10
}
}  // namespace stack_and_heap

std::vector<Slide> slides() {
  return {
      {10, "State and Behavior", two_objects::run},
      {13, "On the Stack and on the Heap", stack_and_heap::run},
  };
}

}  // namespace classes
