/**
 * @file inheritance.cpp
 * @brief L9 Section 2, Class Relationships: inheritance.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. RoboTaxi and Taxi derive from L8's
 * Vehicle (l8::Vehicle), which has no virtual method yet: that comes in
 * Section 3. Both use L8's drive(), which prints the trip.
 *
 * The slides that do not compile are kept here as comments, with the error
 * g++ 13.3 gives (-std=c++20 -Wall -Wextra -pedantic-errors -Wshadow).
 */
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "l8_vehicle.hpp"
#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"

namespace inheritance {

// [Slide 37] RoboTaxi Is a Vehicle
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
};

// [Slide 38] Taxi Is a Vehicle Too
class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;  // empty: no driver
};

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  return trip;
}

namespace robotaxi_is_a {
void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.add_sensor(SensorType::lidar, 100.0);  // Vehicle's methods,
  robotaxi.set_speed(10.0);                       // all inherited
  std::cout << robotaxi.sensor_count() << '\n';   // 1
  robotaxi.drive(make_trip());                    // 500 m at 10 m/s: 50 s
}
}  // namespace robotaxi_is_a

// [Slide 39] Inside a Taxi Object
// l8::Vehicle: int id_ (4 bytes, then 4 bytes of padding), two doubles (16),
// a std::vector (24 with libstdc++): 48. Taxi adds a std::string: 32 more.
namespace inside {
void run() {
  Taxi taxi{13.4, "Ana"};
  l8::Vehicle& vehicle{taxi};  // a Taxi is a Vehicle: no cast
  std::cout << sizeof(l8::Vehicle) << ' ' << sizeof(RoboTaxi) << ' '
            << sizeof(Taxi) << '\n';  // 48 48 80
  std::cout << &taxi << '\n' << &vehicle << '\n';  // the same address
  std::cout << sizeof(std::string) << '\n';  // 32
}
}  // namespace inside

namespace taxi_too {
void run() {
  Taxi taxi{13.4, "Ana"};
  taxi.set_speed(10.0);
  taxi.drive(make_trip());  // 500 m at 10 m/s: 50 s
}
}  // namespace taxi_too

// [Slide 41] private Stays private. Does not compile:
//   void go() { speed_ = get_max_speed(); }  // inside RoboTaxi
// error: 'double l8::Vehicle::speed_' is private within this context
// The fix the slide gives: set_speed(get_max_speed());

// [Slide 43] public Inheritance. With "class RoboTaxi : Vehicle"
// (no public), the base is private, and main() cannot call get_speed():
// error: 'double Vehicle::get_speed() const' is inaccessible within this context
// error: 'Vehicle' is not an accessible base of 'RoboTaxi'

// [Slide 45] Without the Base Class Initializer. Does not compile:
//   explicit RoboTaxi(double max_speed) {}
// error: no matching function for call to 'Vehicle::Vehicle()'

// [Slide 46] A Base Member in the List. Does not compile:
//   explicit RoboTaxi(double max_speed) : max_speed_{max_speed} {}
// error: 'double Vehicle::max_speed_' is private within this context
// error: class 'RoboTaxi' does not have any field named 'max_speed_'
// error: no matching function for call to 'Vehicle::Vehicle()'

// [Slide 47] Construction and Destruction Order. Both classes
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
      {37, "RoboTaxi Is a Vehicle", robotaxi_is_a::run},
      {38, "Taxi Is a Vehicle Too", taxi_too::run},
      {39, "Inside a Taxi Object", inside::run},
      {47, "Construction and Destruction Order", order::run},
  };
}

}  // namespace inheritance
