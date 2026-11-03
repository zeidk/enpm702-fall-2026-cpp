/**
 * @file static_members.cpp
 * @brief L8, static Members: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details static Members is the last part of the lecture, so week8_playground
 * runs these slides: main.cpp collects slides().
 * Each slide's Vehicle is its own class, so its counter starts at 1 whatever
 * ran before: the Vehicle of ../include/vehicle.hpp has the same
 * static counter, and in a full run the earlier frames have already used some
 * of its ids.
 */
#include <iostream>
#include <string>

#include "sections.hpp"

namespace static_members {

// [Slide 92] Two Vehicles, the Same Id
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
  std::cout << car_a.get_id() << '\n';  // 1
  std::cout << car_b.get_id() << '\n';  // 1
}
}  // namespace same_ids

// [Slide 93] One Counter for the Class
namespace one_counter {
class Vehicle {
 public:
  explicit Vehicle(double max_speed,
                   const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int get_next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the class
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
};

void run() {
  std::cout << Vehicle::get_next_id() << '\n';  // 1: no Vehicle yet
  Vehicle car_a{13.4};
  Vehicle car_b{13.4};
  std::cout << car_a.get_id() << ' ' << car_b.get_id() << ' '
            << Vehicle::get_next_id() << '\n';  // 1 2 3
}
}  // namespace one_counter

// [Slide 94] The Counter's First Value: without inline, two steps. The class
// declares the counter; one definition outside every function, at namespace
// scope (vehicle.cpp on the slide), gives it its first value. In one step,
// static int next_id_{1}; in the class does not compile:
// error: ISO C++ forbids in-class initialization of non-const static member
namespace counter_definition {
class Vehicle {
 public:
  Vehicle() : id_{next_id_++} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }

 private:
  static int next_id_;  // step 1: declared only
  int id_;
};

int Vehicle::next_id_{1};  // step 2: defined once, the first value

void run() {
  Vehicle car_a;
  Vehicle car_b;
  std::cout << car_a.get_id() << ' ' << car_b.get_id() << '\n';  // 1 2
}
}  // namespace counter_definition

// [Slide 95] A static Method Has No this: get_next_id() is called on the
// class, with no object. The class is that of [Slide 93], with a counter
// of its own, so it starts at 1 whatever ran before.
namespace static_method {
class Vehicle {
 public:
  explicit Vehicle(double max_speed,
                   const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {}
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int get_next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the class
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
};

void run() {
  std::cout << Vehicle::get_next_id() << '\n';  // on the class, no object: 1
}
// Inside Vehicle, which has speed_:
// static double get_speed_kmh() noexcept {  // in Vehicle: an error
//   return speed_ * 3.6;
// }
// error: invalid use of member 'Vehicle::speed_' in static member function
}  // namespace static_method

// [Slide 97] A Method That Uses No Member: mph_to_mps() reads no data member,
// so it needs no this. It is static and is called on the class.
namespace no_member {
class Vehicle {
 public:
  [[nodiscard]] static double mph_to_mps(double mph) noexcept;
};

double Vehicle::mph_to_mps(double mph) noexcept {
  return mph * 0.44704;
}

void run() {
  std::cout << Vehicle::mph_to_mps(30.0) << '\n';  // 13.4112: no object
}
}  // namespace no_member

std::vector<Slide> slides() {
  return {
      {92, "Two Vehicles, the Same Id", same_ids::run},
      {93, "One Counter for the Class", one_counter::run},
      {94, "The Counter's First Value", counter_definition::run},
      {95, "A static Method Has No this", static_method::run},
      {97, "A Method That Uses No Member", no_member::run},
  };
}

}  // namespace static_members
