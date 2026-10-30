/**
 * @file vehicle.hpp
 * @brief L9, live coding: the class Vehicle, which you write in class.
 * @author Zeid Kootbally
 *
 * @details The Vehicle you already wrote: its speed stays from 0 to its maximum
 * speed. L9 changes it: a virtual drive() (Step 4), a virtual destructor
 * (Step 5), and drive() pure virtual (Step 6), so Vehicle becomes abstract.
 * One class per file: the classes derived from Vehicle are in their own
 * files, and this file never names them.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "route.hpp"   // Step 4: drive() takes a Route, in ../../common/include
#include "sensor.hpp"  // Sensor and SensorType, in ../../common/include

namespace fleet {

/// A vehicle of the robotaxi fleet: its speed stays from 0 to its maximum speed.
class Vehicle {
 public:
  /// L8. Throws std::invalid_argument if @p max_speed is not greater than 0.
  /// The member initializer list sets id_, max_speed_ and color_, then the body
  /// checks the maximum speed.
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }

  /// L8: the delegating constructor. Throws std::invalid_argument if either
  /// value breaks the invariant.
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }

  // Step 5: the destructor, virtual and = default.

  // Step 4: the virtual method drive(), which takes a const Route& and is const.
  //         Its body goes to vehicle.cpp.
  // Step 6: drive() becomes pure virtual (= 0).

  /// L8. Throws std::invalid_argument if @p speed is not from 0 to the maximum speed.
  void set_speed(double speed);
  [[nodiscard]] double get_speed() const noexcept;
  [[nodiscard]] const std::string& get_color() const noexcept;

  /// L8. Returns the new Sensor's id: 1, 2, ...
  int add_sensor(SensorType type, double range);
  [[nodiscard]] std::size_t sensor_count() const noexcept;
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept;

  /// L8. The id, and the id the next Vehicle gets.
  [[nodiscard]] int get_id() const noexcept;
  [[nodiscard]] static int next_id() noexcept;

 private:
  inline static int next_id_{1};  // one counter for the whole class
  int id_;                        // from next_id_
  double max_speed_;              // m/s, greater than 0
  double speed_{0.0};             // m/s, from 0 to max_speed_
  std::string color_;             // set by the constructor
  double mileage_{0.0};           // m driven
  std::vector<std::unique_ptr<Sensor>> sensors_;  // the only owner of each Sensor
};

}  // namespace fleet
