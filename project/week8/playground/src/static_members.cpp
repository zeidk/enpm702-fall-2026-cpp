/**
 * @file static_members.cpp
 * @brief L8 Section 9, static Members: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. Each slide's Vehicle is its own class, so
 * its counter starts at 1 whatever ran before: the Vehicle of
 * ../../common/include/vehicle.hpp has the same static counter, and in a full
 * run the earlier slides have already used some of its ids.
 */
#include <iostream>

#include "sections.hpp"

namespace static_members {

// [Slide 79] Two Vehicles, the Same Id
namespace same_ids {
class Vehicle {
 public:
  explicit Vehicle(double max_speed)
      : id_{next_id_++}, max_speed_{max_speed} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }

 private:
  int next_id_{1};  // one counter in each object
  int id_;
  double max_speed_;
};

void run() {
  Vehicle downtown_car{13.4};
  Vehicle airport_car{13.4};
  std::cout << downtown_car.get_id() << ' '
            << airport_car.get_id() << '\n';  // 1 1
}
}  // namespace same_ids

// [Slide 80] One Counter for the Class
namespace one_counter {
class Vehicle {
 public:
  explicit Vehicle(double max_speed)
      : id_{next_id_++}, max_speed_{max_speed} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the class
  int id_;
  double max_speed_;
};

void run() {
  std::cout << Vehicle::next_id() << '\n';  // 1: no Vehicle yet
  Vehicle downtown_car{13.4};
  Vehicle airport_car{13.4};
  std::cout << downtown_car.get_id() << ' ' << airport_car.get_id() << ' '
            << Vehicle::next_id() << '\n';  // 1 2 3
}
}  // namespace one_counter

// [Slide 81] A static Method Has No this
// Inside Vehicle (../../common/include/vehicle.hpp):
// static std::size_t count() noexcept {  // in Vehicle: an error
//   return sensors_.size();
// }
// error: invalid use of member 'Vehicle::sensors_' in static member function

std::vector<Slide> slides() {
  return {
      {79, "Two Vehicles, the Same Id", same_ids::run},
      {80, "One Counter for the Class", one_counter::run},
  };
}

}  // namespace static_members
