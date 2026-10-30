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
// Step 4: the declaration of Fleet's method drive_all(const Route&), which is
//         const.

}  // namespace fleet
