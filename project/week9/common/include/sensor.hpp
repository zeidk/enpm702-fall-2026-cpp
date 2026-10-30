/**
 * @file sensor.hpp
 * @brief The robotaxi's sensors, shared by every file of week 9.
 * @author Zeid Kootbally
 *
 * @details A robotaxi carries several sensors: a lidar on the roof, cameras,
 * a radar. Sensor is a class, not a struct: its range must be at least 0. That
 * rule is its invariant, so the constructor checks it and the data members are
 * private (Core Guidelines C.2). Vehicle::add_sensor() picks each id.
 *
 * With g++ 13.3 on x86-64: sizeof(SensorType) is 4 and sizeof(Sensor) is 16
 * (an int, a SensorType and a double, no padding).
 */
#pragma once

#include <stdexcept>

namespace fleet {

/// What kind of sensor it is: a scoped enumeration (Lecture 2).
enum class SensorType {
  lidar,   ///< light detection and ranging
  camera,  ///< a video camera
  radar,   ///< radio detection and ranging
};

/// One sensor on a robotaxi: its range is at least 0.
class Sensor {
 public:
  /**
   * @brief A sensor; by default a lidar that sees 100 m (chosen for the examples).
   * @param id Its id, picked by Vehicle::add_sensor().
   * @param type What kind of sensor.
   * @param range How far it sees, in m. Must be at least 0.
   * @throw std::invalid_argument if @p range is less than 0.
   */
  explicit Sensor(int id, SensorType type = SensorType::lidar, double range = 100.0)
      : id_{id}, type_{type}, range_{range} {
    if (range_ < 0.0) {
      throw std::invalid_argument{"range must be at least 0"};
    }
  }

  /// Its id.
  [[nodiscard]] int get_id() const noexcept;

  /// What kind of sensor it is.
  [[nodiscard]] SensorType get_type() const noexcept;

  /// How far it sees, in m.
  [[nodiscard]] double get_range() const noexcept;

 private:
  int id_;           ///< picked by Vehicle::add_sensor()
  SensorType type_;  ///< what kind of sensor
  double range_;     ///< m, at least 0
};

}  // namespace fleet
