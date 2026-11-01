/**
 * @file robotaxi.cpp
 * @brief The methods of RoboTaxi. Its constructor is defined in the class, in
 *        robotaxi.hpp, and calls check_level() before Vehicle takes an id.
 * @author Zeid Kootbally
 *
 * @details RoboTaxi::drive() is the code of [Slide 87], Concrete Classes. It
 * calls Vehicle's public sensor_count(): sensors_ is Vehicle's private member.
 */
#include "robotaxi.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace fleet {

void RoboTaxi::drive(const Route& route) const {
  if (sensor_count() == 0) {  // FR-4: no sensor, no driving
    std::cout << "RoboTaxi: no sensor, stays parked\n";
    return;
  }
  std::cout << "RoboTaxi: " << sensor_count()
            << (sensor_count() == 1 ? " sensor" : " sensors") << " checked, drives "
            << route.length() << " m\n";
}

const std::string& RoboTaxi::get_operating_zone() const noexcept { return operating_zone_; }

AutomationLevel RoboTaxi::get_automation_level() const noexcept {
  return automation_level_;
}

void RoboTaxi::change_zone(std::string operating_zone) {
  operating_zone_ = std::move(operating_zone);
}

void RoboTaxi::check_level(AutomationLevel automation_level) {
  if (automation_level < AutomationLevel::level_0 ||
      automation_level > AutomationLevel::level_5) {
    throw std::invalid_argument{"unknown automation level"};
  }
}

}  // namespace fleet
