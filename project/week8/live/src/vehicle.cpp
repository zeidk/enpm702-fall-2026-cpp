/**
 * @file vehicle.cpp
 * @brief L8, live coding: the methods of Vehicle. Its constructors are defined in
 *        the class, in vehicle.hpp.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <stdexcept>  // std::invalid_argument

void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

// const and noexcept are written here too; [[nodiscard]] is not.
double Vehicle::get_speed() const noexcept { return speed_; }

const std::string& Vehicle::get_color() const noexcept { return color_; }

int Vehicle::add_sensor(SensorType type, double range) {
  const int id{static_cast<int>(sensors_.size()) + 1};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

std::size_t Vehicle::sensor_count() const noexcept { return sensors_.size(); }

const std::vector<std::unique_ptr<Sensor>>& Vehicle::get_sensors() const noexcept {
  return sensors_;
}
