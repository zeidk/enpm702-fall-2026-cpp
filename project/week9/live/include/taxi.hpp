/**
 * @file taxi.hpp
 * @brief L9, live coding: the class Taxi (Steps 3, 4 and 6), which you write in class.
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
//         class, that takes the maximum speed and the driver's name. The method
//         assign_driver(std::string driver_name), which Vehicle does not have,
//         is declared here and defined in taxi.cpp.
// Step 4: in the public part of Taxi, declare its own drive(), with the same
//         name, parameter and const as Vehicle's, then override:
//         void drive(const Route& route) const override;
//         Its body goes to taxi.cpp.
// Step 6: the class becomes final.

}  // namespace fleet
