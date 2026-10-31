/**
 * @file relationships.cpp
 * @brief L9 Section 2, Class Relationships: association, aggregation,
 *        composition and dependency.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. These slides come before inheritance, so
 * they use L8's Vehicle (l8::Vehicle, in ../include/l8_vehicle.hpp): a concrete
 * class whose drive() prints the trip. Passenger and Fleet are written here
 * against it; the final versions, in ../../common, are the same code against
 * the abstract Vehicle of Section 4.
 *
 * The routes are chosen for the examples: (0, 0) to (300, 400) is 500 m, the
 * hypotenuse of a 300-400-500 triangle.
 */
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "l8_vehicle.hpp"
#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"

namespace relationships {

using fleet::Position;
using fleet::Route;
using l8::Vehicle;

// [Appendix iii] A Passenger Rides a Vehicle. The class only: [Slide 26] runs it,
// and Live Coding: Step 1 writes it in ../../live.
class Passenger {
 public:
  explicit Passenger(std::string name) : name_{std::move(name)} {
    if (name_.empty()) {
      throw std::invalid_argument{"a passenger has a name"};
    }
  }
  void board(const std::shared_ptr<Vehicle>& vehicle) { ride_ = vehicle; }
  void leave() noexcept { ride_.reset(); }

 private:
  std::string name_;
  std::weak_ptr<Vehicle> ride_;  // does not own the Vehicle
};

// [Slide 26] Two Separate Lifetimes
// watcher is a second std::weak_ptr, only for the check: expired() is true
// once the Vehicle is destroyed, while maya's std::weak_ptr still refers to it.
namespace association {
void run() {
  auto car_a{std::make_shared<Vehicle>(13.4)};
  Passenger maya{"Maya"};
  std::cout << car_a.use_count() << '\n';  // 1
  maya.board(car_a);
  std::cout << car_a.use_count() << '\n';  // 1: maya is not an owner
  std::weak_ptr<Vehicle> watcher{car_a};   // does not own it either
  car_a.reset();                           // the only owner lets go
  std::cout << std::boolalpha
            << watcher.expired() << '\n';  // true: the Vehicle is destroyed
  maya.leave();                            // maya still exists
}
}  // namespace association

// Fleet, as Live Coding: Step 2 writes it in ../../live. [Slide 35] runs it.
// The same code as ../../common/src/fleet.cpp.
class Fleet {
 public:
  void add(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
      throw std::invalid_argument{"a fleet cannot hold a null vehicle"};
    }
    if (std::find(vehicles_.begin(), vehicles_.end(), vehicle) == vehicles_.end()) {
      vehicles_.push_back(std::move(vehicle));
    }
  }
  bool remove(int id) {
    return std::erase_if(vehicles_, [id](const std::shared_ptr<Vehicle>& vehicle) {
             return vehicle->get_id() == id;
           }) > 0;
  }
  [[nodiscard]] std::size_t size() const noexcept { return vehicles_.size(); }

 private:
  std::vector<std::shared_ptr<Vehicle>> vehicles_;  // shares each Vehicle
};

// [Slide 35] The Vehicles Outlive the Fleet
namespace aggregation {
void run() {
  auto car_a{std::make_shared<Vehicle>(13.4)};
  auto car_b{std::make_shared<Vehicle>(13.4)};
  {
    Fleet fleet;
    fleet.add(car_a);
    fleet.add(car_b);
    std::cout << fleet.size() << ' ' << car_a.use_count() << '\n';  // 2 2
    fleet.remove(car_b->get_id());
    std::cout << fleet.size() << ' ' << car_b.use_count() << '\n';  // 1 1
  }  // the Fleet is destroyed here, not the vehicles
  std::cout << car_a.use_count() << ' '
            << car_a->get_max_speed() << '\n';  // 1 13.4
}
}  // namespace aggregation

// [Slide 43] Each Route Has Its Own Positions
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

// [Slide 47] A Route for One Call
namespace dependency {
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

std::vector<Slide> slides() {
  return {
      {26, "Two Separate Lifetimes", association::run},
      {35, "The Vehicles Outlive the Fleet", aggregation::run},
      {43, "Each Route Has Its Own Positions", composition::run},
      {47, "A Route for One Call", dependency::run},
  };
}

}  // namespace relationships
