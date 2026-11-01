/**
 * @file appendix.cpp
 * @brief L9: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_appendix.
 *
 * @code
 * 702run week9_appendix        # every appendix frame
 * 702run week9_appendix vi     # only [Appendix vi]; 702run week9_appendix 6 works too
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
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "fleet.hpp"
#include "l8_vehicle.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "sensor.hpp"
#include "slides.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace {

using fleet::Fleet;
using fleet::Position;
using fleet::RoboTaxi;
using fleet::Route;
using fleet::Sensor;
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
// [Slide 25] runs it.

// The classes of [Appendix iv]: as on [Slide 68], drive()
// is virtual, and RoboTaxi and Taxi override it. auto or a Base Pointer
// ([Appendix xvi] and [Appendix xvii]) and The Cost of virtual ([Appendix xx])
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
// virtual. The run is the code of [Slide 75], fixed.
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

// [Appendix ix] A final Method. Does not compile:
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

// [Appendix xi] Multiple Inheritance, the Code. The class diagram is on
// [Appendix x], Multiple Inheritance.
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

// [Appendix xiv] A Base Member in the List. Does not compile:
//   explicit RoboTaxi(double max_speed) : max_speed_{max_speed} {}
// error: 'double Vehicle::max_speed_' is private within this context
// error: class 'RoboTaxi' does not have any field named 'max_speed_'
// error: no matching function for call to 'Vehicle::Vehicle()'

namespace runtime {
// [Appendix xvi] Two Pointers, Two Static Types. Its first two lines are on
// [Appendix xv], auto or a Base Pointer.
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

// [Appendix xvii] From auto into a Vector of Vehicles. hand_over takes the
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

// [Appendix xix] Copying a Vehicle of the Design. Neither compiles:
//   Taxi copy{taxi};             // the sensors cannot be copied (std::unique_ptr, L8)
//   Taxi moved{std::move(taxi)};  // the declared destructor stops the moves
// Each gives 4 errors, from inside std::vector. The copy is not suppressed:
// std::is_copy_constructible_v<Taxi> is true. With
//   Vehicle(const Vehicle&) = delete;
//   Vehicle& operator=(const Vehicle&) = delete;
// in Vehicle, the trait is false, and the copy stops with
// error: use of deleted function 'Taxi::Taxi(const Taxi&)'

// [Appendix xx] The Cost of virtual: one hidden pointer per object.
// compile_time::Vehicle is a copy of the Vehicle of [Slide 61], with no virtual
// method; runtime::Vehicle above is a copy of the one of [Slide 68]. Both have
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

// ---------------------------------------------------------------------------
// Frames that continue a lecture slide, the discussions, in the order of the
// appendix.
// ---------------------------------------------------------------------------

// [Appendix xxiv] Each Route Has Its Own Positions
// A Route holds its Position objects by value (composition, as Vehicle holds
// its Sensors through std::unique_ptr). A shared ride: Ana's stop is added
// to a copy of Maya's route.
namespace composition {
void run() {
  Route maya_ride;
  maya_ride.add(Position{0.0, 0.0});      // pickup
  maya_ride.add(Position{300.0, 400.0});  // Maya's stop: 500 m
  Route shared_ride{maya_ride};           // copies the Positions too
  shared_ride.add(Position{300.0, 0.0});  // Ana's stop: 400 m more
  std::cout << maya_ride.length() << ' ' << shared_ride.length() << '\n';  // 500 900
}
}  // namespace composition

// [Appendix xxvii] A Route for One Call
// l8::Vehicle (../../playground/include/l8_vehicle.hpp) is the concrete class
// the students already have: its drive() prints the route's length and
// length / speed. The frame says so; the Abstraction section makes drive()
// pure virtual (../../common).
namespace dependency {
using Vehicle = l8::Vehicle;

void run() {
  Vehicle car_a{13.4};
  car_a.set_speed(10.0);
  {
    Route maya_ride;
    maya_ride.add(Position{0.0, 0.0});
    maya_ride.add(Position{300.0, 400.0});
    car_a.drive(maya_ride);  // 500 m at 10 m/s: 50 s
  }  // the Route is gone; the Vehicle kept nothing of it
  Route next;
  next.add(Position{0.0, 0.0});
  next.add(Position{0.0, 200.0});
  car_a.drive(next);    // 200 m at 10 m/s: 20 s
}
}  // namespace dependency

// [Appendix xxviii] Inside a Taxi Object
// RoboTaxi and Taxi on L8's Vehicle (l8::Vehicle), as Live Coding: Step 3
// writes them: copies of the two classes of
// ../../playground/src/inheritance.cpp.
// l8::Vehicle: int id_ (4 bytes, then 4 bytes of padding), two doubles (16),
// color_, a std::string (32 with libstdc++), mileage_, a double (8), and a
// std::vector (24 with libstdc++): 88. Taxi adds a std::string: 32 more.
namespace inside {
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
};

class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;  // empty: no driver
};

void run() {
  Taxi taxi{13.4, "Ana"};
  std::cout << sizeof(l8::Vehicle) << ' ' << sizeof(RoboTaxi) << ' '
            << sizeof(Taxi) << '\n';         // 88 88 120
  std::cout << sizeof(std::string) << '\n';  // 32
}
}  // namespace inside

// [Appendix xxx] One Overload per Class. RoboTaxi and Taxi redefine drive(),
// as on [Slide 63], Redefinition: copies of the classes of
// ../../playground/src/polymorphism.cpp, on the compile_time::Vehicle above.
namespace compile_time {
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


// [Appendix xxxv] Discussion: What Prints First? A Taxi whose
// base prints and whose member prints.
namespace order {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {
    std::cout << "Vehicle constructor\n";
  }
  ~Vehicle() { std::cout << "~Vehicle\n"; }

 private:
  double max_speed_;
};

struct Meter {  // the fare meter
  Meter() { std::cout << "Meter constructor\n"; }
  ~Meter() { std::cout << "~Meter\n"; }
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {
    std::cout << "Taxi constructor\n";
  }
  ~Taxi() { std::cout << "~Taxi\n"; }

 private:
  std::string driver_name_;
  Meter meter_;
};

void run() {
  Taxi taxi{13.4, "Ana"};
}
// With the list written ": driver_name_{std::move(driver_name)}, Vehicle{max_speed}",
// the order does not change, and g++ warns:
// warning: '{anonymous}::order::Taxi::driver_name_' will be initialized after [-Wreorder]
// warning:   base '{anonymous}::order::Vehicle' [-Wreorder]
}  // namespace order

// [Appendix xxxvii] Discussion: Which drive() Runs?
namespace which {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void drive(const Route& route) const override {
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }
};

void run() {
  const Route maya_ride{make_trip()};
  RoboTaxi robotaxi{13.4};
  Vehicle& ref{robotaxi};
  Vehicle* ptr{&robotaxi};
  Vehicle copy{robotaxi};
  robotaxi.drive(maya_ride);  // 1: RoboTaxi: drives itself 500 m
  ref.drive(maya_ride);       // 2: RoboTaxi: drives itself 500 m
  ptr->drive(maya_ride);      // 3: RoboTaxi: drives itself 500 m
  copy.drive(maya_ride);      // 4: Vehicle: drives 500 m
}
}  // namespace which

// [Appendix xxxviii] Discussion: Who Needs a virtual Destructor?
namespace who_needs {
void run() {
  std::cout << std::boolalpha << std::has_virtual_destructor_v<Vehicle> << ' '
            << std::has_virtual_destructor_v<RoboTaxi> << ' '
            << std::has_virtual_destructor_v<Taxi> << ' '
            << std::has_virtual_destructor_v<Sensor> << ' '
            << std::has_virtual_destructor_v<Route> << ' '
            << std::has_virtual_destructor_v<Fleet> << '\n';
  // true true true false false false
}
}  // namespace who_needs

// [Appendix xxxix] Discussion: Add a RoboBus. One answer: a new
// class, and no change to Fleet. Its invariant: more than 0 seats.
namespace robobus {
class RoboBus : public Vehicle {
 public:
  RoboBus(double max_speed, int seats) : Vehicle{max_speed}, seats_{seats} {
    if (seats_ <= 0) {
      throw std::invalid_argument{"a bus has more than 0 seats"};
    }
  }
  void drive(const Route& route) const override {
    std::cout << "RoboBus: " << seats_ << " seats, drives " << route.length() << " m\n";
  }

 private:
  int seats_;  // more than 0
};

void run() {
  Fleet fleet;
  auto bus{std::make_shared<RoboBus>(11.2, 30)};
  fleet.add(bus);  // Fleet's code does not change
  bus->drive(make_trip());  // RoboBus: 30 seats, drives 500 m
  std::cout << fleet.size() << '\n';  // 1
}
}  // namespace robobus


}  // namespace

int main(int argc, char* argv[]) {
  return run_slides(
      {
          {4, "One Function for Every Vehicle", runtime::one_function::run},
          {5, "With a virtual Destructor", virtual_dtor::run},
          {6, "No Vehicle Objects", no_vehicle::run},
          {7, "RoboTaxi and Taxi", concrete::run},
          {11, "Multiple Inheritance, the Code", multiple::run},
          {16, "Two Pointers, Two Static Types", runtime::auto_calls::run},
          {17, "From auto into a Vector of Vehicles", runtime::auto_fleet::run},
          {20, "The Cost of virtual", vptr::run},
          {24, "Each Route Has Its Own Positions", composition::run},
          {27, "A Route for One Call", dependency::run},
          {28, "Inside a Taxi Object", inside::run},
          {30, "One Overload per Class", compile_time::overloads::run},
          {35, "Discussion: What Prints First?", order::run},
          {37, "Discussion: Which drive() Runs?", which::run},
          {38, "Discussion: Who Needs a virtual Destructor?", who_needs::run},
          {39, "Discussion: Add a RoboBus", robobus::run},
      },
      argc, argv, Part::appendix);
}
