/**
 * @file classes.cpp
 * @brief L8, Classes and Objects: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The appendix frame of this section, State and
 * Behavior, is listed in appendix_slides(), which week8_appendix runs.
 *
 * The slides of this section use a first version of Vehicle, with no
 * constructor and no check. It is defined here, in namespace first_version.
 * The full class is in ../../common/include/vehicle.hpp.
 *
 * The Live Coding slides (Step 1 and Your Turn) have no code here: we write
 * that code together in ../../live/src/main.cpp (week8_live).
 */
#include <iostream>
#include <stdexcept>
#include <string>

#include "sections.hpp"

namespace classes {

// [Slide 9] Two Ways to Write the Same Program
// The same speed, written the procedural way (a struct and a function) and the
// object-oriented way (a class). Both set_speed functions check the speed.
namespace two_ways {
struct VehicleData {
  double max_speed{13.4};
  double speed{0.0};
};
void set_speed(VehicleData& car,
               double speed);

class Vehicle {
 public:
  void set_speed(double speed);
 private:
  double max_speed_{13.4};
  double speed_{0.0};
};

void set_speed(VehicleData& car, double speed) {
  if (speed < 0.0 || speed > car.max_speed) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  car.speed = speed;
}

void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

void run() {
  VehicleData car_a;
  car_a.speed = -50.0;  // compiles
  std::cout << car_a.speed << '\n';  // -50: the check of set_speed never ran

  Vehicle car_b;
  // car_b.speed_ = -50.0;  // error
  // error: 'double classes::two_ways::Vehicle::speed_' is private within this context
  // (the slide's code has no namespaces, so the slide's error names Vehicle only)
  car_b.set_speed(12.0);  // the only way to change speed_: the check runs
}
}  // namespace two_ways

// [Slide 15] The Parts of a Class: the first version of Vehicle, used by the
// slides of this section.
namespace first_version {
class Vehicle {  // a first version
 public:
  void set_speed(double speed);
  double get_speed() const;

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
  std::string color_{"white"};
  double mileage_{0.0};     // m driven
};

// No check yet: the first version takes any speed.
void Vehicle::set_speed(double speed) { speed_ = speed; }

double Vehicle::get_speed() const { return speed_; }

void run() {
  Vehicle car_a;
  std::cout << car_a.get_speed() << '\n';  // 0: the default value of speed_
}
}  // namespace first_version

using first_version::Vehicle;

// [Slide 16] Object: two objects of one class, each with its own data members.
namespace object {
void run() {
  Vehicle car_a;
  Vehicle car_b;
  std::cout << car_a.get_speed() << ' '
            << car_b.get_speed() << '\n';  // 0 0
}
}  // namespace object

// [Appendix ii] State and Behavior
namespace two_objects {
void run() {
  Vehicle car_a;
  Vehicle car_b;

  car_a.set_speed(12.0);
  car_b.set_speed(10.0);

  std::cout << car_a.get_speed() << ' '
    << car_b.get_speed() << '\n';  // 12 10
  std::cout << sizeof(Vehicle) << '\n';    // 56
}
}  // namespace two_objects

std::vector<Slide> slides() {
  return {
      {9, "Two Ways to Write the Same Program", two_ways::run},
      {15, "The Parts of a Class", first_version::run},
      {16, "Object", object::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
      {2, "State and Behavior", two_objects::run},
  };
}

}  // namespace classes
