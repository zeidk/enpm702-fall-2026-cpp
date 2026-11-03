/**
 * @file vehicle.hpp
 * @brief L8, live coding: the class Vehicle, finished.
 * @author Zeid Kootbally
 *
 * @details A Vehicle holds its maximum speed, its speed, its color and its
 * mileage (the distance it has driven). Its invariant: the speed stays from 0 to
 * the maximum speed. It owns its sensors, each through a std::unique_ptr, so it
 * writes none of the five special member functions (the rule of zero).
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
#pragma once

#include <cstddef>    // std::size_t
#include <memory>     // std::unique_ptr
#include <stdexcept>  // std::invalid_argument
#include <string>
#include <vector>

#include "sensor.hpp"  // Sensor and SensorType, in ../../common/include

/**
 * @brief A vehicle of the robotaxi fleet.
 *
 * Invariant: its maximum speed is greater than 0, and its speed stays from 0
 * to its maximum speed. It owns its sensors (composition).
 */
class Vehicle {
 public:
  /**
   * @brief A vehicle that stands still.
   *
   * The member initializer list sets max_speed_ and color_, then the body
   * checks the maximum speed.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param color The color of the body.
   * @throw std::invalid_argument if @p max_speed is not greater than 0.
   */
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }

  /**
   * @brief A vehicle that already moves: the delegating constructor.
   *
   * The first constructor checks the maximum speed, then set_speed() checks
   * the speed.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param speed The speed, in m/s. Must be from 0 to @p max_speed.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }

  /**
   * @brief Sets the speed.
   * @param speed The new speed, in m/s.
   * @throw std::invalid_argument if @p speed is not from 0 to the maximum speed.
   */
  void set_speed(double speed);
  /**
   * @brief Returns the speed.
   * @return The speed, in m/s.
   */
  [[nodiscard]] double get_speed() const noexcept;

  /**
   * @brief Returns the color of the body.
   * @return The color given to the constructor.
   */
  [[nodiscard]] const std::string& get_color() const noexcept;

  /**
   * @brief Adds a sensor that the vehicle owns.
   * @param type What kind of sensor.
   * @param range How far it sees, in m. Must be at least 0.
   * @return The new Sensor's id: 1, 2, ...
   * @throw std::invalid_argument if @p range is less than 0 (from Sensor's
   *        constructor).
   */
  int add_sensor(SensorType type, double range);
  /**
   * @brief Returns how many sensors the vehicle has.
   * @return The number of elements in sensors_.
   */
  [[nodiscard]] std::size_t sensor_count() const noexcept;
  /**
   * @brief Gives read access to the sensors.
   * @return The sensors, in the order they were added. The vehicle still owns
   *         them.
   */
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept;

 private:
  double max_speed_;              ///< m/s, greater than 0
  double speed_{0.0};             ///< m/s, from 0 to max_speed_
  std::string color_;             ///< set by the constructor
  double mileage_{0.0};           ///< m driven
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< the only owner of each Sensor
};
