/**
 * @file vehicle.cpp
 * @brief L9, ride service: the methods of Vehicle.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <iostream>  // drive() and log_event() print
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace fleet {

void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

double Vehicle::get_speed() const noexcept { return speed_; }

const std::string& Vehicle::get_color() const noexcept { return color_; }

double Vehicle::get_max_speed() const noexcept { return max_speed_; }

double Vehicle::get_mileage() const noexcept { return mileage_; }

void Vehicle::add_mileage(double meters) {
  if (meters < 0.0) {
    throw std::invalid_argument{"a trip's distance must be at least 0"};
  }
  mileage_ += meters;
}

int Vehicle::add_sensor(SensorType type, double range) {
  sensors_.push_back(
      std::make_unique<Sensor>(sensors_.size() + 1, type, range));
  return sensors_.back()->get_id();
}

std::size_t Vehicle::sensor_count() const noexcept { return sensors_.size(); }

const std::vector<std::unique_ptr<Sensor>>& Vehicle::get_sensors() const noexcept {
  return sensors_;
}

int Vehicle::get_id() const noexcept { return id_; }

// The definition does not repeat static.
int Vehicle::get_next_id() noexcept { return next_id_; }

void Vehicle::log_event(const std::string& event) const {
  std::cout << "vehicle " << id_ << ": " << event << '\n';
}

// Step 5: delete this body. drive() is pure virtual now, and Vehicle gives it
//         no body: RoboTaxi and Taxi each define their own.
void Vehicle::drive(const Route& route) const {
  std::cout << "Vehicle: drives " << route.length() << " m\n";
}

}  // namespace fleet
