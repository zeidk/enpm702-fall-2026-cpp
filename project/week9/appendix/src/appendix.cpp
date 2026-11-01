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
 * Appendix vi to xi, "What the Live Coding Replaces", hold the class of Live
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
#include <type_traits>
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

// [Appendix vi] A Passenger Rides a Vehicle shows the class only, with no
// output. The class is in ../../playground/src/relationships.cpp, where
// [Slide 25] runs it.

// The classes of [Appendix vii]: as on [Slide 80], drive()
// is virtual, and RoboTaxi and Taxi override it. auto or a Base Pointer
// ([Appendix xix] and [Appendix xx]) and The Cost of virtual ([Appendix xxiii])
// use them too.
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

// [Appendix vii] One Function for Every Vehicle
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

// [Appendix viii] With a virtual Destructor. Both destructors print; ~Vehicle is
// virtual. The run is the code of [Slide 88], fixed.
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

// [Appendix ix] No Vehicle Objects. The abstract Vehicle of ../../common. Does
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

// [Appendix x] RoboTaxi and Taxi
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

// [Appendix xi] A final Class. RoboTaxi is final in ../../common. Does not
// compile:
//   class SuperRoboTaxi : public RoboTaxi {};
// error: cannot derive from 'final' base 'RoboTaxi' in derived type 'SuperRoboTaxi'

// [Appendix xii] A final Method. Does not compile:
//   class Taxi : public Vehicle {  // Taxi not final, drive() final
//    public:
//     void drive(const Route& route) const final;
//   };
//   class NightTaxi : public Taxi {
//    public:
//     void drive(const Route& route) const override;
//   };
// error: virtual function 'virtual void NightTaxi::drive(const Route&) const' overriding final function
// note: overridden function is 'virtual void Taxi::drive(const Route&) const'

// [Appendix xiv] Multiple Inheritance, the Code. The class diagram is on
// [Appendix xiii], Multiple Inheritance.
namespace multiple {
class Animal {
 public:
  explicit Animal(bool has_wings) : has_wings_{has_wings} {}
  [[nodiscard]] bool can_fly() const noexcept { return has_wings_; }

 private:
  bool has_tail_{true};
  bool has_wings_{false};
  bool has_horns_{false};
};

class Human {
 public:
  void speak() const { std::cout << "Human: speaks\n"; }

 private:
  bool is_bipedal_{true};
};

class MythicalCreature : public Animal, public Human {
 public:
  MythicalCreature(bool has_wings, std::vector<std::string> powers)
      : Animal{has_wings}, powers_{std::move(powers)} {}
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

// [Appendix xvii] A Base Member in the List. Does not compile:
//   explicit RoboTaxi(double max_speed) : max_speed_{max_speed} {}
// error: 'double Vehicle::max_speed_' is private within this context
// error: class 'RoboTaxi' does not have any field named 'max_speed_'
// error: no matching function for call to 'Vehicle::Vehicle()'

namespace runtime {
// [Appendix xix] Two Pointers, Two Static Types. Its first two lines are on
// [Appendix xviii], auto or a Base Pointer.
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
  //   'std::unique_ptr<{anonymous}::runtime::RoboTaxi,
  //   std::default_delete<{anonymous}::runtime::RoboTaxi> >' and
  //   'std::__detail::__unique_ptr_t<{anonymous}::runtime::Taxi>')
}
}  // namespace auto_calls

// [Appendix xx] From auto into a Vector of Vehicles. hand_over takes the
// ownership of the vehicle, and the route.
namespace auto_fleet {
void hand_over(std::unique_ptr<Vehicle> vehicle, const Route& route) {
  vehicle->drive(route);
}

void run() {
  const Route maya_ride{make_trip()};
  std::vector<std::unique_ptr<Vehicle>> vehicles;
  auto robotaxi{std::make_unique<RoboTaxi>(13.4)};
  vehicles.push_back(std::move(robotaxi));              // converts
  auto taxi{std::make_unique<Taxi>(13.4, "Ana")};
  vehicles.emplace_back(std::move(taxi));               // converts too
  std::cout << vehicles.size() << '\n';                 // 2
  hand_over(std::make_unique<RoboTaxi>(13.4), maya_ride);
  // prints RoboTaxi: drives itself 500 m
}
}  // namespace auto_fleet
}  // namespace runtime

// [Appendix xxii] Copying a Vehicle of the Design. Neither compiles:
//   Taxi copy{taxi};             // the sensors cannot be copied (std::unique_ptr, L8)
//   Taxi moved{std::move(taxi)};  // the declared destructor stops the moves
// Each gives 4 errors, from inside std::vector. The copy is not suppressed:
// std::is_copy_constructible_v<Taxi> is true. With
//   Vehicle(const Vehicle&) = delete;
//   Vehicle& operator=(const Vehicle&) = delete;
// in Vehicle, the trait is false, and the copy stops with
// error: use of deleted function 'Taxi::Taxi(const Taxi&)'

// [Appendix xxiii] The Cost of virtual: one hidden pointer per object.
// compile_time::Vehicle is a copy of the Vehicle of [Slide 70], with no virtual
// method; runtime::Vehicle above is a copy of the one of [Slide 80]. Both have
// the same data members.
// The frame's nm line reads week9_playground's object file, where the
// originals are: nm -C polymorphism.cpp.o, in build/project/week9/CMakeFiles/
// week9_playground.dir/playground/src/, lists
// "vtable for polymorphism::runtime::Vehicle". Without -C (demangle), nm
// prints the name as the linker sees it: _ZTVN12polymorphism7runtime7VehicleE.
namespace compile_time {
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
}  // namespace compile_time

namespace vptr {
void run() {
  using Plain = compile_time::Vehicle;  // no virtual method
  using Poly = runtime::Vehicle;        // virtual methods
  std::cout << sizeof(Plain) << ' ' << sizeof(Poly) << '\n';  // 16 24
  std::cout << std::boolalpha << std::is_polymorphic_v<Plain> << ' '
            << std::is_polymorphic_v<Poly> << '\n';  // false true
}
}  // namespace vptr

}  // namespace

int main(int argc, char* argv[]) {
  return run_slides(
      {
          {7, "One Function for Every Vehicle", runtime::one_function::run},
          {8, "With a virtual Destructor", virtual_dtor::run},
          {9, "No Vehicle Objects", no_vehicle::run},
          {10, "RoboTaxi and Taxi", concrete::run},
          {14, "Multiple Inheritance, the Code", multiple::run},
          {19, "Two Pointers, Two Static Types", runtime::auto_calls::run},
          {20, "From auto into a Vector of Vehicles", runtime::auto_fleet::run},
          {23, "The Cost of virtual", vptr::run},
      },
      argc, argv, Part::appendix);
}
