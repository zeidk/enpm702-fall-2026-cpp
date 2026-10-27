/**
 * @file vehicle.cpp
 * @brief The methods of Vehicle. Its constructors are defined in the class, in
 *        vehicle.hpp.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <iostream>
#include <stdexcept>

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

void Vehicle::drive(const Route& route) const {
  std::cout << route.length() << " m";
  if (speed_ > 0.0) {
    std::cout << " at " << speed_ << " m/s: " << route.length() / speed_ << " s\n";
  } else {
    std::cout << ", but the speed is 0\n";
  }
}

int Vehicle::next_id() noexcept { return next_id_; }
