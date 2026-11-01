/**
 * @file fleet.hpp
 * @brief L9, live coding: the class Fleet (Steps 2 and 4), which you write in class.
 * @author Zeid Kootbally
 *
 * @details A Fleet shares its vehicles with the rest of the program
 * (aggregation, Step 2) and drives them all in one loop (Step 4). It names
 * only Vehicle, so a new kind of vehicle needs no change here.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "route.hpp"
#include "vehicle.hpp"

namespace fleet {

// Step 2: the class Fleet. Its private data member is vehicles_, a
//         std::vector<std::shared_ptr<Vehicle>>. Its public methods, declared
//         here: add(std::shared_ptr<Vehicle>), bool remove(int id) and size().
// Step 4: in the public part of Fleet, declare the method drive_all(). It
//         returns nothing, takes the route by const reference and does not
//         change the Fleet (const):
//         void drive_all(const Route& route) const;
//         Its body goes to fleet.cpp.

}  // namespace fleet
