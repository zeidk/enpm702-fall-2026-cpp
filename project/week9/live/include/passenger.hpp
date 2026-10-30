/**
 * @file passenger.hpp
 * @brief L9, live coding: the class Passenger (Step 1), which you write in class.
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

// Step 1: the class Passenger. Its private data members are name_, a
//         std::string, and ride_, a std::weak_ptr<Vehicle>. Its public methods
//         are an explicit constructor that takes the name, written here in the
//         class (it throws std::invalid_argument for an empty name), and the
//         declarations of board(const std::shared_ptr<Vehicle>&) and leave().

}  // namespace fleet
