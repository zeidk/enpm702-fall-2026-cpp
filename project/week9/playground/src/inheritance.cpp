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
#include <string>
#include <utility>
#include <vector>

#include "l8_vehicle.hpp"
#include "sections.hpp"

namespace inheritance {

// RoboTaxi and Taxi on L8's Vehicle, as Live Coding: Step 3 writes them. Their
// sizes are on [Slide 54].
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
};

// Taxi's constructor is also on [Slide 59], Constructors and Inheritance.
class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;  // empty: no driver
};

// [Slide 54] Inside a Taxi Object
// l8::Vehicle: int id_ (4 bytes, then 4 bytes of padding), two doubles (16),
// color_, a std::string (32 with libstdc++), mileage_, a double (8), and a
// std::vector (24 with libstdc++): 88. Taxi adds a std::string: 32 more.
namespace inside {
void run() {
  Taxi taxi{13.4, "Ana"};
  l8::Vehicle& vehicle{taxi};  // a Taxi is a Vehicle: no cast
  std::cout << sizeof(l8::Vehicle) << ' ' << sizeof(RoboTaxi) << ' '
            << sizeof(Taxi) << '\n';                // 88 88 120
  std::cout << sizeof(std::string) << '\n';         // 32
  std::cout << &taxi << '\n' << &vehicle << '\n';  // the same address
}
}  // namespace inside

// [Slide 55] protected Members. L8's Vehicle with one protected method:
// Vehicle's methods and the methods of RoboTaxi and Taxi can call log_event(),
// and other code cannot. Only the members this slide needs are written here.
namespace protected_member {
class Vehicle {  // L8's Vehicle, with one protected method
 public:     // any code
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}

 protected:  // Vehicle's, RoboTaxi's and Taxi's methods
  void log_event(const std::string& event) const {
    std::cout << "vehicle " << id_ << ": " << event << '\n';
  }

 private:    // Vehicle's methods only
  inline static int next_id_{1};
  int id_;
  double max_speed_;
  std::string color_;
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void start() const { log_event("starts"); }  // a method of a derived class: OK
};

void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.start();  // vehicle 1: starts
}
// From main(), robotaxi.log_event("starts"); does not compile:
// error: 'void Vehicle::log_event(const std::string&) const' is protected within this context
}  // namespace protected_member

// [Slide 56] private Stays private. Does not compile:
//   void go() { speed_ = get_max_speed(); }  // inside RoboTaxi
// error: 'double l8::Vehicle::speed_' is private within this context
// The fix the slide gives: set_speed(get_max_speed());

// [Slide 57] protected Data. If speed_ were protected, a derived class could
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
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void speed_up() { speed_ = 20.0; }  // compiles: no check
};

void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.speed_up();
  std::cout << robotaxi.get_speed() << ' ' << robotaxi.get_max_speed() << '\n';  // 20 13.4
}
}  // namespace protected_data

// [Slide 58] public Inheritance. With "class RoboTaxi : Vehicle"
// (no public), the base is private, and main() cannot call get_speed():
// error: 'double Vehicle::get_speed() const' is inaccessible within this context
// error: 'Vehicle' is not an accessible base of 'RoboTaxi'

// [Slide 60] Without the Base Class Initializer. Does not compile:
//   explicit RoboTaxi(double max_speed) {}
// error: no matching function for call to 'Vehicle::Vehicle()'
// note: candidate: 'Vehicle::Vehicle(double, double)'
// note:   candidate expects 2 arguments, 0 provided
// note: candidate: 'Vehicle::Vehicle(double, const std::string&)'
// note:   candidate expects 2 arguments, 0 provided
// The slide cuts the notes after these: the copy and move constructors are
// candidates too.

// [Slide 61] A Base Member in the List. Does not compile:
//   explicit RoboTaxi(double max_speed) : max_speed_{max_speed} {}
// error: 'double Vehicle::max_speed_' is private within this context
// error: class 'RoboTaxi' does not have any field named 'max_speed_'
// error: no matching function for call to 'Vehicle::Vehicle()'

// [Slide 62] Construction and Destruction Order. Both classes
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
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {
    std::cout << "RoboTaxi constructor\n";
  }
  ~RoboTaxi() { std::cout << "~RoboTaxi\n"; }
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
      {54, "Inside a Taxi Object", inside::run},
      {55, "protected Members", protected_member::run},
      {57, "protected Data", protected_data::run},
      {62, "Construction and Destruction Order", order::run},
  };
}

}  // namespace inheritance
