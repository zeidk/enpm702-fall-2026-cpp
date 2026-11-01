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

#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

// Step 3: the class RoboTaxi, derived from Vehicle, with two private data
//         members: operating_zone_, a std::string (the area it may drive in),
//         and automation_level_, an int (its SAE J3016 level of driving
//         automation). An explicit constructor, written here in the class,
//         takes the maximum speed, the zone (default "College Park") and the
//         level (default 4), and passes the maximum speed to Vehicle. Two
//         const getters, get_operating_zone() and get_automation_level(),
//         are declared here and defined in robotaxi.cpp.
// Step 4: the declaration drive(const Route&) const override.
// Step 7: the class becomes final.

}  // namespace fleet
