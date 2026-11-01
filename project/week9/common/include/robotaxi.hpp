/**
 * @file robotaxi.hpp
 * @brief RoboTaxi: a Vehicle that drives itself (L9).
 * @author Zeid Kootbally
 *
 * @details A RoboTaxi is a Vehicle (inheritance). Its own drive() checks the
 * sensors first: a robotaxi with no sensor does not move (the requirement FR-4
 * asks for one or more). It adds two data members: the area it may drive in,
 * and its level of driving automation (SAE J3016), an AutomationLevel. The
 * constructor rejects a level that is not level_0 to level_5. The sensors are
 * Vehicle's.
 * change_zone() moves it to another area; Vehicle has no such method.
 *
 * RoboTaxi is final: a derived class could override drive() and skip the
 * sensor check.
 */
#pragma once

#include <stdexcept>
#include <string>
#include <utility>

#include "automation_level.hpp"
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
   * @param automation_level Its SAE J3016 level of driving automation, Level 0 to 5.
   * @throw std::invalid_argument if @p automation_level is not level_0 to level_5
   *        (a number cast to AutomationLevel, such as static_cast<AutomationLevel>(7)).
   */
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {
    if (automation_level_ < AutomationLevel::level_0 ||
        automation_level_ > AutomationLevel::level_5) {
      throw std::invalid_argument{"unknown automation level"};
    }
  }

  /// Drives the route, or stays parked with no sensor.
  void drive(const Route& route) const override;

  /// The area it may drive in: from the constructor, or from the last change_zone().
  [[nodiscard]] const std::string& get_operating_zone() const noexcept;

  /// Its SAE J3016 level of driving automation, given to the constructor.
  [[nodiscard]] AutomationLevel get_automation_level() const noexcept;

  /**
   * @brief Sets a new area to drive in, in place of the old one. It meets FR-4.4,
   *        Change the operating zone.
   * @param operating_zone The new area it may drive in.
   */
  void change_zone(std::string operating_zone);

 private:
  std::string operating_zone_;        ///< the area it may drive in
  AutomationLevel automation_level_;  ///< SAE J3016 level of driving automation
};

}  // namespace fleet
