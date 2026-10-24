/**
 * @file cruise_control.cpp
 * @brief The methods of CruiseControl that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "cruise_control.hpp"

#include <stdexcept>

CruiseControl::CruiseControl(double speed_limit) : speed_limit_{speed_limit} {
  if (speed_limit_ <= 0.0) {
    throw std::invalid_argument{"speed limit must be greater than 0"};
  }
}

CruiseControl::CruiseControl(double speed_limit, double target_speed)
    : CruiseControl{speed_limit} {
  set_target_speed(target_speed);
}

void CruiseControl::set_target_speed(double target_speed) {
  if (target_speed < 0.0 || target_speed > speed_limit_) {
    throw std::invalid_argument{"target speed must be from 0 to the speed limit"};
  }
  target_speed_ = target_speed;
}
