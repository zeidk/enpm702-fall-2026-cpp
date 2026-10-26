/**
 * @file passenger.cpp
 * @brief The methods of Passenger that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "passenger.hpp"

#include <stdexcept>
#include <utility>

Passenger::Passenger(std::string name) : name_{std::move(name)} {
  if (name_.empty()) {
    throw std::invalid_argument{"a passenger has a name"};
  }
}
