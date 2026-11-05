/**
 * @file inheritance.cpp
 * @brief L9 Section 2, Class Relationships: inheritance.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. RoboTaxi and Taxi derive from L8's
 * Vehicle (l8::Vehicle), which has no virtual method yet: that comes in
 * Section 3.
 *
 * A slide whose code does not compile has one file in ../not_compiling, with
 * the error g++ 13.3 gives. A single line that does not compile, inside code
 * that runs, stays here as a comment with its error.
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
// sizes are on [Appendix xxv], Inside a Taxi Object, in
// appendix.cpp, with copies of these two classes. Each has
// one method that Vehicle does not have, change_zone() and assign_driver(), for
// [Slide 62] and [Slide 60]. Each class declares it; its body is below the
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

// Taxi's constructor is also on [Slide 74], Base Class Initializer.
class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  void assign_driver(std::string driver_name);  // body below the class

 private:
  std::string driver_name_;  // empty: no driver
};

void Taxi::assign_driver(std::string driver_name) { driver_name_ = std::move(driver_name); }

// [Slide 57] A RoboTaxi in Memory. car_a holds every data member of Vehicle,
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

// [Slide 58] A Taxi through a Pointer. taxi, on the stack, is one pointer (8
// bytes); the Taxi on the heap is its Vehicle part (88) and driver_name_ (32).
namespace taxi_pointer {
void run() {
  auto taxi{std::make_unique<Taxi>(13.4, "Ana")};
  std::cout << sizeof(taxi) << ' ' << sizeof(*taxi) << '\n';  // 8 120
  std::cout << taxi->get_color() << '\n';                     // the Vehicle part
}
}  // namespace taxi_pointer

// [Slide 59] Where the Compiler Looks for a Method. RoboTaxi writes no
// set_speed() and no get_speed(), so the compiler finds both in Vehicle.
namespace method_lookup {
void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.set_speed(10.0);  // Vehicle's
  std::cout << robotaxi.get_speed() << '\n';  // 10
}
}  // namespace method_lookup

// [Slide 60] A Base Class Reference to a Derived Object. No cast: base_ref is
// another name for the Vehicle part of taxi, its base subobject. The compiler
// checks a call through base_ref against Vehicle's members only, so
// assign_driver() is reachable only through taxi.
namespace base_ref {
void run() {
  Taxi taxi{13.4, "Ana"};
  taxi.assign_driver("Ben");    // a Taxi method
  l8::Vehicle& base_ref{taxi};  // no cast needed
  // base_ref.assign_driver("Ben");  // does not compile
  base_ref.set_speed(10.0);
  std::cout << taxi.get_speed() << '\n';  // 10: taxi itself changed
}
// Uncommented, the line gives:
// error: 'class l8::Vehicle' has no member named 'assign_driver'
}  // namespace base_ref

// [Slide 61] base_ref in Memory: the line Vehicle& base_ref{taxi}; of [Slide 60].

// [Slide 62] A Base Class Pointer to a Derived Object. No cast: pointer holds
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

// [Slide 64] A Base Class Smart Pointer to a Derived Object deletes a RoboTaxi
// through a Vehicle*, and l8::Vehicle has no virtual destructor: its code is in
// undefined.cpp.

// [Slide 65] Not the Other Way: does not compile. The code and its error are in
// ../not_compiling/slide_065_not_the_other_way.cpp.

// [Slide 66] Slicing is in polymorphism.cpp: it needs a Vehicle with drive().

// [Slide 68] A Base Class Reference or Pointer to a Derived Object. Nothing is
// copied: base_ref and pointer both name taxi's Vehicle part, and vehicles holds
// the whole Taxi. The output line is not on the slide.
namespace summary {
void run() {
  Taxi taxi{13.4, "Ana"};
  l8::Vehicle& base_ref{taxi};   // refers to taxi: nothing is copied
  l8::Vehicle* pointer{&taxi};   // holds taxi's address: nothing is copied
  std::vector<std::shared_ptr<l8::Vehicle>> vehicles;
  vehicles.push_back(std::make_shared<Taxi>(13.4, "Ana"));  // the whole Taxi
  std::cout << std::boolalpha << (pointer == &base_ref) << ' '
            << vehicles.size() << '\n';  // true 1
}
}  // namespace summary

// [Slide 69] protected Members. L8's Vehicle with one protected method:
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

// [Slide 70] private Stays private: does not compile. The code and its error are in
// ../not_compiling/slide_070_private_stays_private.cpp.

// [Slide 71] protected Data. If speed_ were protected, a derived class could
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
  void speed_up() { speed_ = 100.0; }  // compiles: no check

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.speed_up();
  std::cout << robotaxi.get_speed() << ' ' << robotaxi.get_max_speed() << '\n';  // 100 13.4
}
}  // namespace protected_data

// [Slide 72] Through Vehicle's public Methods, and [Slide 73] Vehicle's Check
// Runs for a RoboTaxi. speed_ stays private: RoboTaxi reads it with get_speed()
// and writes it with set_speed(), so the check runs.
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

// [Slide 48] No Specifier Means private Inheritance: does not compile. The code and its error are in
// ../not_compiling/slide_048_no_specifier.cpp.

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

// A Base Member in the List is an appendix frame: [Appendix xiii], in
// appendix.cpp.

// [Slide 75] Construction and Destruction Order. Both classes
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
      {57, "A RoboTaxi in Memory", robotaxi_memory::run},
      {58, "A Taxi through a Pointer", taxi_pointer::run},
      {59, "Where the Compiler Looks for a Method", method_lookup::run},
      {60, "A Base Class Reference to a Derived Object", base_ref::run},
      {62, "A Base Class Pointer to a Derived Object", base_ptr::run},
      {68, "A Base Class Reference or Pointer to a Derived Object", summary::run},
      {69, "protected Members", protected_member::run},
      {71, "protected Data", protected_data::run},
      {73, "Vehicle's Check Runs for a RoboTaxi", through_methods::run},
      {75, "Construction and Destruction Order", order::run},
  };
}

}  // namespace inheritance
