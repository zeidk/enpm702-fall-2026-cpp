/**
 * @file static_members.cpp
 * @brief L8, static Members, in the appendix: the code of every frame,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details static Members is a part of the appendix, so week8_appendix runs
 * these frames: ../../appendix/src/appendix.cpp collects appendix_slides().
 * Each frame's Vehicle is its own class, so its counter starts at 1 whatever
 * ran before: the Vehicle of ../../common/include/vehicle.hpp has the same
 * static counter, and in a full run the earlier frames have already used some
 * of its ids.
 */
#include <iostream>
#include <string>

#include "sections.hpp"

namespace static_members {

// [Appendix xxii] Two Vehicles, the Same Id
namespace same_ids {
class Vehicle {
 public:
  explicit Vehicle(double max_speed,
                   const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }

 private:
  int next_id_{1};  // one counter in each object
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
};

void run() {
  Vehicle car_a{13.4};
  Vehicle car_b{13.4};
  std::cout << car_a.get_id() << ' '
            << car_b.get_id() << '\n';  // 1 1
}
}  // namespace same_ids

// [Appendix xxiii] One Counter for the Class
namespace one_counter {
class Vehicle {
 public:
  explicit Vehicle(double max_speed,
                   const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the class
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
};

void run() {
  std::cout << Vehicle::next_id() << '\n';  // 1: no Vehicle yet
  Vehicle car_a{13.4};
  Vehicle car_b{13.4};
  std::cout << car_a.get_id() << ' ' << car_b.get_id() << ' '
            << Vehicle::next_id() << '\n';  // 1 2 3
}
}  // namespace one_counter

// [Appendix xxiv] A static Method Has No this: next_id() is called on the
// class, with no object. The class is that of [Appendix xxiii], with a counter
// of its own, so it starts at 1 whatever ran before.
namespace static_method {
class Vehicle {
 public:
  explicit Vehicle(double max_speed,
                   const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the class
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
};

void run() {
  std::cout << Vehicle::next_id() << '\n';  // on the class, no object: 1
}
// Inside Vehicle (../../common/include/vehicle.hpp, which has sensors_):
// static std::size_t count() noexcept {  // in Vehicle: an error
//   return sensors_.size();
// }
// error: invalid use of member 'Vehicle::sensors_' in static member function
}  // namespace static_method

std::vector<Slide> appendix_slides() {
  return {
      {22, "Two Vehicles, the Same Id", same_ids::run},
      {23, "One Counter for the Class", one_counter::run},
      {24, "A static Method Has No this", static_method::run},
  };
}

}  // namespace static_members
