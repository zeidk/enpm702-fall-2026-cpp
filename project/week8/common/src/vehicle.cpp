/**
 * @file vehicle.cpp
 * @brief The methods of Vehicle that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <iostream>
#include <stdexcept>

Vehicle::Vehicle(double max_speed, const std::string& color)
    : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
  if (max_speed_ <= 0.0) {
    throw std::invalid_argument{"maximum speed must be greater than 0"};
  }
}

Vehicle::Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
  set_speed(speed);
}

void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

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

void Vehicle::drive(const Route& route) const {
  std::cout << route.length() << " m";
  if (speed_ > 0.0) {
    std::cout << " at " << speed_ << " m/s: " << route.length() / speed_ << " s\n";
  } else {
    std::cout << ", but the speed is 0\n";
  }
}
