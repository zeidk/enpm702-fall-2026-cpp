/**
 * @file taxi.cpp
 * @brief The methods of Taxi. Its constructor is defined in the class, in
 *        taxi.hpp.
 * @author Zeid Kootbally
 */
#include "taxi.hpp"

#include <iostream>

namespace fleet {

void Taxi::drive(const Route& route) const {
  if (driver_name_.empty()) {
    std::cout << "Taxi: no driver, stays parked\n";
    return;
  }
  std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
}

}  // namespace fleet
