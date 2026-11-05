/**
 * @file taxi.hpp
 * @brief L9, ride service: the class Taxi (Steps 3, 4 and 6). Step 3 is given: we review it in class.
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
/**
 * @brief A taxi that a person drives.
 *
 * A Taxi is a Vehicle (inheritance), with the name of its driver.
 */
class Taxi : public Vehicle {
 public:
  /**
   * @brief A taxi and its driver.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param driver_name The driver's name; an empty name for no driver.
   * @throw std::invalid_argument if @p max_speed is not greater than 0 (from
   *        Vehicle's constructor).
   */
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  /**
   * @brief Step 4: drives @p route and prints one line: "Taxi: <driver> drives
   *        <length> m", or "Taxi: no driver, stays parked" when there is no
   *        driver.
   * @param route The route to drive.
   */
  void drive(const Route& route) const;
  /**
   * @brief Gives the taxi a new driver, in place of the old one.
   * @param driver_name The new driver's name; an empty name for no driver.
   */
  void assign_driver(std::string driver_name);

 private:
  std::string driver_name_;  ///< empty when the taxi has no driver
};

}  // namespace fleet
