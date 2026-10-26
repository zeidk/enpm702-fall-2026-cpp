/**
 * @file passenger.hpp
 * @brief Passenger: a person who rides a vehicle (L9).
 * @author Zeid Kootbally
 *
 * @details A Passenger rides a Vehicle and neither owns the other
 * (association). It keeps a std::weak_ptr<Vehicle> (L7): it can reach the
 * vehicle while the vehicle exists, and it never keeps the vehicle alive.
 *
 * The invariant: the name is not empty.
 */
#pragma once

#include <memory>
#include <string>

#include "vehicle.hpp"

/// A person who rides at most one vehicle at a time.
class Passenger {
 public:
  /**
   * @param name The passenger's name.
   * @throw std::invalid_argument if @p name is empty.
   */
  explicit Passenger(std::string name);

  /// Boards a vehicle (FR-12). The passenger does not own it.
  void board(const std::shared_ptr<Vehicle>& vehicle) { ride_ = vehicle; }

  /// Gets off the vehicle (FR-12).
  void leave() noexcept { ride_.reset(); }

 private:
  std::string name_;            ///< not empty
  std::weak_ptr<Vehicle> ride_;  ///< association: does not own
};
