/**
 * @file taxi.hpp
 * @brief Taxi: a Vehicle that a person drives (L9).
 * @author Zeid Kootbally
 *
 * @details A Taxi is a Vehicle (inheritance) with the name of its driver. Taxi
 * adds no rule of its own, so its invariant is Vehicle's. An empty name means
 * the taxi has no driver, and then it does not move. assign_driver() changes
 * the driver; Vehicle has no such method.
 *
 * Taxi is final: a derived class could override drive() and skip the
 * driver check.
 */
#pragma once

#include <string>
#include <utility>

#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

/// A taxi driven by a person.
class Taxi final : public Vehicle {
 public:
  /**
   * @brief Starts parked.
   * @param max_speed The maximum speed, in m/s, passed to Vehicle.
   * @param driver_name The driver's name; empty for no driver.
   */
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

  /// Drives the route, or stays parked with no driver.
  void drive(const Route& route) const override;

  /**
   * @brief Assigns a new driver, in place of the old one. It meets FR-11,
   *        Manage drivers: an empty name removes the driver, and then the taxi
   *        stays parked (FR-5.1).
   * @param driver_name The new driver's name; empty for no driver.
   */
  void assign_driver(std::string driver_name);

 private:
  std::string driver_name_;  ///< empty: no driver
};

}  // namespace fleet
