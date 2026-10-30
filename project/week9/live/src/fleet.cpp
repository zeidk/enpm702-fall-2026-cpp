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
//         adds the vehicle only if std::find does not find it in vehicles_.
//         Fleet::remove() erases the vehicle with that id (std::erase_if) and
//         returns true if it removed one. Fleet::size() returns how many
//         vehicles the fleet holds.

// Step 4: Fleet::drive_all() calls drive() on every vehicle, in one loop.

}  // namespace fleet
