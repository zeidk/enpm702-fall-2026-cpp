/**
 * @file fleet.hpp
 * @brief L9, live coding: L8's Vehicle, and the classes of L9, which we write in
 *        class, step by step.
 * @author Zeid Kootbally
 *
 * @details One header for the five classes of the "Live Coding" slides of L9.
 * Vehicle is L8's class, as the L8 live coding finished it: its invariant is
 * that the speed stays from 0 to the maximum speed. L9 changes it, so it is in
 * this file too. Each "Step N" comment below says what that step adds or
 * changes here:
 * - Passenger rides a Vehicle without owning it (association, Step 1).
 * - Fleet shares its vehicles with the rest of the program (aggregation,
 *   Step 2) and drives them all in one loop (Step 4).
 * - RoboTaxi and Taxi are two kinds of Vehicle (inheritance, Step 3).
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>  // Step 3: std::move
#include <vector>

#include "route.hpp"   // Step 4: drive() takes a Route, in ../../common/include
#include "sensor.hpp"  // L8: Sensor and SensorType, in ../../common/include

/// A vehicle of the robotaxi fleet: its speed stays from 0 to its maximum speed.
class Vehicle {
 public:
  /// L8. Throws std::invalid_argument if @p max_speed is not greater than 0.
  explicit Vehicle(double max_speed, const std::string& color = "white");

  /// L8. Throws std::invalid_argument if either value breaks the invariant.
  Vehicle(double max_speed, double speed);

  // Step 5: the destructor, virtual and = default.

  // Step 4: the virtual method drive(), which takes a const Route& and is const.
  //         Its body goes to fleet.cpp.
  // Step 6: drive() becomes pure virtual (= 0).

  /// L8. Throws std::invalid_argument if @p speed is not from 0 to the maximum speed.
  void set_speed(double speed);
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  [[nodiscard]] const std::string& get_color() const noexcept { return color_; }

  /// L8. Returns the new Sensor's id: 1, 2, ...
  int add_sensor(SensorType type, double range);
  [[nodiscard]] std::size_t sensor_count() const noexcept { return sensors_.size(); }
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept {
    return sensors_;
  }

  /// L8. The id, and the id the next Vehicle gets.
  [[nodiscard]] int get_id() const noexcept { return id_; }
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  // one counter for the whole class
  int id_;                        // from next_id_
  double max_speed_;              // m/s, greater than 0
  double speed_{0.0};             // m/s, from 0 to max_speed_
  std::string color_;             // set by the constructor
  double mileage_{0.0};           // m driven
  std::vector<std::unique_ptr<Sensor>> sensors_;  // the only owner of each Sensor
};

// Step 1: the class Passenger. Its private data members are name_, a
//         std::string, and ride_, a std::weak_ptr<Vehicle>. Its public methods
//         are an explicit constructor that takes the name (its body, in
//         fleet.cpp, throws std::invalid_argument for an empty name),
//         board(const std::shared_ptr<Vehicle>&) and leave().

// Step 2: the class Fleet. Its private data member is vehicles_, a
//         std::vector<std::shared_ptr<Vehicle>>. Its public methods are
//         add(std::shared_ptr<Vehicle>), bool remove(int id) and size().
// Step 4: Fleet's method drive_all(const Route&), which is const.

// Step 3: the class RoboTaxi, derived from Vehicle, with an explicit constructor
//         that takes the maximum speed and passes it to Vehicle.
// Step 3: the class Taxi, derived from Vehicle, with the private data member
//         driver_name_, a std::string, and a constructor that takes the
//         maximum speed and the driver's name.
// Step 4: in both classes, drive(const Route&) const override.
// Step 7: both classes become final.
