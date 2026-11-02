/**
 * @file robotaxi.cpp
 * @brief L9, live coding: the methods of RoboTaxi (Steps 3 and 4).
 * @author Zeid Kootbally
 */
#include "robotaxi.hpp"

#include <iostream>  // Step 4: drive() prints
#include <stdexcept>  // Step 3: std::invalid_argument
#include <utility>   // Step 3: std::move

namespace fleet {

// Step 3: the getters get_operating_zone() and get_automation_level() return
//         operating_zone_ and automation_level_. change_zone() moves its
//         parameter into operating_zone_ (std::move).

// Step 3: given. Uncomment it once robotaxi.hpp declares check_level.
// void RoboTaxi::check_level(AutomationLevel automation_level) {
//   if (automation_level < AutomationLevel::level_0 ||
//       automation_level > AutomationLevel::level_5) {
//     throw std::invalid_argument{"unknown automation level"};
//   }
// }

// Step 4: define RoboTaxi::drive(const Route& route) const. Its body prints one
//         line to std::cout: "RoboTaxi: drives itself ", then route.length(),
//         then " m" and a new line. For the 500 m route it prints
//         "RoboTaxi: drives itself 500 m". The definition does not repeat
//         override: only the declaration in robotaxi.hpp has it.

}  // namespace fleet
