/**
 * @file fleet.cpp
 * @brief L9, ride service: the methods of Fleet (Steps 2 and 4).
 * @author Zeid Kootbally
 *
 * @details The Step 2 methods are given: we review them in class. You write
 * drive_all() in Step 4.
 */
#include "fleet.hpp"

#include <algorithm>  // Step 2: std::find
#include <stdexcept>
#include <utility>    // Step 2: std::move

namespace fleet {

// Step 2, given: a null vehicle is rejected, and a vehicle already in the
//         fleet is not added twice (std::find returns vehicles_.end() when the
//         vehicle is not in vehicles_).
void Fleet::add(std::shared_ptr<Vehicle> vehicle) {
  if (!vehicle) {
    throw std::invalid_argument{"a fleet cannot hold a null vehicle"};
  }
  if (std::find(vehicles_.begin(), vehicles_.end(), vehicle) == vehicles_.end()) {
    vehicles_.push_back(std::move(vehicle));
  }
}

// Step 2, given: std::erase_if (C++20) returns how many elements it removed.
bool Fleet::remove(int id) {
  return std::erase_if(vehicles_, [id](const std::shared_ptr<Vehicle>& vehicle) {
           return vehicle->get_id() == id;
         }) > 0;
}

// Step 2, given.
std::size_t Fleet::size() const noexcept { return vehicles_.size(); }

// Step 4. The loop names only Vehicle: each object runs the drive() of its own
// class, so the Step 4 block prints the RoboTaxi line, then the Taxi line.
void Fleet::drive_all(const Route& route) const {
  for (const std::shared_ptr<Vehicle>& vehicle : vehicles_) {
    vehicle->drive(route);
  }
}

}  // namespace fleet
