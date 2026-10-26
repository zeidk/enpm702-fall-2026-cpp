/**
 * @file robotaxi.cpp
 * @brief The methods of RoboTaxi that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "robotaxi.hpp"

#include <algorithm>

int RoboTaxi::add_sensor(SensorType type, double range) {
  const int id{next_id_++};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

bool RoboTaxi::remove_sensor(int id) {
  const auto removed{std::erase_if(
      sensors_, [id](const std::unique_ptr<Sensor>& sensor) { return sensor->get_id() == id; })};
  return removed > 0;
}

const Sensor* RoboTaxi::find_sensor(int id) const {
  for (const std::unique_ptr<Sensor>& sensor : sensors_) {
    if (sensor->get_id() == id) {
      return sensor.get();
    }
  }
  return nullptr;
}
