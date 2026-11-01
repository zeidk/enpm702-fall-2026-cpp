/**
 * @file automation_level.hpp
 * @brief L9, live coding: the enumeration AutomationLevel, given.
 * @author Zeid Kootbally
 *
 * @details SAE J3016 defines six levels of driving automation, Level 0 to
 * Level 5 (J3016_202609, September 2026:
 * https://saemobilus.sae.org/standards/j3016_202609-taxonomy-definitions-terms-related-driving-automation-systems-road-motor-vehicles).
 * The requirement FR-4.3 allows these six and no other, so the level is a
 * scoped enumeration, not an int: a number such as 7 cannot be passed where
 * an AutomationLevel is expected. A cast still makes one:
 * static_cast<AutomationLevel>(7) compiles and gives a level that names no
 * enumerator, so RoboTaxi's constructor checks the level too.
 *
 * With g++ 13.3 on x86-64: sizeof(AutomationLevel) is 4 (its underlying type
 * is int).
 */
#pragma once

namespace fleet {

/// The SAE J3016 level of driving automation: a scoped enumeration.
enum class AutomationLevel {
  level_0,  ///< no driving automation
  level_1,  ///< driver support for steering or speed
  level_2,  ///< driver support for steering and speed
  level_3,  ///< automated driving under defined conditions, a human drives after an alert
  level_4,  ///< automated driving under defined conditions, no human driving needed
  level_5,  ///< automated driving under all conditions in which humans can drive
};

}  // namespace fleet
