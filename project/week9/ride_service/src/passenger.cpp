/**
 * @file passenger.cpp
 * @brief L9, ride service: the methods of Passenger (Step 1).
 * @author Zeid Kootbally
 */
#include "passenger.hpp"


namespace fleet {

// Step 1, given.
void Passenger::board(const std::shared_ptr<Vehicle>& vehicle) { ride_ = vehicle; }

// Step 1, given.
void Passenger::leave() noexcept { ride_.reset(); }

}  // namespace fleet
