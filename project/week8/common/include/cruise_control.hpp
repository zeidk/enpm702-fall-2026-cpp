/**
 * @file cruise_control.hpp
 * @brief The ego vehicle's cruise control: the first class of L8.
 * @author Zeid Kootbally
 *
 * @details The cruise control holds the speed the car tries to keep, the
 * target speed. The target speed must stay between 0 and the speed limit of
 * the road. That rule is the class's invariant: every constructor and every
 * method that changes a member checks it, so no CruiseControl object ever
 * breaks it.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph, the speed of L7's
 * oncoming car (chosen for that example).
 */
#pragma once

/// Holds the speed the car tries to keep, between 0 and the speed limit.
class CruiseControl {
 public:
  /**
   * @brief Starts with a target speed of 0.
   * @param speed_limit The road's speed limit, in m/s. Must be greater than 0.
   * @throw std::invalid_argument if @p speed_limit is not greater than 0.
   */
  explicit CruiseControl(double speed_limit);

  /**
   * @brief Starts with the given target speed.
   * @param speed_limit The road's speed limit, in m/s. Must be greater than 0.
   * @param target_speed The speed to keep, in m/s, from 0 to @p speed_limit.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  CruiseControl(double speed_limit, double target_speed);

  /// The road's speed limit, in m/s.
  [[nodiscard]] double get_speed_limit() const noexcept { return speed_limit_; }

  /// The speed the car tries to keep, in m/s.
  [[nodiscard]] double get_target_speed() const noexcept { return target_speed_; }

  /**
   * @brief Sets the speed to keep.
   * @param target_speed In m/s, from 0 to the speed limit.
   * @throw std::invalid_argument if @p target_speed is outside that range.
   */
  void set_target_speed(double target_speed);

 private:
  double speed_limit_;          ///< m/s, greater than 0
  double target_speed_{0.0};    ///< m/s, from 0 to speed_limit_
};
