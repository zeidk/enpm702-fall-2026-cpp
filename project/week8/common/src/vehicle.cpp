/**
 * @file vehicle.cpp
 * @brief The methods of Vehicle that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "vehicle.hpp"

#include <stdexcept>

Vehicle::Vehicle(double max_speed) : max_speed_{max_speed} {
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
