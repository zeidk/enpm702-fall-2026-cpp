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

using l8::Vehicle;

// [Slide 20] A Passenger Rides a Vehicle
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

// [Slide 21] Two Separate Lifetimes
namespace association {
void run() {
  auto downtown_car{std::make_shared<Vehicle>(13.4)};
  Passenger maya{"Maya"};
  std::cout << downtown_car.use_count() << '\n';  // 1
  maya.board(downtown_car);
  std::cout << downtown_car.use_count() << '\n';  // 1: maya is not an owner
  downtown_car.reset();  // the only owner lets go: the Vehicle is destroyed
  maya.leave();          // maya still exists
  std::cout << std::boolalpha << (downtown_car == nullptr) << '\n';  // true
}
}  // namespace association

// [Slide 24] A Fleet of Shared Vehicles
class Fleet {
 public:
  void add(std::shared_ptr<Vehicle> vehicle) {
    if (!vehicle) {
      throw std::invalid_argument{"a fleet cannot hold a null vehicle"};
    }
    for (const std::shared_ptr<Vehicle>& held : vehicles_) {
      if (held == vehicle) { return; }  // already in the fleet
    }
    vehicles_.push_back(std::move(vehicle));
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

// [Slide 25] The Vehicles Outlive the Fleet
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

// [Slide 29] Each Route Has Its Own Positions
// A Route holds its Position objects by value (composition, as Vehicle holds
// its Sensors through std::unique_ptr in L8).
namespace composition {
void run() {
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  Route longer{trip};               // copies the Positions too
  longer.add(Position{300.0, 0.0});  // 400 m more
  std::cout << trip.length() << ' ' << longer.length() << '\n';  // 500 900
}
}  // namespace composition

// [Slide 33] A Route for One Call
namespace dependency {
void run() {
  Vehicle downtown_car{13.4};
  downtown_car.set_speed(10.0);
  {
    Route trip;
    trip.add(Position{0.0, 0.0});
    trip.add(Position{300.0, 400.0});
    downtown_car.drive(trip);  // 500 m at 10 m/s: 50 s
  }  // the Route is gone; the Vehicle kept nothing of it
  Route next;
  next.add(Position{0.0, 0.0});
  next.add(Position{0.0, 200.0});
  downtown_car.drive(next);    // 200 m at 10 m/s: 20 s
}
}  // namespace dependency

std::vector<Slide> slides() {
  return {
      {21, "Two Separate Lifetimes", association::run},
      {25, "The Vehicles Outlive the Fleet", aggregation::run},
      {29, "Each Route Has Its Own Positions", composition::run},
      {33, "A Route for One Call", dependency::run},
  };
}

}  // namespace relationships
