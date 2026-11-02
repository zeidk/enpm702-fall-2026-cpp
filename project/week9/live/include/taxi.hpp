/**
 * @file taxi.hpp
 * @brief L9, live coding: the class Taxi (Steps 3, 4 and 6). Step 3 is given: we review it in class.
 * @author Zeid Kootbally
 *
 * @details A Taxi is a Vehicle with a driver (inheritance). It has its own
 * file and includes vehicle.hpp; vehicle.hpp does not include it.
 */
#pragma once

#include <string>
#include <utility>  // Step 3: std::move

#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

// Step 3, given: a Taxi is a Vehicle with a driver.
// Step 6: the class becomes final.
/// A taxi that a person drives.
class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  // Step 4: declare Taxi's own drive() here, with the same name, parameter and
  //         const as Vehicle's, then override:
  //         void drive(const Route& route) const override;
  //         Its body goes to taxi.cpp.
  /// A new driver, in place of the old one; an empty name for no driver.
  void assign_driver(std::string driver_name);

 private:
  std::string driver_name_;
};

}  // namespace fleet
