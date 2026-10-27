/**
 * @file fleet.hpp
 * @brief Fleet: the vehicles one company runs (L9).
 * @author Zeid Kootbally
 *
 * @details A Fleet holds vehicles it does not own alone (aggregation): it keeps
 * std::shared_ptr<Vehicle>, so a vehicle can exist before the fleet, after it,
 * and in another fleet too. Removing a vehicle from the fleet does not destroy
 * it while other code still holds it.
 *
 * The invariant: no null pointer and no vehicle twice. Fleet names only
 * Vehicle, never RoboTaxi or Taxi, so a new kind of vehicle needs no change here.
 */
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "vehicle.hpp"

/// The vehicles of a fleet, shared with the rest of the program.
class Fleet {
 public:
  /**
   * @brief Adds a vehicle to the fleet. A vehicle already in it is not added again.
   * @throw std::invalid_argument if @p vehicle is null.
   */
  void add(std::shared_ptr<Vehicle> vehicle);

  /**
   * @brief Removes the vehicle with this id. The vehicle lives on while other
   *        code holds it.
   * @return False when no vehicle of the fleet has that id.
   */
  bool remove(int id);

  /// How many vehicles the fleet holds.
  [[nodiscard]] std::size_t size() const noexcept;

 private:
  std::vector<std::shared_ptr<Vehicle>> vehicles_;  ///< aggregation: shared
};
