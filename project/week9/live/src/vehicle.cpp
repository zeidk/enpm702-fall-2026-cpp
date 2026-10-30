/**
 * @file vehicle.cpp
 * @brief L9, live coding: the methods of Vehicle.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace fleet {

// L8.
void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

// L8.
double Vehicle::get_speed() const noexcept { return speed_; }

// L8.
const std::string& Vehicle::get_color() const noexcept { return color_; }

// L8.
int Vehicle::add_sensor(SensorType type, double range) {
  const int id{static_cast<int>(sensors_.size()) + 1};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

// L8.
std::size_t Vehicle::sensor_count() const noexcept { return sensors_.size(); }

// L8.
const std::vector<std::unique_ptr<Sensor>>& Vehicle::get_sensors() const noexcept {
  return sensors_;
}

// L8.
int Vehicle::get_id() const noexcept { return id_; }

// L8. The definition does not repeat static.
int Vehicle::next_id() noexcept { return next_id_; }

// Step 4: Vehicle::drive() prints "Vehicle: drives ", the route's length and " m".
// Step 6: Vehicle::drive() is deleted: a pure virtual method has no body.

}  // namespace fleet
