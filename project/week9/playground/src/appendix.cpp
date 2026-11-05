/**
 * @file appendix.cpp
 * @brief L9: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. Run one appendix frame by its number in
 * the deck, in roman numerals:
 *
 * @code
 * 702run week9_playground iv     # only [Appendix iv]
 * @endcode
 *
 * Appendix iii to vi, "What the Live Coding Replaces", hold the class of Live
 * Coding: Step 1 and code that goes with Steps 5 and 6, the final classes of
 * ../include. The virtual frames moved to the reading page virtual Methods in
 * Depth on Read the Docs (2026-11-02).
 *
 * A frame whose code does not compile has one file in ../not_compiling, with
 * the error g++ 13.3 gives. A single line that does not compile, inside code
 * that runs, stays here as a comment with its error.
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "automation_level.hpp"
#include "fleet.hpp"
#include "l8_vehicle.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "sensor.hpp"
#include "sections.hpp"
#include "slides.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace appendix {

using fleet::AutomationLevel;
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
// output. The class is in relationships.cpp, where
// [Slide 25] runs it.

// Classes with a virtual drive(), as on [Slide 87]: RoboTaxi and Taxi override
// it. auto or a Base Pointer ([Appendix xv] and [Appendix xvi]) uses them.
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

}  // namespace runtime

// [Appendix iv] No Vehicle Objects. The abstract Vehicle of ../include. Does
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

// [Appendix v] RoboTaxi and Taxi
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

// [Appendix vi] A final Class. RoboTaxi is final in ../include. Does not
// compile:
//   class SuperRoboTaxi : public RoboTaxi {};
// error: cannot derive from 'final' base 'RoboTaxi' in derived type 'SuperRoboTaxi'

// [Appendix vii] A final Method: does not compile. The code and its error are in
// ../not_compiling/appendix_vii_final_method.cpp.

// [Appendix viii] Multiple Inheritance, the Code. The class diagram is on
// [Slide 54], Multiple Inheritance.
namespace multiple {
class Lion {
 public:
  void run() const { std::cout << "Lion: runs on " << leg_count_ << " legs\n"; }

 private:
  int leg_count_{4};
};

class Human {
 public:
  void speak() const { std::cout << "Human: speaks\n"; }
};

class Bird {
 public:
  explicit Bird(double wing_span) : wing_span_{wing_span} {}
  [[nodiscard]] bool can_fly() const noexcept { return wing_span_ > 0.0; }

 private:
  double wing_span_;
};

// A sphinx has the body of a lion, the head of a human and the wings of a bird.
class Sphinx : public Lion, public Human, public Bird {
 public:
  Sphinx(double wing_span, std::string riddle)
      : Bird{wing_span}, riddle_{std::move(riddle)} {}
  void ask_riddle() const { std::cout << "Sphinx: " << riddle_ << '\n'; }

 private:
  std::string riddle_;
};

void run() {
  Sphinx sphinx{2.5, "What walks on four legs in the morning?"};
  const Lion& lion{sphinx};
  const Human& human{sphinx};
  const Bird& bird{sphinx};
  lion.run();                                             // Lion: runs on 4 legs
  human.speak();                                          // Human: speaks
  std::cout << std::boolalpha << bird.can_fly() << '\n';  // true
  sphinx.ask_riddle();  // Sphinx: What walks on four legs in the morning?
}
}  // namespace multiple

// [Appendix xii] protected and private Inheritance, the Code (the story is on
// [Appendix xi], When protected and private Inheritance Make Sense). Shuttle
// reuses the code of PreTripCheck, and that code calls ok(), which Shuttle
// overrides. A Shuttle is not a kind of PreTripCheck, so the inheritance is
// private. The pattern is the isocpp FAQ's, at
// https://isocpp.org/wiki/faq/private-inheritance
// The destructor is protected and non-virtual: Core Guidelines C.35.
namespace private_base {
class PreTripCheck {
 protected:
  ~PreTripCheck() = default;
  void check() const {
    std::cout << (ok() ? "go\n" : "stop\n");
  }
  virtual bool ok() const = 0;
};

class Shuttle : private PreTripCheck {
 public:
  void start() const { check(); }

 private:
  bool ok() const override { return true; }
};

void run() {
  Shuttle shuttle;
  shuttle.start();  // go
  // Does not compile:
  //   const PreTripCheck& base{shuttle};
  // error: 'PreTripCheck' is an inaccessible base of 'Shuttle'
  // Nor does a NightShuttle derived from this Shuttle that calls check():
  // error: 'void PreTripCheck::check() const' is private within this context
}
}  // namespace private_base

// The same with protected: NightShuttle, derived from Shuttle, can call check().
namespace protected_base {
using private_base::PreTripCheck;

class Shuttle : protected PreTripCheck {
 public:
  void start() const { check(); }

 private:
  bool ok() const override { return true; }
};

class NightShuttle : public Shuttle {
 public:
  void start_late() const { check(); }
};

void run() {
  NightShuttle night_shuttle;
  night_shuttle.start_late();  // go
}
}  // namespace protected_base

// [Appendix xiii] A Base Member in the List: does not compile. The code and its error are in
// ../not_compiling/appendix_xiii_base_member_in_list.cpp.

namespace runtime {
// [Appendix xv] Two Pointers, Two Static Types. Its first two lines are on
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

// [Appendix xvi] From auto into a Vector of Vehicles. hand_over takes the
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

// [Appendix xviii] Copying a Vehicle of the Design: does not compile. The code and its error are in
// ../not_compiling/appendix_xviii_copying_a_vehicle.cpp.

// compile_time::Vehicle is a copy of the Vehicle of [Slide 80], with no virtual
// method, for One Overload per Class below.
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

// ---------------------------------------------------------------------------
// Frames that continue a lecture slide, the discussions, in the order of the
// appendix.
// ---------------------------------------------------------------------------

// [Appendix xxi] Each Route Has Its Own Positions
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

// [Appendix xxiv] A Route for One Call
// l8::Vehicle (../include/l8_vehicle.hpp) is the concrete class
// the students already have: its drive() prints the route's length and
// length / speed. The frame says so; the Abstraction section makes drive()
// pure virtual (../include).
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

// [Appendix xxv] Inside a Taxi Object
// RoboTaxi and Taxi on L8's Vehicle (l8::Vehicle), as Live Coding: Step 3
// writes them: copies of the two classes of
// inheritance.cpp.
// l8::Vehicle: int id_ (4 bytes, then 4 bytes of padding), two doubles (16),
// color_, a std::string (32 with libstdc++), mileage_, a double (8), and a
// std::vector (24 with libstdc++): 88. RoboTaxi adds a std::string (32) and an
// AutomationLevel (4, as an int, then 4 bytes of padding): 40 more, 128. Taxi
// adds a std::string: 32 more, 120.
namespace inside {
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
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
            << sizeof(Taxi) << '\n';         // 88 128 120
  std::cout << sizeof(std::string) << '\n';  // 32
}
}  // namespace inside

// [Appendix xxviii] One Overload per Class. RoboTaxi and Taxi redefine drive(),
// as on [Slide 80], Redefinition: copies of the classes of
// polymorphism.cpp, on the compile_time::Vehicle above.
namespace compile_time {
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


// [Appendix xxxi] Discussion: What Prints First? A Taxi whose
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

// [Appendix xxxiii] Discussion: Add a RoboBus. One answer: a new
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

// [Appendix xxxv] std::find on a Vector: code to copy for Live Coding: Step 2,
// on a vector of numbers. std::find is in <algorithm>.
namespace find_value {
void run() {
  std::vector<int> numbers{4, 8, 15, 16, 23, 42};
  // std::find returns end() when 15 is not there
  if (std::find(numbers.begin(), numbers.end(), 15) != numbers.end()) {
    std::cout << "15 is there\n";
  }
  // add 7 only if it is not there yet
  if (std::find(numbers.begin(), numbers.end(), 7) == numbers.end()) {
    numbers.push_back(7);
  }
  std::cout << numbers.size() << '\n';
}
}  // namespace find_value

// [Appendix xxxvi] std::erase_if on a Vector: code to copy for Live Coding:
// Step 2. The std::erase_if for a vector is in <vector> (C++20).
namespace erase_value {
void run() {
  std::vector<int> numbers{4, 8, 15, 16, 23, 42};
  // erase every element the lambda accepts; it returns how many it erased
  auto erased{std::erase_if(numbers, [](int n) { return n % 2 == 0; })};
  std::cout << erased << " erased, " << numbers.size() << " left\n";
  int target{23};
  std::erase_if(numbers, [target](int n) { return n == target; });
  for (int n : numbers) {
    std::cout << n << ' ';
  }
  std::cout << '\n';
}
}  // namespace erase_value

// [Appendix xxxvii] Owners Keep Objects Alive, on L8's Vehicle (l8::Vehicle)
// car_a is the only owner of its Vehicle. car_b and copy_b are two owners of
// one Vehicle; watcher observes it without owning it, so use_count() is 2.
// The Vehicle is destroyed when the last owner lets go: then watcher expires.
namespace ownership {
using l8::Vehicle;

void run() {
  auto car_a{std::make_unique<Vehicle>(13.4)};
  auto car_b{std::make_shared<Vehicle>(13.4)};
  auto copy_b{car_b};  // a second owner
  std::weak_ptr<Vehicle> watcher{car_b};  // not an owner
  std::cout << std::boolalpha << car_b.use_count()
            << ' ' << watcher.expired() << '\n';  // 2 false
  car_b.reset();
  copy_b.reset();  // the last owner lets go
  std::cout << watcher.expired() << '\n';  // true: the Vehicle is destroyed
}
}  // namespace ownership

// [Appendix xl] A Typo That Compiles: no override.
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
// [Appendix xli] override Catches Both. With override, both stop the build.
}  // namespace typo

std::vector<Slide> slides() {
  std::vector<Slide> frames{
      {4, "No Vehicle Objects", no_vehicle::run},
      {5, "RoboTaxi and Taxi", concrete::run},
      {8, "Multiple Inheritance, the Code", multiple::run},
      {12, "protected and private Inheritance, the Code", [] { private_base::run(); protected_base::run(); }},
      {15, "Two Pointers, Two Static Types", runtime::auto_calls::run},
      {16, "From auto into a Vector of Vehicles", runtime::auto_fleet::run},
      {21, "Each Route Has Its Own Positions", composition::run},
      {24, "A Route for One Call", dependency::run},
      {25, "Inside a Taxi Object", inside::run},
      {28, "One Overload per Class", compile_time::overloads::run},
      {31, "Discussion: What Prints First?", order::run},
      {33, "Discussion: Add a RoboBus", robobus::run},
      {35, "std::find on a Vector", find_value::run},
      {36, "std::erase_if on a Vector", erase_value::run},
      {37, "Owners Keep Objects Alive", ownership::run},
      {40, "A Typo That Compiles", typo::run},
  };
  for (Slide& frame : frames) {
    frame.part = Part::appendix;
  }
  return frames;
}

}  // namespace appendix
