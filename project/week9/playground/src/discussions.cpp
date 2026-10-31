/**
 * @file discussions.cpp
 * @brief L9: the code behind the discussion slides, runnable. Discuss first.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground.
 */
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "fleet.hpp"
#include "passenger.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "sections.hpp"
#include "sensor.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace discussions {

using fleet::Fleet;
using fleet::Position;
using fleet::RoboTaxi;
using fleet::Route;
using fleet::Sensor;
using fleet::Taxi;
using fleet::Vehicle;

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route maya_ride;
  maya_ride.add(Position{0.0, 0.0});
  maya_ride.add(Position{300.0, 400.0});
  return maya_ride;
}

// [Slide 66] Discussion: What Prints First? A Taxi whose
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
// warning: 'discussions::order::Taxi::driver_name_' will be initialized after [-Wreorder]
// warning:   base 'discussions::order::Vehicle' [-Wreorder]
}  // namespace order

// [Slide 84] Discussion: Which drive() Runs?
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
  robotaxi.drive(maya_ride);      // 1: RoboTaxi: drives itself 500 m
  ref.drive(maya_ride);           // 2: RoboTaxi: drives itself 500 m
  ptr->drive(maya_ride);          // 3: RoboTaxi: drives itself 500 m
  copy.drive(maya_ride);          // 4: Vehicle: drives 500 m
  ref.Vehicle::drive(maya_ride);  // 5: Vehicle: drives 500 m
}
}  // namespace which

// [Slide 94] Discussion: Who Needs a virtual Destructor?
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

// [Slide 103] Discussion: Add a RoboBus. One answer: a new
// class, and no change to Fleet. Its invariant: more than 0 seats.
namespace robobus {
class RoboBus final : public Vehicle {
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

std::vector<Slide> slides() {
  return {
      {66, "Discussion: What Prints First?", order::run},
      {84, "Discussion: Which drive() Runs?", which::run},
      {94, "Discussion: Who Needs a virtual Destructor?", who_needs::run},
      {103, "Discussion: Add a RoboBus", robobus::run},
  };
}

}  // namespace discussions
