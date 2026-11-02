/**
 * @file robotaxi.hpp
 * @brief L9, live coding: the class RoboTaxi (Steps 3, 4 and 7), which you write in class.
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

// Step 3: the class RoboTaxi, derived from Vehicle, with two private data
//         members: operating_zone_, a std::string (the area it may drive in),
//         and automation_level_, an AutomationLevel (its SAE J3016 level of
//         driving automation, from automation_level.hpp). An explicit
//         constructor, written here in the class, takes the maximum speed,
//         the zone (default "College Park") and the level (default
//         AutomationLevel::level_4), and passes the maximum speed to Vehicle.
//         Declare the private static method check_level(AutomationLevel) of the
//         class diagram: its definition is given in robotaxi.cpp, commented out,
//         and you uncomment it. It throws std::invalid_argument for a level that
//         is not level_0 to level_5. The constructor calls it before Vehicle's
//         constructor, so a rejected RoboTaxi takes no id:
//         Vehicle{(check_level(automation_level), max_speed)}.
//         Two
//         const getters, get_operating_zone() and get_automation_level(),
//         and the method change_zone(std::string operating_zone), which
//         Vehicle does not have, are declared here and defined in robotaxi.cpp.
// Step 4: in the public part of RoboTaxi, declare its own drive(), with the
//         same name, parameter and const as Vehicle's, then override:
//         void drive(const Route& route) const override;
//         Its body goes to robotaxi.cpp.
// Step 7: the class becomes final.

}  // namespace fleet
