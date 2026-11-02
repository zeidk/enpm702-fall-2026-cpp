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
/// A taxi that drives itself.
class RoboTaxi : public Vehicle {
 public:
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
  /// The area it may drive in.
  [[nodiscard]] const std::string& get_operating_zone() const noexcept;
  /// Its SAE J3016 level of driving automation.
  [[nodiscard]] AutomationLevel get_automation_level() const noexcept;
  /// A new area to drive in, in place of the old one.
  void change_zone(std::string operating_zone);

 private:
  /// Throws std::invalid_argument for a level that is not level_0 to level_5.
  static void check_level(AutomationLevel automation_level);

  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

}  // namespace fleet
