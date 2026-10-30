/**
 * @file taxi.hpp
 * @brief Taxi: a Vehicle that a person drives (L9).
 * @author Zeid Kootbally
 *
 * @details A Taxi is a Vehicle (inheritance) with the name of its driver. Taxi
 * adds no rule of its own, so its invariant is Vehicle's. An empty name means
 * the taxi has no driver, and then it does not move.
 *
 * Taxi is final: no class derives from it.
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

 private:
  std::string driver_name_;  ///< empty: no driver
};

}  // namespace fleet
