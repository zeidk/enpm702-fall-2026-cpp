/**
 * @file no_move.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Without the Moves.
 * @author Zeid Kootbally
 *
 * @details The Vehicle declares a destructor, so the compiler writes no move
 * constructor and no move assignment. std::move(car_b) then finds no move
 * constructor, and the compiler's copy constructor runs: it copies the
 * address. AddressSanitizer stops the program: attempting double-free.
 * Run it: 702run week8_rule_of_five no_move
 */
#include <utility>

#include "parts.hpp"
#include "sensor.hpp"

namespace no_move {
class Vehicle {  // a destructor, and no copy or move written
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  // Destructor
  ~Vehicle() { delete sensor_; }

 private:
  Sensor* sensor_;
};

void run() {
  Vehicle car_b{Sensor{2}};
  Vehicle moved{std::move(car_b)};  // the compiler's copy constructor runs
}  // moved deletes the Sensor, then car_b deletes it again
}  // namespace no_move
