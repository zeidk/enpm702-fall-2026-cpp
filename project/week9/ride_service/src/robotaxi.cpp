/**
 * @file robotaxi.cpp
 * @brief L9, ride service: the methods of RoboTaxi (Steps 3 and 4).
 * @author Zeid Kootbally
 */
#include "robotaxi.hpp"

#include <iostream>  // Step 4: drive() prints
#include <stdexcept>  // Step 3: std::invalid_argument
#include <utility>   // Step 3: std::move

namespace fleet {

// Step 3, given.
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

// Step 4. The definition does not repeat override: only the declaration in
// robotaxi.hpp has it.
void RoboTaxi::drive(const Route& route) const {
  std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
}

}  // namespace fleet
