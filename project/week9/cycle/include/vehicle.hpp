/**
 * @file vehicle.hpp
 * @brief A small Vehicle that keeps its riders: one side of a two-way
 *        association (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details riders_ holds pointers to Passenger, so this header needs only the
 * name Passenger: a forward declaration. The #include "passenger.hpp" is in
 * vehicle.cpp, where the full class is used.
 */
// [Slide 29] A Forward Declaration Breaks the Cycle
#pragma once
#include <vector>
class Passenger;  // the name only
class Vehicle {
 public:
  void add_rider(const Passenger& passenger);
 private:
  std::vector<const Passenger*> riders_;
};
