/**
 * @file fleet.cpp
 * @brief L9, live coding: the methods of Fleet (Steps 2 and 4).
 * @author Zeid Kootbally
 */
#include "fleet.hpp"

#include <algorithm>  // Step 2: std::find
#include <stdexcept>
#include <utility>    // Step 2: std::move

namespace fleet {

// Step 2: Fleet::add() throws std::invalid_argument for a null pointer, and
//         skips a vehicle already in the fleet: std::find (from <algorithm>)
//         returns vehicles_.end() when the vehicle is not there.
//         Fleet::remove() erases the vehicle with that id (std::erase_if) and
//         returns true if it removed one. Fleet::size() returns how many
//         vehicles the fleet holds.

// Step 4: define Fleet::drive_all(const Route& route) const. Its body is one
//         range-based for loop over vehicles_ that calls drive(route) on each
//         vehicle, through its std::shared_ptr<Vehicle> (->). It names only
//         Vehicle: each object runs the drive() of its own class, so the Step 4
//         block prints the RoboTaxi line, then the Taxi line.

}  // namespace fleet
