/**
 * @file vehicle.hpp
 * @brief L9, live coding: the class Vehicle, given. Read it before Step 1.
 * @author Zeid Kootbally
 *
 * @details Given, not written in class: its speed stays from 0 to its maximum
 * speed. L9 changes it: a virtual drive() and a virtual destructor (Step 4),
 * and drive() pure virtual (Step 5), so Vehicle becomes abstract.
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

#include "route.hpp"   // drive() takes a Route
#include "sensor.hpp"  // Sensor and SensorType

namespace fleet {

/**
 * @brief A vehicle of the robotaxi fleet.
 *
 * Invariant: its maximum speed is greater than 0, and its speed stays from 0
 * to its maximum speed. It owns its sensors (composition) and takes a Route
 * only as a parameter of drive() (dependency).
 */
class Vehicle {
 public:
  /**
   * @brief A vehicle that stands still, with the next id.
   *
   * The member initializer list sets id_, max_speed_ and color_, then the body
   * checks the maximum speed.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param color The color of the body.
   * @throw std::invalid_argument if @p max_speed is not greater than 0.
   */
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }

  /**
   * @brief A vehicle that already moves: the delegating constructor.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param speed The speed, in m/s. Must be from 0 to @p max_speed.
   * @throw std::invalid_argument if either value breaks the invariant.
   */
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} { set_speed(speed); }

  // Step 4: first add the destructor here, virtual and = default:
  //         virtual ~Vehicle() = default;
  //         Deleting a RoboTaxi or a Taxi through a Vehicle pointer then runs
  //         its own destructor first, then ~Vehicle. Then uncomment the Step 4
  //         block in main.cpp.

  /**
   * @brief Drives @p route and prints one line: "Vehicle: drives <length> m".
   *
   * The Route is a parameter: Vehicle uses it during the call and keeps no
   * Route member. That is a dependency.
   * @param route The route to drive.
   */
  // Step 4: add virtual in front of this declaration, so a call through a
  //         Vehicle& or a Vehicle* runs the drive() of the object's own class:
  //         virtual void drive(const Route& route) const;
  //         Its body in vehicle.cpp does not change: the definition does not
  //         repeat virtual, only this declaration has it.
  // Step 5: drive() becomes pure virtual (= 0).
  void drive(const Route& route) const;

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
   * @return The color the constructor set.
   */
  [[nodiscard]] const std::string& get_color() const noexcept;
  /**
   * @brief Returns the maximum speed.
   * @return The maximum speed, in m/s.
   */
  [[nodiscard]] double get_max_speed() const noexcept;

  /**
   * @brief Returns the distance driven so far.
   * @return The mileage, in m.
   */
  [[nodiscard]] double get_mileage() const noexcept;
  /**
   * @brief Adds the distance of a trip to the mileage.
   * @param meters The distance of the trip, in m.
   * @throw std::invalid_argument if @p meters is less than 0: the mileage never
   *        goes down.
   */
  void add_mileage(double meters);

  /**
   * @brief Adds a sensor that the vehicle owns.
   * @param type What kind of sensor: lidar, camera or radar.
   * @param range How far it sees, in m. Must be at least 0.
   * @return The new Sensor's id: 1, 2, ...
   * @throw std::invalid_argument if @p type or @p range breaks Sensor's
   *        invariant (from Sensor's constructor).
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

  /**
   * @brief Returns the vehicle's id.
   * @return The id, unique among the vehicles made so far.
   */
  [[nodiscard]] int get_id() const noexcept;
  /**
   * @brief Returns the id the next Vehicle gets.
   * @return The next id.
   */
  [[nodiscard]] static int get_next_id() noexcept;

 protected:
  /**
   * @brief Prints one line about this vehicle: "vehicle <id>: <event>".
   *
   * For the methods of Vehicle and of the classes derived from it.
   * @param event What happened.
   */
  void log_event(const std::string& event) const;

 private:
  inline static int next_id_{1};  ///< one counter for the whole class
  int id_;                        ///< from next_id_
  double max_speed_;              ///< m/s, greater than 0
  double speed_{0.0};             ///< m/s, from 0 to max_speed_
  std::string color_;             ///< set by the constructor
  double mileage_{0.0};           ///< m driven
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< the only owner of each Sensor
};

}  // namespace fleet
