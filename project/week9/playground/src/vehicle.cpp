/**
 * @file vehicle.cpp
 * @brief The methods of Vehicle. Its constructors are defined in the class, in
 *        vehicle.hpp.
 * @author Zeid Kootbally
 *
 * @details L8's code, without drive(): in L9 each derived class writes it.
 */
#include "vehicle.hpp"

#include <iostream>
#include <stdexcept>

namespace fleet {

int Vehicle::get_id() const noexcept { return id_; }

const std::string& Vehicle::get_color() const noexcept { return color_; }

double Vehicle::get_max_speed() const noexcept { return max_speed_; }

double Vehicle::get_speed() const noexcept { return speed_; }

void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

double Vehicle::get_mileage() const noexcept { return mileage_; }

void Vehicle::add_mileage(double meters) {
  if (meters < 0.0) {
    throw std::invalid_argument{"a trip's distance must be at least 0"};
  }
  mileage_ += meters;
}

int Vehicle::add_sensor(SensorType type, double range) {
  const int id{static_cast<int>(sensors_.size()) + 1};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

std::size_t Vehicle::sensor_count() const noexcept { return sensors_.size(); }

const std::vector<std::unique_ptr<Sensor>>& Vehicle::get_sensors() const noexcept {
  return sensors_;
}

int Vehicle::next_id() noexcept { return next_id_; }

void Vehicle::log_event(const std::string& event) const {
  std::cout << "vehicle " << id_ << ": " << event << '\n';
}

}  // namespace fleet
