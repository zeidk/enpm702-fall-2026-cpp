/**
 * @file sensor.hpp
 * @brief L9, ride service: the class Sensor, given.
 * @author Zeid Kootbally
 *
 * @details A robotaxi carries several sensors: a lidar on the roof, cameras,
 * a radar. Sensor is a class, not a struct: its type must be lidar, camera or
 * radar, and its range must be at least 0. Those rules are its invariant, so
 * the constructor checks them and the data members are private (Core
 * Guidelines C.2). Vehicle::add_sensor() picks each id. SensorType is in
 * sensor_type.hpp.
 *
 * With g++ 13.3 on x86-64: sizeof(SensorType) is 4 and sizeof(Sensor) is 16
 * (an int, a SensorType and a double, no padding).
 */
#pragma once

#include <stdexcept>

#include "sensor_type.hpp"

namespace fleet {

/// One sensor on a robotaxi: a lidar, a camera or a radar; its range is at least 0.
class Sensor {
 public:
  /**
   * @brief A sensor; by default a lidar that sees 100 m (chosen for the examples).
   * @param id Its id, picked by Vehicle::add_sensor().
   * @param type What kind of sensor: lidar, camera or radar.
   * @param range How far it sees, in m. Must be at least 0.
   * @throw std::invalid_argument if @p type is not lidar, camera or radar (a
   *        number cast to SensorType, such as static_cast<SensorType>(7)).
   * @throw std::invalid_argument if @p range is less than 0.
   */
  explicit Sensor(int id, SensorType type = SensorType::lidar, double range = 100.0)
      : id_{id}, type_{type}, range_{range} {
    if (type_ != SensorType::lidar && type_ != SensorType::camera &&
        type_ != SensorType::radar) {
      throw std::invalid_argument{"unknown sensor type"};
    }
    if (range_ < 0.0) {
      throw std::invalid_argument{"range must be at least 0"};
    }
  }

  /**
   * @brief Returns its id.
   * @return The id that Vehicle::add_sensor() picked.
   */
  [[nodiscard]] int get_id() const noexcept;

  /**
   * @brief Returns what kind of sensor it is.
   * @return lidar, camera or radar.
   */
  [[nodiscard]] SensorType get_type() const noexcept;

  /**
   * @brief Returns how far it sees.
   * @return The range, in m, at least 0.
   */
  [[nodiscard]] double get_range() const noexcept;

 private:
  int id_;           ///< picked by Vehicle::add_sensor()
  SensorType type_;  ///< what kind of sensor
  double range_;     ///< m, at least 0
};

}  // namespace fleet
