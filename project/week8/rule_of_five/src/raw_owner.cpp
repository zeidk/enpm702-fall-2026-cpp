/**
 * @file raw_owner.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        A Class That Owns Memory.
 * @author Zeid Kootbally
 *
 * @details A Vehicle that owns one Sensor on the heap through a raw pointer. The
 * constructor calls new, so the destructor calls delete. The destructor prints
 * the Sensor's id, to show when it runs.
 * Run it: 702run week8_rule_of_five raw_owner
 */
#include <iostream>
#include <memory>

#include "parts.hpp"
#include "sensor.hpp"

namespace raw_owner {
class Vehicle {  // a Sensor on the heap
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  // Destructor
  ~Vehicle() {
    std::cout << "~Vehicle: sensor "
              << sensor_->get_id()
              << '\n';
    delete sensor_;
  }

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
}  // prints ~Vehicle: sensor 2, then 1, then 3
}  // namespace raw_owner
