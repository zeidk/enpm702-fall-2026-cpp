/**
 * @file undefined.cpp
 * @brief L8: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with -fsanitize=address,undefined (see
 * ../../CMakeLists.txt). The reports on the slides come from these runs.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "sensor.hpp"

namespace undefined {

// [Slide 65] Without the Destructor
// The Vehicle of [Slide 64] with ~Vehicle removed. LeakSanitizer reports, at exit:
// Direct leak of 16 byte(s) in 1 object(s) allocated from: (three times)
// SUMMARY: AddressSanitizer: 48 byte(s) leaked in 3 allocation(s).
namespace no_destructor {
class Vehicle {  // owns one Sensor on the heap, and never gives it back
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  {
    Vehicle downtown_car{Sensor{1}};
    Vehicle airport_car{Sensor{2}};
    std::cout << "end of scope\n";
  }
  auto spare_car{std::make_unique<Vehicle>(Sensor{3})};
  spare_car.reset();
  std::cout << "after reset\n";
}
}  // namespace no_destructor

// [Slide 67] Double Free, Measured
// The Vehicle of [Slide 63], copied by the compiler's copy constructor
// ([Slide 66]). AddressSanitizer stops the program:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace double_free {
class Vehicle {  // a first owner: one Sensor on the heap
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
  Vehicle airport_car{Sensor{2, SensorType::camera, 80.0}};
  Vehicle copy{airport_car};  // the compiler's copy constructor
  std::cout << copy.get_sensor().get_id() << '\n';  // 2
}  // copy's destructor deletes the Sensor, then airport_car's deletes it again
}  // namespace double_free

// [Slide 72] Discussion: Who Deletes What?
// The move assignment does not set other.sensor_ to nullptr, so after line 9
// downtown_car and airport_car point at the same Sensor (id 2), and the Sensor
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
  Vehicle downtown_car{Sensor{1}};
  Vehicle airport_car{Sensor{2}};
  downtown_car = std::move(airport_car);
  std::cout << downtown_car.get_sensor().get_id() << '\n';  // 2
}
}  // namespace move_bug

}  // namespace undefined
