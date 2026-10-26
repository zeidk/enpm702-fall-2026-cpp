/**
 * @file vehicle.hpp
 * @brief The robotaxi's speed: the first class of L8.
 * @author Zeid Kootbally
 *
 * @details A Vehicle holds the maximum speed of the robotaxi and the speed it
 * drives at. The speed must stay between 0 and the maximum speed. That rule is
 * the class's invariant: every constructor and every method that changes a
 * member checks it, so no Vehicle object ever breaks it.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
#pragma once

/// Holds the speed of a robotaxi, between 0 and its maximum speed.
class Vehicle {
 public:
  /**
   * @brief Starts with a speed of 0.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @throw std::invalid_argument if @p max_speed is not greater than 0.
   */
  explicit Vehicle(double max_speed);

  /**
   * @brief Starts with the given speed.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param speed The speed to drive at, in m/s, from 0 to @p max_speed.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  Vehicle(double max_speed, double speed);

  /// The maximum speed, in m/s.
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

  /// The speed the robotaxi drives at, in m/s.
  [[nodiscard]] double get_speed() const noexcept { return speed_; }

  /**
   * @brief Sets the speed to drive at.
   * @param speed In m/s, from 0 to the maximum speed.
   * @throw std::invalid_argument if @p speed is outside that range.
   */
  void set_speed(double speed);

 private:
  double max_speed_;    ///< m/s, greater than 0
  double speed_{0.0};   ///< m/s, from 0 to max_speed_
};
