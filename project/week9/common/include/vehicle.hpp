/**
 * @file vehicle.hpp
 * @brief Vehicle, the abstract base class of RoboTaxi and Taxi (L9).
 * @author Zeid Kootbally
 *
 * @details L8's Vehicle (project/week8/common), with two changes made in L9:
 * - drive() is a pure virtual method, so Vehicle is an abstract class: no
 *   Vehicle object can be created, only RoboTaxi and Taxi objects. Each says
 *   how it drives.
 * - The destructor is virtual, so deleting a RoboTaxi or a Taxi through a
 *   Vehicle pointer runs its own destructor first (Core Guidelines C.35).
 *
 * Everything else is L8's: the invariant (the speed stays from 0 to the
 * maximum speed, and the maximum speed is greater than 0), the id from a static
 * counter, and the sensors it owns through std::unique_ptr (composition).
 *
 * A Vehicle cannot be copied (std::unique_ptr cannot be copied, L8) and cannot
 * be moved (a declared destructor stops the compiler from writing the moves).
 * Core Guidelines C.67 asks exactly that of a polymorphic class: no public copy
 * or move, so no slicing.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "route.hpp"
#include "sensor.hpp"

/// What every vehicle of the fleet holds and does.
class Vehicle {
 public:
  /**
   * @brief Starts with a speed of 0 and no sensor.
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

  /// Virtual: deleting through a Vehicle pointer runs the derived destructor.
  virtual ~Vehicle() = default;

  /// Drives the route. Each derived class says how: pure virtual.
  virtual void drive(const Route& route) const = 0;

  /// Its id, from the counter of the class.
  [[nodiscard]] int get_id() const noexcept { return id_; }

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

  /**
   * @brief Mounts a new sensor. The Vehicle owns it.
   * @return The new Sensor's id: 1 for the first sensor, 2 for the second, ...
   * @throw std::invalid_argument if @p range is less than 0.
   */
  int add_sensor(SensorType type, double range);

  /// How many sensors the vehicle carries.
  [[nodiscard]] std::size_t sensor_count() const noexcept { return sensors_.size(); }

  /// The id the next Vehicle gets.
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  inline static int next_id_{1};  ///< one counter for the whole class
  int id_;                        ///< from next_id_
  double max_speed_;              ///< m/s, greater than 0
  double speed_{0.0};             ///< m/s, from 0 to max_speed_
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< composition: the only owner of each
};
