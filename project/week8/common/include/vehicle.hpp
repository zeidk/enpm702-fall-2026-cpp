/**
 * @file vehicle.hpp
 * @brief A vehicle of the robotaxi fleet: the class L8 builds.
 * @author Zeid Kootbally
 *
 * @details A Vehicle holds its maximum speed, the speed it drives at, its color
 * and its mileage (the distance it has driven). The speed must stay between 0 and
 * the maximum speed, and the mileage never goes down. That rule is the class's
 * invariant: every constructor and every method that changes a member checks
 * it, so no Vehicle object ever breaks it.
 *
 * It owns its sensors, each through a std::unique_ptr, so it defines none of
 * the five special member functions that manage resources (the rule of zero):
 * a Vehicle can be moved and cannot be copied. Its id comes from a static
 * counter, shared by every Vehicle, so no two vehicles get the same id.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 *
 * The slides build this class in steps; each step is its own class in the
 * playground. In L9, drive() becomes pure virtual.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "route.hpp"
#include "sensor.hpp"

/// A vehicle: its speed stays between 0 and its maximum speed.
class Vehicle {
 public:
  /**
   * @brief Starts with a speed of 0, a mileage of 0 and no sensor.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param color Its color. Any color name is a color, so there is nothing to check.
   * @throw std::invalid_argument if @p max_speed is not greater than 0.
   */
  explicit Vehicle(double max_speed, const std::string& color = "white");

  /**
   * @brief Starts with the given speed. The color is "white".
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param speed The speed to drive at, in m/s, from 0 to @p max_speed.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  Vehicle(double max_speed, double speed);

  /// Its id, from the counter of the class.
  [[nodiscard]] int get_id() const noexcept { return id_; }

  /// The color, given to the constructor.
  [[nodiscard]] const std::string& get_color() const noexcept { return color_; }

  /// The maximum speed, in m/s.
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

  /// The speed the vehicle drives at, in m/s.
  [[nodiscard]] double get_speed() const noexcept { return speed_; }

  /**
   * @brief Sets the speed to drive at.
   * @param speed In m/s, from 0 to the maximum speed.
   * @throw std::invalid_argument if @p speed is outside that range.
   */
  void set_speed(double speed);

  /// The distance driven so far, in m.
  [[nodiscard]] double get_mileage() const noexcept { return mileage_; }

  /**
   * @brief Adds the distance of a trip to the mileage.
   * @param meters The trip's distance, in m. At least 0: the mileage never goes down.
   * @throw std::invalid_argument if @p meters is less than 0.
   */
  void add_mileage(double meters);

  /**
   * @brief Mounts a new sensor. The Vehicle owns it.
   * @param type What kind of sensor: lidar, camera or radar.
   * @param range How far it sees, in m. Must be at least 0.
   * @return The new Sensor's id: 1 for the first sensor, 2 for the second, ...
   * @throw std::invalid_argument if @p range is less than 0.
   */
  int add_sensor(SensorType type, double range);

  /// How many sensors the vehicle carries.
  [[nodiscard]] std::size_t sensor_count() const noexcept { return sensors_.size(); }

  /// The sensors, by const reference: no copy, and the caller cannot change them.
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept {
    return sensors_;
  }

  /// Prints the trip: the route's length, and the time it takes at the speed.
  void drive(const Route& route) const;

  /// The id the next Vehicle gets.
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  ///< one counter for the whole class
  int id_;                        ///< from next_id_
  double max_speed_;              ///< m/s, greater than 0
  double speed_{0.0};             ///< m/s, from 0 to max_speed_
  std::string color_;             ///< from the constructor
  double mileage_{0.0};           ///< m driven, never goes down
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< the only owner of each
};
