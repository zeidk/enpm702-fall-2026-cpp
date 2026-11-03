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
/**
 * @brief A person who rides at most one vehicle at a time.
 *
 * A Passenger keeps a std::weak_ptr to its vehicle, so it does not keep the
 * vehicle alive (association). Invariant: the name is not empty.
 */
class Passenger {
 public:
  /**
   * @brief A passenger with no ride yet.
   * @param name The passenger's name. Must not be empty.
   * @throw std::invalid_argument if @p name is empty.
   */
  explicit Passenger(std::string name) : name_{std::move(name)} {
    if (name_.empty()) {
      throw std::invalid_argument{"a passenger has a name"};
    }
  }
  /**
   * @brief Boards @p vehicle: it becomes the passenger's ride.
   * @param vehicle The vehicle to ride. The passenger keeps a std::weak_ptr to
   *        it, so the call adds no owner.
   */
  void board(const std::shared_ptr<Vehicle>& vehicle);
  /// @brief Leaves the vehicle: the passenger has no ride.
  void leave() noexcept;

 private:
  std::string name_;             ///< not empty
  /// The vehicle ridden; does not own it. The constructor does not set it: a
  /// default-constructed std::weak_ptr is empty (C++20 [util.smartptr.weak.const]),
  /// so expired() is true and lock() returns an empty pointer until board().
  std::weak_ptr<Vehicle> ride_;
};

}  // namespace fleet
