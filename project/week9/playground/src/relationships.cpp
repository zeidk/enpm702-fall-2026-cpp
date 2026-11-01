/**
 * @file relationships.cpp
 * @brief L9 Section 2, Class Relationships: association and aggregation.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. These slides come before inheritance, so
 * they use L8's Vehicle (l8::Vehicle, in ../include/l8_vehicle.hpp): a concrete
 * class whose drive() prints the trip. Passenger and Fleet are written here
 * against it; the final versions, in ../../common, are the same code against
 * the abstract Vehicle of Section 4.
 *
 * The code of the composition and dependency frames, Each Route Has Its Own
 * Positions and A Route for One Call, is in ../../appendix/src/appendix.cpp.
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
#include "sections.hpp"

namespace relationships {

using l8::Vehicle;

// [Slide 5] Owners Keep Objects Alive
// car_a is the only owner of its Vehicle. car_b and copy_b are two owners of
// one Vehicle; watcher observes it without owning it, so use_count() is 2.
// The Vehicle is destroyed when the last owner lets go: then watcher expires.
namespace ownership {
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

// Fleet, as Live Coding: Step 2 writes it in ../../live. [Slide 34] runs it.
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

// [Slide 34] The Vehicles Outlive the Fleet
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

std::vector<Slide> slides() {
  return {
      {5, "Owners Keep Objects Alive", ownership::run},
      {26, "Two Separate Lifetimes", association::run},
      {34, "The Vehicles Outlive the Fleet", aggregation::run},
  };
}

}  // namespace relationships
