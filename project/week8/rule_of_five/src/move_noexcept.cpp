/**
 * @file move_noexcept.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Why the Moves Are noexcept.
 * @author Zeid Kootbally
 *
 * @details Two classes that own nothing and only count their copies and moves.
 * They differ in one word: Vehicle's move constructor is noexcept,
 * RiskyVehicle's is not. A growing std::vector moves its elements only when
 * the move constructor is noexcept; otherwise it copies them.
 * Run it: 702run week8_rule_of_five move_noexcept
 */
#include <iostream>
#include <vector>

#include "parts.hpp"

namespace move_noexcept {
int copies{0};  // how many copies and moves the vector made
int moves{0};

class Vehicle {  // its move constructor is noexcept
 public:
  // Default constructor
  Vehicle() = default;
  // Copy constructor: counts
  Vehicle(const Vehicle&) { ++copies; }
  // Move constructor: counts, and cannot throw
  Vehicle(Vehicle&&) noexcept { ++moves; }
};

class RiskyVehicle {  // the same, but its move constructor is not noexcept
 public:
  // Default constructor
  RiskyVehicle() = default;
  // Copy constructor: counts
  RiskyVehicle(const RiskyVehicle&) { ++copies; }
  // Move constructor: counts, and may throw
  RiskyVehicle(RiskyVehicle&&) { ++moves; }
};

void run() {
  std::vector<Vehicle> fleet;
  for (int i{0}; i < 5; ++i) {
    fleet.emplace_back();
  }
  std::cout << "with noexcept: " << copies << " copies, " << moves << " moves\n";

  copies = 0;
  moves = 0;
  std::vector<RiskyVehicle> risky_fleet;
  for (int i{0}; i < 5; ++i) {
    risky_fleet.emplace_back();
  }
  std::cout << "without it: " << copies << " copies, " << moves << " moves\n";
}
}  // namespace move_noexcept
