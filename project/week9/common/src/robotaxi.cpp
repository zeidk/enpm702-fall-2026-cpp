/**
 * @file robotaxi.cpp
 * @brief The methods of RoboTaxi that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "robotaxi.hpp"

#include <iostream>

void RoboTaxi::drive(const Route& route) const {
  if (sensor_count() == 0) {  // Vehicle's public method: sensors_ is private
    std::cout << "RoboTaxi: no sensor, stays parked\n";
    return;
  }
  std::cout << "RoboTaxi: " << sensor_count()
            << (sensor_count() == 1 ? " sensor" : " sensors") << " checked, drives "
            << route.length() << " m\n";
}
