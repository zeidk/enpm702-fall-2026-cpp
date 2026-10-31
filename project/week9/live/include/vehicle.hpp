/**
 * @file vehicle.hpp
 * @brief L9, live coding: the class Vehicle, given. Read it before Step 1.
 * @author Zeid Kootbally
 *
 * @details Given, not written in class: its speed stays from 0 to its maximum
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

#include "route.hpp"   // Step 4: drive() takes a Route
#include "sensor.hpp"  // Sensor and SensorType

namespace fleet {

/// A vehicle of the robotaxi fleet: its speed stays from 0 to its maximum speed.
class Vehicle {
 public:
  /// Throws std::invalid_argument if @p max_speed is not greater than 0.
  /// The member initializer list sets id_, max_speed_ and color_, then the body
  /// checks the maximum speed.
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }

  /// The delegating constructor. Throws std::invalid_argument if either
  /// value breaks the invariant.
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }

  // Step 5: the destructor, virtual and = default.

  // Step 4: the virtual method drive(), which takes a const Route& and is const.
  //         Its body goes to vehicle.cpp.
  // Step 6: drive() becomes pure virtual (= 0).

  /// Throws std::invalid_argument if @p speed is not from 0 to the maximum speed.
  void set_speed(double speed);
  [[nodiscard]] double get_speed() const noexcept;
  [[nodiscard]] const std::string& get_color() const noexcept;
  /// The maximum speed, in m/s.
  [[nodiscard]] double get_max_speed() const noexcept;

  /// The distance driven so far, in m.
  [[nodiscard]] double get_mileage() const noexcept;
  /// Adds the distance of a trip to the mileage. Throws std::invalid_argument
  /// if @p meters is less than 0: the mileage never goes down.
  void add_mileage(double meters);

  /// Returns the new Sensor's id: 1, 2, ...
  int add_sensor(SensorType type, double range);
  [[nodiscard]] std::size_t sensor_count() const noexcept;
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept;

  /// The id, and the id the next Vehicle gets.
  [[nodiscard]] int get_id() const noexcept;
  [[nodiscard]] static int next_id() noexcept;

 protected:
  /// Prints one line about this vehicle: "vehicle <id>: <event>". For the
  /// methods of Vehicle and of the classes derived from it.
  void log_event(const std::string& event) const;

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
