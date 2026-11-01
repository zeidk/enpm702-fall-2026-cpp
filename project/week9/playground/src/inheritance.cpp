/**
 * @file inheritance.cpp
 * @brief L9 Section 2, Class Relationships: inheritance.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. RoboTaxi and Taxi derive from L8's
 * Vehicle (l8::Vehicle), which has no virtual method yet: that comes in
 * Section 3.
 *
 * The slides that do not compile are kept here as comments, with the error
 * g++ 13.3 gives (-std=c++20 -Wall -Wextra -pedantic-errors -Wshadow).
 */
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "automation_level.hpp"
#include "l8_vehicle.hpp"
#include "sections.hpp"

namespace inheritance {

using fleet::AutomationLevel;

// RoboTaxi and Taxi on L8's Vehicle, as Live Coding: Step 3 writes them. Their
// sizes are on [Appendix xxviii], Inside a Taxi Object, in
// ../../appendix/src/appendix.cpp, with copies of these two classes. Each has
// one method that Vehicle does not have, change_zone() and assign_driver(), for
// [Slide 55] and [Slide 54]. Each class declares it; its body is below the
// class, as it is in robotaxi.cpp and taxi.cpp in the program.
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void change_zone(std::string operating_zone);  // body below the class

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void RoboTaxi::change_zone(std::string operating_zone) {
  operating_zone_ = std::move(operating_zone);
}

// Taxi's constructor is also on [Slide 62], Base Class Initializer.
class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  void assign_driver(std::string driver_name);  // body below the class

 private:
  std::string driver_name_;  // empty: no driver
};

void Taxi::assign_driver(std::string driver_name) { driver_name_ = std::move(driver_name); }

// [Slide 52] A RoboTaxi in Memory. car_a holds every data member of Vehicle,
// all private: Vehicle's public methods read them. RoboTaxi adds
// operating_zone_, a std::string (32 bytes), and automation_level_, an
// AutomationLevel (4 bytes, as an int, then 4 bytes of padding): 88 + 40 = 128
// (g++ 13.3). Run alone, car_a's id is 1.
namespace robotaxi_memory {
void run() {
  RoboTaxi car_a{13.4};
  std::cout << car_a.get_id() << ' ' << car_a.get_max_speed() << ' '
            << car_a.get_speed() << ' ' << car_a.get_color() << ' '
            << car_a.get_mileage() << ' ' << car_a.sensor_count() << '\n';
  std::cout << sizeof(l8::Vehicle) << ' ' << sizeof(RoboTaxi) << '\n';  // 88 128
}
}  // namespace robotaxi_memory

// [Slide 53] A Taxi through a Pointer. taxi, on the stack, is one pointer (8
// bytes); the Taxi on the heap is its Vehicle part (88) and driver_name_ (32).
namespace taxi_pointer {
void run() {
  auto taxi{std::make_unique<Taxi>(13.4, "Ana")};
  std::cout << sizeof(taxi) << ' ' << sizeof(*taxi) << '\n';  // 8 120
  std::cout << taxi->get_color() << '\n';                     // the Vehicle part
}
}  // namespace taxi_pointer

// [Slide 54] A Base Class Reference to a Derived Object. No cast: vehicle is
// another name for the Vehicle part of taxi, its base subobject. The compiler
// checks a call through vehicle against Vehicle's members only, so
// assign_driver() is reachable only through taxi.
namespace base_ref {
void run() {
  Taxi taxi{13.4, "Ana"};
  taxi.assign_driver("Ben");   // a Taxi method
  l8::Vehicle& vehicle{taxi};  // no cast needed
  // vehicle.assign_driver("Ben");  // does not compile
  vehicle.set_speed(10.0);
  std::cout << taxi.get_speed() << '\n';  // 10: taxi itself changed
}
// Uncommented, the line gives:
// error: 'class l8::Vehicle' has no member named 'assign_driver'
}  // namespace base_ref

// [Slide 55] A Base Class Pointer to a Derived Object. No cast: pointer holds
// the address of the Vehicle part of robotaxi, then of taxi. As with a
// reference, change_zone() is reachable only through robotaxi.
namespace base_ptr {
void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.change_zone("Silver Spring");  // a RoboTaxi method
  l8::Vehicle* pointer{&robotaxi};        // no cast needed
  // pointer->change_zone("Silver Spring");  // does not compile
  pointer->set_speed(10.0);
  std::cout << robotaxi.get_speed() << '\n';  // 10
  Taxi taxi{13.4, "Ana"};
  pointer = &taxi;  // now taxi
}
// Uncommented, the line gives:
// error: 'class l8::Vehicle' has no member named 'change_zone'
}  // namespace base_ptr

// [Slide 56] Not the Other Way. Does not compile:
//   Taxi* back{pointer};
// error: invalid conversion from 'l8::Vehicle*' to 'inheritance::Taxi*' [-fpermissive]

