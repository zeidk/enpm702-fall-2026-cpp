/**
 * @file destructors.cpp
 * @brief L8, Destructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The slides show when a destructor runs
 * ([Slides 87 and 88]) and an accessor that returns the sensors by const&
 * ([Slide 90]), which uses the finished class, ../../common/include/vehicle.hpp.
 * We write the sensors_ member together in Live Coding Step 6 ([Slide 91]),
 * in ../../live/src/main.cpp (week8_live).
 *
 * appendix_slides(), which week8_appendix runs, lists the appendix frame Many
 * Sensors ([Appendix x]) and the rule of zero of the reading page Rule of Five
 * and Rule of Zero on Read the Docs ([Reading: zero]): 702run week8_appendix zero.
 * The Vehicle that owns its Sensor through a raw pointer, and the rule of five,
 * are in ../../appendix/src/appendix.cpp.
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "sections.hpp"
#include "sensor.hpp"
#include "vehicle.hpp"

namespace destructors {

// [Slide 87] Destructor and [Slide 88] When It Runs: the class of
// [Appendix ix], The Class So Far, with a destructor that prints the speed.
namespace when_it_runs {
class Vehicle {
 public:
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
    set_speed(speed);
  }
  ~Vehicle() {  // prints, to show when it runs
    std::cout << "~Vehicle: speed " << speed_ << '\n';
  }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  void set_speed(double speed) {
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }
  void add_mileage(double meters) {
    if (meters < 0.0) {
      throw std::invalid_argument{"a trip's distance must be at least 0"};
    }
    mileage_ += meters;
  }

 private:
  double max_speed_;     // m/s, greater than 0
  double speed_{0.0};    // m/s, from 0 to max_speed_
  std::string color_;    // from the constructor
  double mileage_{0.0};  // m driven, never goes down
};

void run() {
  {
    Vehicle car_a{13.4, 12.0};
    Vehicle car_b{13.4, 10.0};
    std::cout << "end of scope\n";
  }
  auto spare_car{
      std::make_unique<Vehicle>(13.4, 5.0)};
  spare_car.reset();
  std::cout << "after reset\n";
}  // prints: end of scope, ~Vehicle: speed 10, ~Vehicle: speed 12,
   // ~Vehicle: speed 5, after reset
}  // namespace when_it_runs

// [Reading: zero] Rule of Zero, and What the Compiler Writes Now: the reading
// page Rule of Five and Rule of Zero on Read the Docs. 702run week8_appendix zero
namespace zero {
class Vehicle {  // one Sensor only, before sensors_
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{std::make_unique<Sensor>(sensor)} {}
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  std::unique_ptr<Sensor> sensor_;  // deletes the Sensor
};

void run() {
  Vehicle car_b{Sensor{2, SensorType::camera, 80.0}};  // one Sensor only
  Vehicle moved{std::move(car_b)};  // OK: moves the unique_ptr
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
  // Vehicle copy{moved};  // does not compile:
  // error: use of deleted function 'destructors::zero::Vehicle::Vehicle(const destructors::zero::Vehicle&)'
  // note: 'destructors::zero::Vehicle::Vehicle(const destructors::zero::Vehicle&)' is implicitly deleted because the
  //       default definition would be ill-formed:
  // (the page's code has no namespaces, so the page's error names Vehicle only)
}
}  // namespace zero

// [Appendix x] Many Sensors: the sensors_ of the class diagram. The finished
// class, with the speed members and the id, is ../../common/include/vehicle.hpp.
namespace many_sensors {
class Vehicle {  // owns one Sensor for each sensor it carries
 public:
  int add_sensor(SensorType type, double range) {
    const int id{static_cast<int>(sensors_.size()) + 1};
    sensors_.push_back(std::make_unique<Sensor>(id, type, range));
    return id;
  }
  [[nodiscard]] std::size_t sensor_count() const noexcept {
    return sensors_.size();
  }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
};

void run() {
  Vehicle car_a;
  const int lidar{car_a.add_sensor(SensorType::lidar, 100.0)};
  const int camera{car_a.add_sensor(SensorType::camera, 80.0)};
  std::cout << lidar << ' ' << camera << ' ' << car_a.sensor_count() << '\n';  // 1 2 2
  // Vehicle copy{car_a};  // does not compile. The errors come from the
  // standard library code that copies the std::vector (stl_uninitialized.h,
  // stl_construct.h): it cannot copy a std::unique_ptr. The first one is
  // error: static assertion failed: result type must be constructible from input type
}
}  // namespace many_sensors

// [Slide 90] Returning a Large Member: get_sensors(), the accessor of the
// finished Vehicle in ../../common/include/vehicle.hpp.
namespace large_member {
void run() {
  Vehicle car_a{13.4};
  car_a.add_sensor(SensorType::lidar, 100.0);  // id 1
  car_a.add_sensor(SensorType::camera, 80.0);  // id 2
  for (const std::unique_ptr<Sensor>& sensor :
       car_a.get_sensors()) {
    std::cout << sensor->get_id() << ' ' << sensor->get_range() << '\n';
  }  // prints 1 100, then 2 80
  // An accessor that returns a copy does not compile: each std::unique_ptr
  // would be copied. In the class in vehicle.hpp:
  // [[nodiscard]] std::vector<std::unique_ptr<Sensor>> get_sensors_copy() const {
  //   return sensors_;
  // }
}
}  // namespace large_member

std::vector<Slide> slides() {
  return {
      {87, "Destructor", when_it_runs::run},
      {88, "When It Runs", when_it_runs::run},
      {90, "Returning a Large Member", large_member::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
      {10, "Many Sensors", many_sensors::run},
      {0, "Rule of Zero", zero::run, false, "zero"},
  };
}

}  // namespace destructors
