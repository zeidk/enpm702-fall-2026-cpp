/**
 * @file passenger.hpp
 * @brief L9, live coding: the class Passenger (Step 1), given: we review it in class.
 * @author Zeid Kootbally
 *
 * @details A Passenger rides a Vehicle and does not own it (association).
 */
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>  // std::move

#include "vehicle.hpp"

namespace fleet {

// Step 1, given: a Passenger rides a Vehicle and does not own it (association).
/// A person who rides at most one vehicle at a time. The name is not empty.
class Passenger {
 public:
  /// Throws std::invalid_argument if @p name is empty.
  explicit Passenger(std::string name) : name_{std::move(name)} {
    if (name_.empty()) {
      throw std::invalid_argument{"a passenger has a name"};
    }
  }
  void board(const std::shared_ptr<Vehicle>& vehicle);
  void leave() noexcept;

 private:
  std::string name_;
  std::weak_ptr<Vehicle> ride_;  // does not own the Vehicle
};

}  // namespace fleet
