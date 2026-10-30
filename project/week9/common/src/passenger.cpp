/**
 * @file passenger.cpp
 * @brief The methods of Passenger. Its constructor is defined in the class, in
 *        passenger.hpp.
 * @author Zeid Kootbally
 */
#include "passenger.hpp"

namespace fleet {

void Passenger::board(const std::shared_ptr<Vehicle>& vehicle) { ride_ = vehicle; }

void Passenger::leave() noexcept { ride_.reset(); }

}  // namespace fleet
