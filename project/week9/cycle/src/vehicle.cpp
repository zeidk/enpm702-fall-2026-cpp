/**
 * @file vehicle.cpp
 * @brief The method of the small Vehicle (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details add_rider stores the address of a Passenger and calls none of its
 * methods, so the name from the forward declaration is enough: this file does
 * not include passenger.hpp.
 */
// [Slide 29] A Forward Declaration Breaks the Cycle
#include "vehicle.hpp"
void Vehicle::add_rider(const Passenger& passenger) { riders_.push_back(&passenger); }
