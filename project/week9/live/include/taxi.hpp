/**
 * @file taxi.hpp
 * @brief L9, live coding: the class Taxi (Steps 3, 4 and 7), which you write in class.
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

// Step 3: the class Taxi, derived from Vehicle, with the private data member
//         driver_name_, a std::string, and a constructor, written here in the
//         class, that takes the maximum speed and the driver's name.
// Step 4: the declaration drive(const Route&) const override.
// Step 7: the class becomes final.

}  // namespace fleet
