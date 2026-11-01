/**
 * @file passenger.hpp
 * @brief A small Passenger that rides a Vehicle: the other side of the
 *        two-way association (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details ride_ is a std::weak_ptr<Vehicle>, so this header needs only the
 * name Vehicle: a forward declaration. The #include "vehicle.hpp" is in
 * passenger.cpp.
 */
// [Slide 29] A Forward Declaration Breaks the Cycle
#pragma once
#include <memory>
class Vehicle;  // the name only
class Passenger {
 public:
  void board(const std::shared_ptr<Vehicle>& vehicle);
 private:
  std::weak_ptr<Vehicle> ride_;
};
