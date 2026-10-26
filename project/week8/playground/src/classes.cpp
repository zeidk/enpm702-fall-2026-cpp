/**
 * @file classes.cpp
 * @brief L8 Section 1, Classes and Objects: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * The slides of this section use a first version of Vehicle, with no
 * constructor and no check. It is defined here, in namespace first_version.
 * The full class is in ../../common/include/vehicle.hpp.
 */
#include <iostream>
#include <memory>

#include "sections.hpp"

namespace classes {

// [Slide 8] Class, and [Slide 9] The Parts of a Class
namespace first_version {
class Vehicle {
 public:
  void set_speed(double speed);
  double get_speed() const;

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

// No check yet: the first version takes any speed.
void Vehicle::set_speed(double speed) { speed_ = speed; }

double Vehicle::get_speed() const { return speed_; }
}  // namespace first_version

using first_version::Vehicle;

// [Slide 11] State and Behavior
namespace two_objects {
void run() {
  Vehicle downtown_car;
  Vehicle airport_car;
  downtown_car.set_speed(12.0);
  airport_car.set_speed(10.0);
  std::cout << downtown_car.get_speed() << ' '
            << airport_car.get_speed() << '\n';  // 12 10
  std::cout << sizeof(Vehicle) << '\n';          // 16
}
}  // namespace two_objects

// [Slide 14] On the Stack and on the Heap
namespace stack_and_heap {
void run() {
  Vehicle downtown_car;  // on the stack
  downtown_car.set_speed(12.0);
  std::cout << downtown_car.get_speed() << '\n';  // 12

  auto airport_car{std::make_unique<Vehicle>()};  // on the heap
  airport_car->set_speed(10.0);
  std::cout << airport_car->get_speed() << '\n';  // 10
}
}  // namespace stack_and_heap

std::vector<Slide> slides() {
  return {
      {11, "State and Behavior", two_objects::run},
      {14, "On the Stack and on the Heap", stack_and_heap::run},
  };
}

}  // namespace classes
