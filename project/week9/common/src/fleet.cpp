/**
 * @file fleet.cpp
 * @brief The methods of Fleet.
 * @author Zeid Kootbally
 */
#include "fleet.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace fleet {

void Fleet::add(std::shared_ptr<Vehicle> vehicle) {
  if (!vehicle) {
    throw std::invalid_argument{"a fleet cannot hold a null vehicle"};
  }
  if (std::find(vehicles_.begin(), vehicles_.end(), vehicle) == vehicles_.end()) {
    vehicles_.push_back(std::move(vehicle));
  }
}

bool Fleet::remove(int id) {
  return std::erase_if(vehicles_, [id](const std::shared_ptr<Vehicle>& vehicle) {
           return vehicle->get_id() == id;
         }) > 0;
}

std::size_t Fleet::size() const noexcept { return vehicles_.size(); }

}  // namespace fleet
