/**
 * @file robotaxi.hpp
 * @brief RoboTaxi: a Vehicle that drives itself (L9).
 * @author Zeid Kootbally
 *
 * @details A RoboTaxi is a Vehicle (inheritance). Its own drive() checks the
 * sensors first: a robotaxi with no sensor does not move (the requirement FR-4
 * asks for one or more). It adds two data members: the area it may drive in,
 * and its level of driving automation (SAE J3016). The sensors are Vehicle's.
 *
 * RoboTaxi is final: a derived class could override drive() and skip the
 * sensor check.
 */
#pragma once

#include <string>
#include <utility>

#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

/// A taxi that drives itself with its sensors.
class RoboTaxi final : public Vehicle {
 public:
  /**
   * @brief Starts parked, with no sensor.
   * @param max_speed The maximum speed, in m/s, passed to Vehicle.
   * @param operating_zone The area it may drive in.
   * @param automation_level Its SAE J3016 level of driving automation.
   */
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    int automation_level = 4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}

  /// Drives the route, or stays parked with no sensor.
  void drive(const Route& route) const override;

  /// The area it may drive in, given to the constructor.
  [[nodiscard]] const std::string& get_operating_zone() const noexcept;

  /// Its SAE J3016 level of driving automation, given to the constructor.
  [[nodiscard]] int get_automation_level() const noexcept;

 private:
  std::string operating_zone_;  ///< the area it may drive in
  int automation_level_;        ///< SAE J3016 level of driving automation
};

}  // namespace fleet
