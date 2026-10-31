/**
 * @file appendix.cpp
 * @brief L9: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_appendix.
 *
 * @code
 * 702run week9_appendix        # every appendix frame
 * 702run week9_appendix ix     # only [Appendix ix]; 702run week9_appendix 9 works too
 * @endcode
 *
 * Appendix iii to viii, "What the Live Coding Replaces", hold the class of Live
 * Coding: Step 1 and code that goes with Steps 4 to 7. Each frame keeps the
 * classes of its step, written here in its own namespace: simple classes with a
 * virtual drive() in Steps 4 and 5, and the final classes of ../../common from
 * Step 6 on.
 *
 * The frames that do not compile are kept here as comments, with the error
 * g++ 13.3 gives (-std=c++20 -Wall -Wextra -pedantic-errors -Wshadow).
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "slides.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace {

using fleet::Position;
using fleet::RoboTaxi;
using fleet::Route;
using fleet::SensorType;
using fleet::Taxi;
using fleet::Vehicle;

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route maya_ride;
  maya_ride.add(Position{0.0, 0.0});
  maya_ride.add(Position{300.0, 400.0});
  return maya_ride;
}

// [Appendix iii] A Passenger Rides a Vehicle shows the class only, with no
// output. The class is in ../../playground/src/relationships.cpp, where
// [Slide 26] runs it.

// The classes of [Appendix iv]: as on [Slide 82], drive()
// is virtual, and RoboTaxi and Taxi override it.
namespace runtime {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void drive(const Route& route) const override {
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  void drive(const Route& route) const override {
    std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
  }

 private:
  std::string driver_name_;
};

// [Appendix iv] One Function for Every Vehicle
void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

namespace one_function {
void run() {
  const Route maya_ride{make_trip()};
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, maya_ride);  // RoboTaxi: drives itself 500 m
  start_shift(taxi, maya_ride);      // Taxi: Ana drives 500 m
}
}  // namespace one_function
}  // namespace runtime

// [Appendix v] With a virtual Destructor. Both destructors print; ~Vehicle is
// virtual. The run is the code of [Slide 95], fixed.
namespace virtual_dtor {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() { std::cout << "~Vehicle\n"; }
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  ~Taxi() override { std::cout << "~Taxi\n"; }
  void drive(const Route& route) const override {
    std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
  }

 private:
  std::string driver_name_;
};

void run() {
  const Route maya_ride{make_trip()};
  std::unique_ptr<Vehicle> vehicle{std::make_unique<Taxi>(13.4, "Ana Maria Rodrigues")};
  vehicle->drive(maya_ride);
}  // ~Taxi, then ~Vehicle
}  // namespace virtual_dtor

// [Appendix vi] No Vehicle Objects. The abstract Vehicle of ../../common. Does
// not compile:
//   Vehicle vehicle{13.4};
// error: cannot declare variable 'vehicle' to be of abstract type 'Vehicle'
// note:   because the following virtual functions are pure within 'Vehicle':
// note:     'virtual void Vehicle::drive(const Route&) const'
// A reference or a pointer to Vehicle is fine:
namespace no_vehicle {
void run() {
  const Route maya_ride{make_trip()};
  RoboTaxi robotaxi{13.4};
  robotaxi.add_sensor(SensorType::lidar, 100.0);
  const Vehicle& vehicle{robotaxi};  // a reference to Vehicle is fine
  vehicle.drive(maya_ride);  // RoboTaxi: 1 sensor checked, drives 500 m
}
}  // namespace no_vehicle

// [Appendix vii] RoboTaxi and Taxi
namespace concrete {
void run() {
  const Route maya_ride{make_trip()};
  RoboTaxi robotaxi{13.4};
  robotaxi.drive(maya_ride);  // RoboTaxi: no sensor, stays parked
  robotaxi.add_sensor(SensorType::lidar, 100.0);
  robotaxi.drive(maya_ride);  // RoboTaxi: 1 sensor checked, drives 500 m
  Taxi taxi{13.4, "Ana"};
  taxi.drive(maya_ride);      // Taxi: Ana drives 500 m
}
}  // namespace concrete

// [Appendix viii] A final Class. RoboTaxi is final in ../../common. Does not
// compile:
//   class SuperRoboTaxi : public RoboTaxi {};
// error: cannot derive from 'final' base 'RoboTaxi' in derived type 'SuperRoboTaxi'

// [Appendix ix] Multiple Inheritance
namespace multiple {
class Animal {
 public:
  [[nodiscard]] bool can_fly() const noexcept { return has_wings_; }

 protected:
  bool has_tail_{true};
  bool has_wings_{false};
  bool has_horns_{false};
};

class Human {
 public:
  void speak() const { std::cout << "Human: speaks\n"; }

 protected:
  bool is_bipedal_{true};
};

class MythicalCreature : public Animal, public Human {
 public:
  MythicalCreature(bool has_wings, std::vector<std::string> powers)
      : powers_{std::move(powers)} {
    has_wings_ = has_wings;  // a protected member of Animal
  }
  void use_power(const std::string& power) const {
    const bool has_it{std::ranges::find(powers_, power) != powers_.end()};
    std::cout << (has_it ? "uses " : "does not have ") << power << '\n';
  }

 private:
  std::vector<std::string> powers_;
};

void run() {
  MythicalCreature sphinx{true, {"riddle"}};
  const Animal& animal{sphinx};
  const Human& human{sphinx};
  std::cout << std::boolalpha << animal.can_fly() << '\n';  // true
  human.speak();               // Human: speaks
  sphinx.use_power("riddle");  // uses riddle
}
}  // namespace multiple

// [Appendix xi] Copying a Vehicle of the Design. Neither compiles:
//   Taxi copy{taxi};             // the sensors cannot be copied (std::unique_ptr, L8)
//   Taxi moved{std::move(taxi)};  // the declared destructor stops the moves
// Each gives 4 errors, from inside std::vector. The copy is not suppressed:
// std::is_copy_constructible_v<Taxi> is true. With
//   Vehicle(const Vehicle&) = delete;
//   Vehicle& operator=(const Vehicle&) = delete;
// in Vehicle, the trait is false, and the copy stops with
// error: use of deleted function 'Taxi::Taxi(const Taxi&)'

}  // namespace

int main(int argc, char* argv[]) {
  return run_slides(
      {
          {4, "One Function for Every Vehicle", runtime::one_function::run},
          {5, "With a virtual Destructor", virtual_dtor::run},
          {6, "No Vehicle Objects", no_vehicle::run},
          {7, "RoboTaxi and Taxi", concrete::run},
          {9, "Multiple Inheritance", multiple::run},
      },
      argc, argv, Part::appendix);
}
