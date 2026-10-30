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
 * ../../appendix/src/appendix.cpp: [Appendix iv] and [Appendix v].
 */
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"

namespace polymorphism {

using fleet::Position;
using fleet::Route;

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  return trip;
}

// ---------------------------------------------------------------------------
// Compile-time polymorphism: no virtual.
// ---------------------------------------------------------------------------
namespace compile_time {
// [Slide 67] Compile-Time Polymorphism: this Vehicle and this RoboTaxi.
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

// [Slide 68] Redefinition
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

// [Slide 69] Through a Base Reference
void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

namespace base_reference {
void run() {
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, make_trip());  // Vehicle: drives 500 m
  start_shift(taxi, make_trip());      // Vehicle: drives 500 m
}
}  // namespace base_reference

// [Slide 70] One Overload per Class. In their own namespace, so the calls
// of [Slide 69] above still find only start_shift(const Vehicle&, const Route&).
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
// [Slide 71] virtual Method
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;  // why: [Slide 83]
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 77] override: RoboTaxi::drive is marked override.
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

// [Slide 74] The Cost of virtual: one hidden pointer per object.
// compile_time::Vehicle has the same data members and no virtual method.
// The table is in the object file, in build/project/week9/CMakeFiles/
// week9_playground.dir/playground/src/: nm -C polymorphism.cpp.o lists
// "vtable for polymorphism::runtime::Vehicle". Without -C (demangle), nm
// prints the name as the linker sees it: _ZTVN12polymorphism7runtime7VehicleE.
namespace vptr {
void run() {
  using Plain = compile_time::Vehicle;  // no virtual method
  using Poly = runtime::Vehicle;        // virtual methods
  std::cout << sizeof(Plain) << ' ' << sizeof(Poly) << '\n';  // 16 24
  std::cout << std::boolalpha << std::is_polymorphic_v<Plain> << ' '
            << std::is_polymorphic_v<Poly> << '\n';  // false true
}
}  // namespace vptr

// [Slide 75] Slicing
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

// [Slide 81] Two Pointers, Two Static Types. Its first two lines are on [Slide 80].
namespace auto_calls {
void run() {
  auto robotaxi{std::make_unique<RoboTaxi>(13.4)};
  static_assert(std::is_same_v<decltype(robotaxi), std::unique_ptr<RoboTaxi>>);
  std::unique_ptr<Vehicle> vehicle{std::make_unique<RoboTaxi>(13.4)};
  robotaxi->drive(make_trip());  // RoboTaxi: drives itself 500 m
  vehicle->drive(make_trip());   // RoboTaxi: drives itself 500 m
  vehicle = std::make_unique<Taxi>(13.4, "Ana");  // a Vehicle pointer: any Vehicle
  vehicle->drive(make_trip());   // Taxi: Ana drives 500 m
  // robotaxi = std::make_unique<Taxi>(13.4, "Ana");  // does not compile:
  // error: no match for 'operator=' (operand types are
  //   'std::unique_ptr<polymorphism::runtime::RoboTaxi,
  //   std::default_delete<polymorphism::runtime::RoboTaxi> >' and
  //   'std::__detail::__unique_ptr_t<polymorphism::runtime::Taxi>')
}
}  // namespace auto_calls

// [Slide 82] From auto into a Vector of Vehicles. This start_shift takes the
// ownership of the vehicle, and the route.
namespace auto_fleet {
void start_shift(std::unique_ptr<Vehicle> vehicle, const Route& route) {
  vehicle->drive(route);
}

void run() {
  const Route trip{make_trip()};
  std::vector<std::unique_ptr<Vehicle>> vehicles;
  auto robotaxi{std::make_unique<RoboTaxi>(13.4)};
  vehicles.push_back(std::move(robotaxi));              // converts
  auto taxi{std::make_unique<Taxi>(13.4, "Ana")};
  vehicles.emplace_back(std::move(taxi));               // converts too
  std::cout << vehicles.size() << '\n';                 // 2
  start_shift(std::make_unique<RoboTaxi>(13.4), trip);  // RoboTaxi: drives itself 500 m
}
}  // namespace auto_fleet

}  // namespace runtime

// ---------------------------------------------------------------------------
// [Slide 78] A Typo That Compiles: no override.
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
  void drvie(const Route& route) const {  // a new method
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }
};

void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

void run() {
  RoboTaxi robotaxi;
  start_shift(robotaxi, make_trip());  // Vehicle: drives 500 m
}
// With "void drive(const Route& route)" (no const) instead of "drvie", it
// compiles too, and -Wall warns: see the slide.
// [Slide 79] override Catches Both. With override, both stop the build.
}  // namespace typo

std::vector<Slide> slides() {
  return {
      {68, "Redefinition", compile_time::redefinition::run},
      {69, "Through a Base Reference", compile_time::base_reference::run},
      {70, "One Overload per Class", compile_time::overloads::run},
      {74, "The Cost of virtual", runtime::vptr::run},
      {75, "Slicing", runtime::slicing::run},
      {78, "A Typo That Compiles", typo::run},
      {81, "Two Pointers, Two Static Types", runtime::auto_calls::run},
      {82, "From auto into a Vector of Vehicles", runtime::auto_fleet::run},
  };
}

}  // namespace polymorphism
