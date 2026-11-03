/**
 * @file robotaxi.hpp
 * @brief L9, live coding: the class RoboTaxi (Steps 3, 4 and 6). Step 3 is given: we review it in class.
 * @author Zeid Kootbally
 *
 * @details A RoboTaxi is a Vehicle (inheritance). It has its own file and
 * includes vehicle.hpp; vehicle.hpp does not include it.
 */
#pragma once

#include <string>
#include <utility>  // Step 3: std::move

#include "automation_level.hpp"  // Step 3: AutomationLevel
#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

// Step 3, given: a RoboTaxi is a Vehicle (inheritance), with a zone and a level.
// Step 6: the class becomes final.
/**
 * @brief A taxi that drives itself.
 *
 * A RoboTaxi is a Vehicle (inheritance), with the area it may drive in and its
 * SAE J3016 level of driving automation.
 */
class RoboTaxi : public Vehicle {
 public:
  /**
   * @brief A robotaxi; by default it drives in College Park at Level 4.
   * @param max_speed The maximum speed, in m/s. Must be greater than 0.
   * @param operating_zone The area it may drive in.
   * @param automation_level Its SAE J3016 level: level_0 to level_5.
   * @throw std::invalid_argument if @p automation_level is not level_0 to
   *        level_5 (a number cast to AutomationLevel).
   * @throw std::invalid_argument if @p max_speed is not greater than 0 (from
   *        Vehicle's constructor).
   */
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      // check_level runs before Vehicle's constructor, so a rejected RoboTaxi takes no id
      : Vehicle{(check_level(automation_level), max_speed)},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  // Step 4: declare RoboTaxi's own drive() here, with the same name, parameter
  //         and const as Vehicle's, then override:
  //         void drive(const Route& route) const override;
  //         Its body goes to robotaxi.cpp.
  /**
   * @brief Returns the area it may drive in.
   * @return The operating zone.
   */
  [[nodiscard]] const std::string& get_operating_zone() const noexcept;
  /**
   * @brief Returns its SAE J3016 level of driving automation.
   * @return The automation level.
   */
  [[nodiscard]] AutomationLevel get_automation_level() const noexcept;
  /**
   * @brief Gives it a new area to drive in, in place of the old one.
   * @param operating_zone The new area.
   */
  void change_zone(std::string operating_zone);

 private:
  /**
   * @brief Checks a level before the constructor uses it.
   * @param automation_level The level to check.
   * @throw std::invalid_argument if @p automation_level is not level_0 to
   *        level_5.
   */
  static void check_level(AutomationLevel automation_level);

  std::string operating_zone_;        ///< the area it may drive in
  AutomationLevel automation_level_;  ///< SAE J3016 level of driving automation
};

}  // namespace fleet
