/**
 * @file double_free.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Copying an Owner.
 * @author Zeid Kootbally
 *
 * @details The Vehicle of raw_owner.cpp, without the print in its destructor,
 * copied by the compiler's copy constructor. AddressSanitizer stops the
 * program: attempting double-free.
 * Run it: 702run week8_rule_of_five double_free
 */
#include <iostream>

#include "parts.hpp"
#include "sensor.hpp"

namespace double_free {
class Vehicle {  // a Sensor on the heap
 public:
  // Constructor
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  // Destructor
  ~Vehicle() { delete sensor_; }
  // Accessor
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  Vehicle car_b{Sensor{2, SensorType::camera, 80.0}};
  Vehicle copy{car_b};  // the compiler's copy constructor
  std::cout << copy.get_sensor().get_id() << '\n';  // 2
}  // copy's destructor deletes the Sensor, then car_b's deletes it again
}  // namespace double_free
