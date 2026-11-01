/**
 * @file passenger.cpp
 * @brief The method of the small Passenger (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details board calls Vehicle::add_rider, and a call needs the full class, so
 * this file includes vehicle.hpp. Without it, g++ 13.3 stops with
 * "invalid use of incomplete type ... 'class Vehicle'".
 */
// [Slide 29] A Forward Declaration Breaks the Cycle
#include "passenger.hpp"
#include "vehicle.hpp"  // a call needs the full class
void Passenger::board(const std::shared_ptr<Vehicle>& vehicle) {
  ride_ = vehicle;
  vehicle->add_rider(*this);
}
