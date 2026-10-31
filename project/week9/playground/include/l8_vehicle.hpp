/**
 * @file l8_vehicle.hpp
 * @brief L8's Vehicle, as project/week8/common builds it, in namespace l8.
 * @author Zeid Kootbally
 *
 * @details The same members and the same code as L8's Vehicle, written inside
 * the class. The slides of Class Relationships use it: a concrete class whose
 * drive() prints the trip. [Slide 42] shows its sensors_, and [Slide 45] its
 * drive(). Section 4 makes drive() pure virtual; that version is
 * ../../common/include/vehicle.hpp.
 */
#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "route.hpp"
#include "sensor.hpp"

namespace l8 {

/// L8's Vehicle: its speed stays between 0 and its maximum speed.
class Vehicle {
 public:
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] const std::string& get_color() const noexcept { return color_; }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  void set_speed(double speed) {
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }
  [[nodiscard]] double get_mileage() const noexcept { return mileage_; }
  void add_mileage(double meters) {
    if (meters < 0.0) {
      throw std::invalid_argument{"a trip's distance must be at least 0"};
    }
    mileage_ += meters;
  }
  int add_sensor(fleet::SensorType type, double range) {
    const int id{static_cast<int>(sensors_.size()) + 1};
    sensors_.push_back(std::make_unique<fleet::Sensor>(id, type, range));
    return id;
  }
  [[nodiscard]] std::size_t sensor_count() const noexcept { return sensors_.size(); }
  [[nodiscard]] const std::vector<std::unique_ptr<fleet::Sensor>>& get_sensors() const noexcept {
    return sensors_;
  }
  /// Prints the trip: the route's length, and the time it takes at the speed.
  void drive(const fleet::Route& route) const {
    std::cout << route.length() << " m";
    if (speed_ > 0.0) {
      std::cout << " at " << speed_ << " m/s: " << route.length() / speed_ << " s\n";
    } else {
      std::cout << ", but the speed is 0\n";
    }
  }
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};
  int id_;
  double max_speed_;
  double speed_{0.0};
  std::string color_;
  double mileage_{0.0};
  std::vector<std::unique_ptr<fleet::Sensor>> sensors_;
};

}  // namespace l8
