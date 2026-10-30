/**
 * @file vehicle.hpp
 * @brief Vehicle, the abstract base class of RoboTaxi and Taxi (L9).
 * @author Zeid Kootbally
 *
 * @details L8's Vehicle (project/week8/common), with three changes made in L9:
 * - drive() is a pure virtual method, so Vehicle is an abstract class: no
 *   Vehicle object can be created, only RoboTaxi and Taxi objects. Each says
 *   how it drives.
 * - The destructor is virtual, so deleting a RoboTaxi or a Taxi through a
 *   Vehicle pointer runs its own destructor first (Core Guidelines C.35).
 * - log_event() is protected: the methods of RoboTaxi and Taxi can call it,
 *   and code outside the classes cannot.
 *
 * The constructors are defined in the class; every other method is declared
 * here and defined in vehicle.cpp.
 *
 * Everything else is L8's: the invariant (the speed stays from 0 to the
 * maximum speed, the maximum speed is greater than 0, and the mileage never
 * goes down), the color, the id from a static counter, and the sensors it owns
 * through std::unique_ptr (composition).
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
#include <stdexcept>
#include <string>
#include <vector>

#include "route.hpp"
#include "sensor.hpp"

namespace fleet {

/// What every vehicle of the fleet holds and does.
class Vehicle {
 public:
  /**
   * @brief Starts with a speed of 0, a mileage of 0 and no sensor.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param color Its color. Any color name is a color, so there is nothing to check.
   * @throw std::invalid_argument if @p max_speed is not greater than 0.
   */
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }

  /**
   * @brief Starts with the given speed. The color is "white".
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param speed The speed to drive at, in m/s, from 0 to @p max_speed.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }

  /// Virtual: deleting through a Vehicle pointer runs the derived destructor.
  virtual ~Vehicle() = default;

  /// Drives the route. Each derived class says how: pure virtual ([Slide 90]).
  virtual void drive(const Route& route) const = 0;

  /// Its id, from the counter of the class.
  [[nodiscard]] int get_id() const noexcept;

  /// The color, given to the constructor.
  [[nodiscard]] const std::string& get_color() const noexcept;

  /// The maximum speed, in m/s.
  [[nodiscard]] double get_max_speed() const noexcept;

  /// The speed the vehicle drives at, in m/s.
  [[nodiscard]] double get_speed() const noexcept;

  /**
   * @brief Sets the speed to drive at.
   * @param speed In m/s, from 0 to the maximum speed.
   * @throw std::invalid_argument if @p speed is outside that range.
   */
  void set_speed(double speed);

  /// The distance driven so far, in m.
  [[nodiscard]] double get_mileage() const noexcept;

  /**
   * @brief Adds the distance of a trip to the mileage.
   * @param meters The trip's distance, in m. At least 0: the mileage never goes down.
   * @throw std::invalid_argument if @p meters is less than 0.
   */
  void add_mileage(double meters);

  /**
   * @brief Mounts a new sensor. The Vehicle owns it.
   * @return The new Sensor's id: 1 for the first sensor, 2 for the second, ...
   * @throw std::invalid_argument if @p range is less than 0.
   */
  int add_sensor(SensorType type, double range);

  /// How many sensors the vehicle carries.
  [[nodiscard]] std::size_t sensor_count() const noexcept;

  /// The sensors, by const reference: no copy, and the caller cannot change them.
  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>& get_sensors() const noexcept;

  /// The id the next Vehicle gets.
  [[nodiscard]] static int next_id() noexcept;

 protected:
  /// Print one line about this vehicle: "vehicle <id>: <event>". For the
  /// methods of Vehicle and of its derived classes, RoboTaxi and Taxi.
  void log_event(const std::string& event) const;

 private:
  inline static int next_id_{1};  ///< one counter for the whole class
  int id_;                        ///< from next_id_
  double max_speed_;              ///< m/s, greater than 0
  double speed_{0.0};             ///< m/s, from 0 to max_speed_
  std::string color_;             ///< from the constructor
  double mileage_{0.0};           ///< m driven, never goes down
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< composition: the only owner of each
};

}  // namespace fleet
