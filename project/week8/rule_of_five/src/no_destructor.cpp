/**
 * @file no_destructor.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Without the Destructor.
 * @author Zeid Kootbally
 *
 * @details The Vehicle of raw_owner.cpp without its destructor: the compiler's
 * destructor does not delete the Sensor. At exit, LeakSanitizer reports
 * 48 byte(s) leaked in 3 allocation(s).
 * Run it: 702run week8_rule_of_five no_destructor
 */
#include <memory>

#include "parts.hpp"
#include "sensor.hpp"

namespace no_destructor {
class Vehicle {  // a Sensor on the heap, never given back
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}

 private:
  Sensor* sensor_;
};

void run() {
  {
    Vehicle car_a{Sensor{1}};
    Vehicle car_b{Sensor{2}};
  }
  auto spare_car{
    std::make_unique<Vehicle>(
        Sensor{3})};
  spare_car.reset();
}
}  // namespace no_destructor
