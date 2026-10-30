/**
 * @file robotaxi.cpp
 * @brief The methods of RoboTaxi. Its constructor is defined in the class, in
 *        robotaxi.hpp.
 * @author Zeid Kootbally
 *
 * @details RoboTaxi::drive() is the code of [Slide 90], Concrete Classes. It
 * calls Vehicle's public sensor_count(): sensors_ is Vehicle's private member.
 */
#include "robotaxi.hpp"

#include <iostream>

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

}  // namespace fleet