// [Slide 57] protected Members. L8's Vehicle with one protected method:
// Vehicle's methods and the methods of RoboTaxi and Taxi can call log_event(),
// and other code cannot. Only the members this slide needs are written here.
// The class declares log_event(); its body is below the class, as it is in
// vehicle.cpp in the program.
namespace protected_member {
class Vehicle {  // L8's Vehicle, with one protected method
 public:     // any code
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}

 protected:  // Vehicle's, RoboTaxi's and Taxi's methods
  void log_event(const std::string& event) const;  // body in vehicle.cpp

 private:    // Vehicle's methods only
  inline static int next_id_{1};
  int id_;
  double max_speed_;
  std::string color_;
};

void Vehicle::log_event(const std::string& event) const {
  std::cout << "vehicle " << id_ << ": " << event << '\n';
}

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void start() const { log_event("starts"); }  // a method of a derived class: OK

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.start();  // vehicle 1: starts
}
// From main(), robotaxi.log_event("starts"); does not compile:
// error: 'void Vehicle::log_event(const std::string&) const' is protected within this context
}  // namespace protected_member

// [Slide 58] private Stays private. Does not compile:
//   void go() { speed_ = get_max_speed(); }  // inside RoboTaxi
// error: 'double l8::Vehicle::speed_' is private within this context
// The fix goes through Vehicle's public methods: [Slide 60].

// [Slide 59] protected Data. If speed_ were protected, a derived class could
// skip set_speed()'s check.
namespace protected_data {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }

 protected:
  double speed_{0.0};  // protected now: what the slide warns against

 private:
  double max_speed_;
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void speed_up() { speed_ = 20.0; }  // compiles: no check

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.speed_up();
  std::cout << robotaxi.get_speed() << ' ' << robotaxi.get_max_speed() << '\n';  // 20 13.4
}
}  // namespace protected_data

// [Slide 60] Through Vehicle's public Methods. speed_ stays private: RoboTaxi
// reads it with get_speed() and writes it with set_speed(), so the check runs.
namespace through_methods {
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void slow_down() { set_speed(get_speed() / 2.0); }  // read, then write
  void speed_up() { set_speed(20.0); }                 // checked: throws

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void run() {
  RoboTaxi car_a{13.4};
  car_a.set_speed(10.0);
  car_a.slow_down();
  std::cout << car_a.get_speed() << '\n';  // 5
  try {
    car_a.speed_up();
  } catch (const std::invalid_argument& e) {
    std::cout << e.what() << '\n';  // speed must be from 0 to the maximum speed
  }
  std::cout << car_a.get_speed() << '\n';  // 5: unchanged
}
}  // namespace through_methods

// [Slide 47] No Specifier Means private Inheritance. With "class RoboTaxi : Vehicle"
// (no public), the base is private, and main() cannot call get_speed():
// error: 'double Vehicle::get_speed() const' is inaccessible within this context
// error: 'Vehicle' is not an accessible base of 'RoboTaxi'

// [Not on a slide since 2026-11-01] Without the Base Class Initializer. Does
// not compile:
//   explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
//                     AutomationLevel automation_level = AutomationLevel::level_4)
//       : operating_zone_{std::move(operating_zone)},
//         automation_level_{automation_level} {}
// error: no matching function for call to 'Vehicle::Vehicle()'
// note: candidate: 'Vehicle::Vehicle(double, double)'
// note:   candidate expects 2 arguments, 0 provided
// note: candidate: 'Vehicle::Vehicle(double, const std::string&)'
// note:   candidate expects 2 arguments, 0 provided
// The notes after these are cut: the copy and move constructors are
// candidates too.

// A Base Member in the List is an appendix frame: [Appendix xiv], in
// ../../appendix/src/appendix.cpp.

// [Slide 63] Construction and Destruction Order. Both classes
// print from their constructor and destructor. (They declare a destructor and
// nothing else of the five: fine here, they own nothing.)
namespace order {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {
    std::cout << "Vehicle constructor\n";
  }
  ~Vehicle() { std::cout << "~Vehicle\n"; }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

 private:
  double max_speed_;
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {
    std::cout << "RoboTaxi constructor\n";
  }
  ~RoboTaxi() { std::cout << "~RoboTaxi\n"; }

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void run() {
  {
    RoboTaxi robotaxi{13.4};
    std::cout << "in the scope\n";
  }
  std::cout << "after the scope\n";
}
}  // namespace order

std::vector<Slide> slides() {
  return {
      {52, "A RoboTaxi in Memory", robotaxi_memory::run},
      {53, "A Taxi through a Pointer", taxi_pointer::run},
      {54, "A Base Class Reference to a Derived Object", base_ref::run},
      {55, "A Base Class Pointer to a Derived Object", base_ptr::run},
      {57, "protected Members", protected_member::run},
      {59, "protected Data", protected_data::run},
      {60, "Through Vehicle's public Methods", through_methods::run},
      {63, "Construction and Destruction Order", order::run},
  };
}

}  // namespace inheritance
