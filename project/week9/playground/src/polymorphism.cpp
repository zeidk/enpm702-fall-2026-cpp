/**
 * @file polymorphism.cpp
 * @brief L9 Section 3, Polymorphism: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. The classes here keep only what these
 * slides need (a maximum speed, a speed, drive), so the output shows which
 * drive() runs. Three versions, one per namespace:
 * - compile_time: drive() is not virtual. Each class redefines it.
 * - runtime: drive() is virtual, and RoboTaxi and Taxi override it.
 * - typo: RoboTaxi misspells drive() and has no override.
 *
 * The slide that deletes a Taxi through a Vehicle pointer with no virtual
 * destructor is undefined behavior: it is in ../../undefined/src/undefined.cpp.
 * The frames that Live Coding: Steps 4 and 5 replace are in
 * ../../appendix/src/appendix.cpp: [Appendix vii] and [Appendix viii]. The
 * appendix frames auto or a Base Pointer ([Appendix xviii] to [Appendix xxi])
 * and The Cost of virtual ([Appendix xxiii]) are there too, with copies of the
 * runtime and compile_time classes of this file.
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"

namespace polymorphism {

using fleet::Position;
using fleet::Route;

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route maya_ride;
  maya_ride.add(Position{0.0, 0.0});
  maya_ride.add(Position{300.0, 400.0});
  return maya_ride;
}

// ---------------------------------------------------------------------------
// Compile-time polymorphism: no virtual.
// ---------------------------------------------------------------------------
namespace compile_time {
// [Slide 70] Compile-Time Polymorphism: this Vehicle and this RoboTaxi.
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 72] Redefinition
class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void drive(const Route& route) const {  // redefines Vehicle::drive
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  void drive(const Route& route) const {  // redefines Vehicle::drive
    std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
  }

 private:
  std::string driver_name_;
};

namespace redefinition {
void run() {
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  robotaxi.drive(make_trip());  // RoboTaxi: drives itself 500 m
  taxi.drive(make_trip());      // Taxi: Ana drives 500 m
}
}  // namespace redefinition

// [Slide 78] Through a Base Reference
void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

namespace base_reference {
void run() {
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, make_trip());  // Vehicle: drives 500 m
  start_shift(taxi, make_trip());      // Vehicle: drives 500 m
}
}  // namespace base_reference

// [Slide 76] One Overload per Class. In their own namespace, so the calls
// of [Slide 78] above still find only start_shift(const Vehicle&, const Route&).
namespace overloads {
void start_shift(const RoboTaxi& robotaxi, const Route& route) { robotaxi.drive(route); }
void start_shift(const Taxi& taxi, const Route& route) { taxi.drive(route); }

void run() {
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, make_trip());  // RoboTaxi: drives itself 500 m
  start_shift(taxi, make_trip());      // Taxi: Ana drives 500 m
}
}  // namespace overloads
}  // namespace compile_time

// ---------------------------------------------------------------------------
// Runtime polymorphism: virtual.
// ---------------------------------------------------------------------------
namespace runtime {
// [Slide 80] virtual Methods in the Base Class
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;  // why: [Slide 87]
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 84] override: RoboTaxi::drive is marked override.
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

// [Slide 81] Slicing
namespace slicing {
void run() {
  Taxi taxi{13.4, "Ana"};
  Vehicle copy{taxi};  // copies only the Vehicle part
  copy.drive(make_trip());  // Vehicle: drives 500 m
  std::vector<Vehicle> parked;
  parked.push_back(taxi);         // sliced too
  parked[0].drive(make_trip());   // Vehicle: drives 500 m
  std::cout << sizeof(Vehicle) << ' ' << sizeof(Taxi) << '\n';  // 24 56
}
}  // namespace slicing

}  // namespace runtime

// ---------------------------------------------------------------------------
// [Slide 85] A Typo That Compiles: no override.
// ---------------------------------------------------------------------------
namespace typo {
class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }
};

class RoboTaxi : public Vehicle {
 public:
  void drvie(const Route& route) const;  // mistake 1: a typo
};

void RoboTaxi::drvie(const Route& route) const {  // a new method
  std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
}

void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

void run() {
  RoboTaxi robotaxi;
  start_shift(robotaxi, make_trip());  // Vehicle: drives 500 m
}
// Mistake 2 on the slide, "void drive(const Route& route);" (no const), is
// left out here: it compiles too, but -Wall warns (-Woverloaded-virtual=), and
// this build has no warnings.
// [Slide 86] override Catches Both. With override, both stop the build.
}  // namespace typo

std::vector<Slide> slides() {
  return {
      {72, "Redefinition", compile_time::redefinition::run},
      {78, "Through a Base Reference", compile_time::base_reference::run},
      {76, "One Overload per Class", compile_time::overloads::run},
      {81, "Slicing", runtime::slicing::run},
      {85, "A Typo That Compiles", typo::run},
  };
}

}  // namespace polymorphism
