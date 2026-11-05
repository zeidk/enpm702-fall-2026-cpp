/**
 * @file polymorphism.cpp
 * @brief L9 Section 3, Polymorphism: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. The classes here keep few members (a
 * maximum speed, a speed, drive; the RoboTaxi of compile_time and runtime also
 * keeps its operating zone and automation level, and Taxi its driver's name),
 * so the output shows which drive() runs. Two versions, one per namespace:
 * - compile_time: drive() is not virtual. Each class redefines it.
 * - runtime: drive() is virtual, and RoboTaxi and Taxi override it.
 *
 * Slicing ([Slide 66]) is in the Inheritance part of the deck, before virtual,
 * so it uses the compile_time classes.
 *
 * The frames that Live Coding: Steps 4 and 5 replace are in
 * appendix.cpp: [Appendix iv] and [Appendix v]. The
 * appendix frames auto or a Base Pointer ([Appendix xiv] to [Appendix xvii]),
 * One Overload per Class ([Appendix xxviii]) and A Typo That Compiles
 * ([Appendix xl]) are there too, with copies of the runtime and compile_time
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
// [Slide 80] Redefinition: Vehicle, RoboTaxi and Taxi each write drive().
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

// [Slide 66] Slicing. With g++ 13.3, this Vehicle is two doubles (16 bytes)
// and its Taxi adds a 32-byte std::string (48).
namespace slicing {
void run() {
  const Route maya_ride{make_trip()};
  Taxi taxi{13.4, "Ana"};
  Vehicle copy{taxi};  // not a reference
  copy.drive(maya_ride);  // Vehicle: drives 500 m
  std::vector<Vehicle> parked;
  parked.push_back(taxi);  // sliced too
  parked[0].drive(maya_ride);  // Vehicle: drives 500 m
  std::cout << sizeof(Vehicle) << ' ' << sizeof(Taxi) << '\n';  // 16 48
}
}  // namespace slicing

// [Slide 81] Each object runs its own class's drive().
namespace redefinition {
void run() {
  Vehicle vehicle{13.4};
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  vehicle.drive(make_trip());   // Vehicle: drives 500 m
  robotaxi.drive(make_trip());  // RoboTaxi: drives itself 500 m
  taxi.drive(make_trip());      // Taxi: Ana drives 500 m
}
}  // namespace redefinition

// [Slide 82] The Challenge: Two Vectors. by_value keeps only the Vehicle
// parts (slicing); through a Vehicle*, the compiler calls Vehicle::drive.
namespace two_vectors {
void run() {
  const Route maya_ride{make_trip()};
  std::vector<Vehicle> by_value;
  by_value.push_back(RoboTaxi{15.0});
  by_value.push_back(Taxi{13.4, "David"});
  for (const auto& vehicle : by_value) {
    vehicle.drive(maya_ride);  // Vehicle: drives 500 m, twice
  }

  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  std::vector<Vehicle*> by_pointer{&robotaxi, &taxi};
  for (const Vehicle* vehicle : by_pointer) {
    vehicle->drive(maya_ride);  // Vehicle: drives 500 m, twice
  }
}
}  // namespace two_vectors

// [Slide 83] The Challenge: A Base Reference
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
// [Slide 86] Step 1: public Inheritance: does not compile. The code and its error are in
// ../not_compiling/slide_086_private_inheritance.cpp.

// [Slide 87] Step 2: A virtual Method in the Base Class
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;  // why: [Slide 64]
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 88] Step 2: Overriding and override, and [Slide 89] RoboTaxi Marks
// drive override.
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

// [Slide 90] Step 3: A Base Reference or Pointer. The code of the challenge
// ([Slide 82] and [Slide 83]), now with drive() virtual: each object runs its
// own drive().
void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

namespace base_reference {
void run() {
  const Route maya_ride{make_trip()};
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  start_shift(robotaxi, maya_ride);  // RoboTaxi: drives itself 500 m
  start_shift(taxi, maya_ride);      // Taxi: Ana drives 500 m
  std::vector<Vehicle*> by_pointer{&robotaxi, &taxi};
  for (const Vehicle* vehicle : by_pointer) {
    vehicle->drive(maya_ride);  // RoboTaxi: ..., then Taxi: ...
  }
}
}  // namespace base_reference

}  // namespace runtime

std::vector<Slide> slides() {
  return {
      {66, "Slicing", compile_time::slicing::run},
      {81, "Redefinition", compile_time::redefinition::run},
      {82, "The Challenge: Two Vectors", compile_time::two_vectors::run},
      {83, "The Challenge: A Base Reference", compile_time::base_reference::run},
      {90, "Step 3: A Base Reference or Pointer", runtime::base_reference::run},
  };
}

}  // namespace polymorphism
