/**
 * @file zero.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        What the Compiler Writes Now (Rule of Zero).
 * @author Zeid Kootbally
 *
 * @details A Vehicle that owns one Sensor through a std::unique_ptr and writes
 * none of the five special member functions: the compiler writes them. The
 * copy does not compile: uncomment it to see the error.
 * Run it: 702run week8_rule_of_five zero
 */
#include <iostream>
#include <memory>
#include <utility>

#include "parts.hpp"
#include "sensor.hpp"

namespace zero {
class Vehicle {  // one Sensor only; no destructor, copy or move written
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{std::make_unique<Sensor>(sensor)} {}
  // Accessor
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  std::unique_ptr<Sensor> sensor_;  // deletes the Sensor
};

void run() {
  Vehicle car_b{Sensor{2, SensorType::camera, 80.0}};
  Vehicle moved{std::move(car_b)};  // OK: moves the unique_ptr
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
  // Vehicle copy{moved};  // does not compile: the copy constructor is deleted
}
}  // namespace zero
