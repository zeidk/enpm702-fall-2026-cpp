/**
 * @file move_bug.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Discussion: Who Deletes What?.
 * @author Zeid Kootbally
 *
 * @details The move assignment does not set other.sensor_ to nullptr, so after
 * car_a = std::move(car_b); both point at the Sensor with id 2, and the one
 * with id 1 was deleted. AddressSanitizer stops the program: attempting
 * double-free.
 * Run it: 702run week8_rule_of_five move_bug
 */
#include <iostream>
#include <utility>

#include "parts.hpp"
#include "sensor.hpp"

namespace move_bug {
class Vehicle {
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  // Destructor
  ~Vehicle() { delete sensor_; }
  // Copy constructor and copy assignment: deleted
  Vehicle(const Vehicle&) = delete;
  Vehicle& operator=(const Vehicle&) = delete;
  // Move constructor
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  // Move assignment: one line is missing
  Vehicle& operator=(Vehicle&& other) noexcept {
    delete sensor_;
    sensor_ = other.sensor_;
    return *this;
  }
  // Accessor
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  Vehicle car_a{Sensor{1}};
  Vehicle car_b{Sensor{2}};
  car_a = std::move(car_b);
  std::cout << car_a.get_sensor().get_id() << '\n';  // 2
}
}  // namespace move_bug
