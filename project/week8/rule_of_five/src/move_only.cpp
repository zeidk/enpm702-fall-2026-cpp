/**
 * @file move_only.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        = default and = delete.
 * @author Zeid Kootbally
 *
 * @details The raw-pointer Vehicle, move-only on purpose: its copies are deleted
 * and its moves are written. Remove the two moves and
 * Vehicle moved{std::move(car_b)}; picks the deleted copy constructor, so it
 * does not compile.
 * Run it: 702run week8_rule_of_five move_only
 */
#include <iostream>
#include <utility>

#include "parts.hpp"
#include "sensor.hpp"

namespace move_only {
class Vehicle {  // the raw-pointer Vehicle, move-only on purpose
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  // Destructor
  ~Vehicle() { delete sensor_; }
  // Copy constructor and copy assignment: deleted, so no copies
  Vehicle(const Vehicle&) = delete;
  Vehicle& operator=(const Vehicle&) = delete;
  // Move constructor
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  // Move assignment
  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {
      delete sensor_;
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
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
  Vehicle car_b{Sensor{2}};
  Vehicle moved{std::move(car_b)};
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
}
}  // namespace move_only
