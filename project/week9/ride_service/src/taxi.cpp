/**
 * @file taxi.cpp
 * @brief L9, ride service: the methods of Taxi (Steps 3 and 4).
 * @author Zeid Kootbally
 */
#include "taxi.hpp"

#include <iostream>  // Step 4: drive() prints
#include <utility>   // Step 3: std::move

namespace fleet {

// Step 3, given.
void Taxi::assign_driver(std::string driver_name) { driver_name_ = std::move(driver_name); }

// Step 4. The definition does not repeat override: only the declaration in
// taxi.hpp has it.
void Taxi::drive(const Route& route) const {
  if (driver_name_.empty()) {
    std::cout << "Taxi: no driver, stays parked\n";
    return;
  }
  std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
}

}  // namespace fleet
