/**
 * @file fleet.cpp
 * @brief L9, live coding: the methods of Fleet (Steps 2 and 4).
 * @author Zeid Kootbally
 *
 * @details Step 2: each method below has an empty body, so the program builds
 * before you start. [[maybe_unused]] stops the unused-parameter warning while
 * a body is empty; delete it when you write the body.
 */
#include "fleet.hpp"

#include <algorithm>  // Step 2: std::find
#include <stdexcept>
#include <utility>    // Step 2: std::move

namespace fleet {

// Step 2: throw std::invalid_argument if vehicle is null. Otherwise, add it
//         to vehicles_ (std::move) only if std::find (from <algorithm>) does
//         not find it there: std::find returns vehicles_.end() when the
//         vehicle is not in vehicles_.
void Fleet::add([[maybe_unused]] std::shared_ptr<Vehicle> vehicle) {}

// Step 2: erase the vehicle whose get_id() is id, with std::erase_if (C++20),
//         which returns how many elements it removed. Return true if it
//         removed one. Replace "return false;".
bool Fleet::remove([[maybe_unused]] int id) { return false; }

// Step 2: return how many vehicles the fleet holds, the number of elements in
//         vehicles_. Replace "return 0;".
std::size_t Fleet::size() const noexcept { return 0; }

// Step 4: define Fleet::drive_all(const Route& route) const. Its body is one
//         range-based for loop over vehicles_ that calls drive(route) on each
//         vehicle, through its std::shared_ptr<Vehicle> (->). It names only
//         Vehicle: each object runs the drive() of its own class, so the Step 4
//         block prints the RoboTaxi line, then the Taxi line.

}  // namespace fleet
