/**
 * @file undefined.cpp
 * @brief L8: the code of the reading page Rule of Five and Rule of Zero
 *        that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with -fsanitize=address,undefined (see
 * ../../CMakeLists.txt). The reports on the reading page come from these runs.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "sensor.hpp"

namespace undefined {

// [Reading: no_destructor] Without the Destructor
// The Vehicle of [Reading: raw_owner] with ~Vehicle removed. At exit, LeakSanitizer:
// Direct leak of 16 byte(s) in 1 object(s) allocated from: (three times)
// SUMMARY: AddressSanitizer: 48 byte(s) leaked in 3 allocation(s).
namespace no_destructor {
class Vehicle {  // a Sensor on the heap, never given back
 public:
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

// [Reading: double_free] Copying an Owner, and Double Free, Measured
// The Vehicle of [Reading: raw_owner], without the print in its destructor,
// copied by the compiler's copy constructor. AddressSanitizer stops the program:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace double_free {
class Vehicle {  // a Sensor on the heap
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() { delete sensor_; }
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

// [Reading: move_bug] Discussion: Who Deletes What?
// The move assignment does not set other.sensor_ to nullptr, so after line 9
// car_a and car_b point at the same Sensor (id 2), and the Sensor
// with id 1 was deleted. At the end of the scope both destructors delete id 2:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace move_bug {
class Vehicle {
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() { delete sensor_; }
  Vehicle(const Vehicle&) = delete;
  Vehicle& operator=(const Vehicle&) = delete;
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  Vehicle& operator=(Vehicle&& other) noexcept {
    delete sensor_;
    sensor_ = other.sensor_;
    return *this;
  }
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

}  // namespace undefined
