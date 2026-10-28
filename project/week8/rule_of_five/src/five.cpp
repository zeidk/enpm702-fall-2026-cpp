/**
 * @file five.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Rule of Five: The Copies, The Moves, and All Five, Run.
 * @author Zeid Kootbally
 *
 * @details The Vehicle of raw_owner.cpp with all five special member functions
 * written. The destructor prints 0 when the Vehicle owns no Sensor (after a
 * move).
 * Run it: 702run week8_rule_of_five five
 */
#include <iostream>
#include <utility>

#include "parts.hpp"
#include "sensor.hpp"

namespace five {
class Vehicle {
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}

  // Destructor
  ~Vehicle() {
    std::cout << "~Vehicle: sensor " << (sensor_ != nullptr ? sensor_->get_id() : 0)
              << '\n';
    delete sensor_;
  }

  // Copy constructor: a new Sensor with other's values, if any
  Vehicle(const Vehicle& other)
      : sensor_{other.sensor_ != nullptr ? new Sensor{*other.sensor_} : nullptr} {}

  // Copy assignment
  Vehicle& operator=(const Vehicle& other) {
    if (this != &other) {  // v = v must not delete its own Sensor
      // Copy first: if new throws, this Vehicle keeps its old Sensor.
      Sensor* copy{other.sensor_ != nullptr ? new Sensor{*other.sensor_} : nullptr};
      delete sensor_;  // give back what we owned
      sensor_ = copy;
    }
    return *this;
  }

  // Move constructor
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;  // the source owns nothing now
  }

  // Move assignment
  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {   // v = std::move(v) must not delete
      delete sensor_;       // give back what we owned
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
  Vehicle copy{car_b};
  std::cout << "copy: sensor " << copy.get_sensor().get_id() << ", own Sensor: "
            << std::boolalpha << (&copy.get_sensor() != &car_b.get_sensor())
            << '\n';  // copy: sensor 2, own Sensor: true
  Vehicle car_a{Sensor{1}};
  copy = car_a;
  std::cout << "after copy assignment: sensor " << copy.get_sensor().get_id()
            << '\n';  // 1
  Vehicle moved{std::move(car_b)};
  std::cout << "moved: sensor " << moved.get_sensor().get_id() << '\n';  // 2
  car_a = std::move(moved);
  std::cout << "after move assignment: sensor " << car_a.get_sensor().get_id()
            << '\n';  // 2
}  // ~Vehicle: sensor 0, 2, 1, 0
}  // namespace five
