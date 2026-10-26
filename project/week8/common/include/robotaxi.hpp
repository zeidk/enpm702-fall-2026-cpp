/**
 * @file robotaxi.hpp
 * @brief A robotaxi, as a class: it owns one Sensor for each sensor it carries.
 * @author Zeid Kootbally
 *
 * @details The invariant: no two Sensor objects the robotaxi holds have the
 * same id. add_sensor() gives each new Sensor the next id, so code outside the
 * class never picks one. The ids come from a static counter, shared by every
 * RoboTaxi, so two robotaxis of the fleet never hand out the same id either.
 *
 * RoboTaxi defines none of the five special members: its only data member, a
 * std::vector of std::unique_ptr, gives back the memory itself (the rule of
 * zero). A RoboTaxi can be moved and cannot be copied, because a
 * std::unique_ptr cannot be copied.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "sensor.hpp"

/// Owns the Sensor objects of the sensors a robotaxi carries.
class RoboTaxi {
 public:
  /**
   * @brief Mounts a new sensor.
   * @param type What kind of sensor.
   * @param range How far it sees, in m.
   * @return The id the new Sensor gets.
   */
  int add_sensor(SensorType type, double range);

  /**
   * @brief Removes a sensor, and deletes its Sensor.
   * @param id The id add_sensor() returned.
   * @return False when no Sensor has that id.
   */
  bool remove_sensor(int id);

  /**
   * @brief Finds a sensor by id.
   * @return A pointer that does not own the Sensor (L7), or nullptr.
   */
  [[nodiscard]] const Sensor* find_sensor(int id) const;

  /// How many sensors the robotaxi carries.
  [[nodiscard]] std::size_t sensor_count() const noexcept { return sensors_.size(); }

  /// The id the next add_sensor() gives, in every RoboTaxi.
  [[nodiscard]] static int next_id() noexcept { return next_id_; }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;  ///< the only owner of each
  inline static int next_id_{1};  ///< one counter for the whole class
};
