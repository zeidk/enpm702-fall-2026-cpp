/**
 * @file polymorphism.cpp
 * @brief L9 Section 3, Polymorphism: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. The classes here keep few members (a
 * maximum speed, a speed, drive; the RoboTaxi of compile_time and runtime also
 * keeps its operating zone and automation level, and Taxi its driver's name),
 * so the output shows which drive() runs. Three versions, one per namespace:
 * - compile_time: drive() is not virtual. Each class redefines it.
 * - runtime: drive() is virtual, and RoboTaxi and Taxi override it.
 * - typo: RoboTaxi misspells drive() and has no override.
 *
 * The slide that deletes a Taxi through a Vehicle pointer with no virtual
 * destructor is undefined behavior: it is in ../../undefined/src/undefined.cpp.
 * The frames that Live Coding: Steps 4 and 5 replace are in
 * ../../appendix/src/appendix.cpp: [Appendix iv] and [Appendix v]. The
 * appendix frames auto or a Base Pointer ([Appendix xv] to [Appendix xviii]),
 * The Cost of virtual ([Appendix xx]) and One Overload per Class
 * ([Appendix xxx]) are there too, with copies of the runtime and compile_time
 * classes of this file.
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "automation_level.hpp"
#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"

namespace polymorphism {

using fleet::AutomationLevel;
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
// [Slide 65] Compile-Time Polymorphism: this Vehicle and this RoboTaxi.
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

// [Slide 67] Redefinition
class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void drive(const Route& route) const {  // redefines Vehicle::drive
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
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

// [Slide 70] Through a Base Reference
void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

namespace base_reference {
void run() {
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, make_trip());  // Vehicle: drives 500 m
  start_shift(taxi, make_trip());      // Vehicle: drives 500 m
}
}  // namespace base_reference

}  // namespace compile_time

// ---------------------------------------------------------------------------
// Runtime polymorphism: virtual.
// ---------------------------------------------------------------------------
namespace runtime {
// [Slide 71] virtual Methods in the Base Class
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;  // why: [Slide 79]
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 75] override: RoboTaxi::drive is marked override.
class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void drive(const Route& route) const override {
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
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

// [Slide 73] Slicing
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
// [Slide 76] A Typo That Compiles: no override.
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
// [Slide 77] override Catches Both. With override, both stop the build.
}  // namespace typo

std::vector<Slide> slides() {
  return {
      {67, "Redefinition", compile_time::redefinition::run},
      {70, "Through a Base Reference", compile_time::base_reference::run},
      {73, "Slicing", runtime::slicing::run},
      {76, "A Typo That Compiles", typo::run},
  };
}

}  // namespace polymorphism
