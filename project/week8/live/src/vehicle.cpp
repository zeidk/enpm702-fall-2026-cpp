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

int Vehicle::get_id() const noexcept { return id_; }

// The definition does not repeat static.
int Vehicle::get_next_id() noexcept { return next_id_; }
