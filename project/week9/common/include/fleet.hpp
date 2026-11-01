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

/**
 * @brief The classes of the ride service program (L9).
 *
 * Every class of the program is in fleet, so common names such as Vehicle,
 * Route and Sensor cannot clash with the same names in another library. The
 * namespace is documented here only: Doxygen joins every comment it finds on a
 * namespace, so the other headers open it without one.
 */
namespace fleet {

/**
 * @brief The vehicles one company runs.
 *
 * A Fleet shares its vehicles with the rest of the program (aggregation): it
 * keeps a std::shared_ptr to each, so a vehicle can exist before the fleet,
 * after it, and in another fleet. It names only Vehicle, never RoboTaxi or
 * Taxi, so a new kind of vehicle needs no change here.
 *
 * Invariant: no null pointer and no vehicle twice.
 */
class Fleet {
 public:
  /**
   * @brief Adds @p vehicle to the fleet, unless the fleet already holds it.
   * @param vehicle The vehicle to add. The fleet becomes one of its owners.
   * @throw std::invalid_argument if @p vehicle is null.
   */
  void add(std::shared_ptr<Vehicle> vehicle);

  /**
   * @brief Removes the vehicle whose id is @p id from the fleet.
   * @param id The id of the vehicle to remove, as Vehicle::get_id() gives it.
   * @return true if a vehicle was removed, false if no vehicle of the fleet
   *         has this id.
   * @note The vehicle lives on while other code still holds it.
   */
  bool remove(int id);

  /**
   * @brief Returns how many vehicles the fleet holds.
   * @return The number of elements in vehicles_.
   */
  [[nodiscard]] std::size_t size() const noexcept;

 private:
  /// The fleet's vehicles. Each std::shared_ptr makes the fleet one of the
  /// vehicle's owners (aggregation). Invariant: no null pointer and no vehicle
  /// twice.
  std::vector<std::shared_ptr<Vehicle>> vehicles_;
};

}  // namespace fleet
