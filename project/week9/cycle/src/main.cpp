/**
 * @file main.cpp
 * @brief Two headers that need each other, built with forward declarations
 *        (L9, the cyclic include): target week9_cycle.
 * @author Zeid Kootbally
 *
 * @details A Passenger boards a Vehicle, and board adds the Passenger to the
 * Vehicle's riders: a two-way association. The program prints "built". The version
 * that includes each header from the other, and its errors, is in a comment
 * in ../CMakeLists.txt.
 * Run it: 702run week9_cycle
 */
// [Slide 29] A Forward Declaration Breaks the Cycle
#include <iostream>
#include <memory>

#include "passenger.hpp"
#include "vehicle.hpp"

int main() {
  auto car_a{std::make_shared<Vehicle>()};
  Passenger maya;
  maya.board(car_a);  // car_a now lists maya among its riders
  std::cout << "built\n";
}
