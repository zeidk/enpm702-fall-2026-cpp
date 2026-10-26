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

// [Slide 62] Without the Destructor
// The SensorMount of [Slide 61] with ~SensorMount removed. LeakSanitizer
// reports, at exit:
// Direct leak of 16 byte(s) in 1 object(s) allocated from: (three times)
// SUMMARY: AddressSanitizer: 48 byte(s) leaked in 3 allocation(s).
namespace no_destructor {
class SensorMount {  // owns one Sensor on the heap, and never gives it back
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  {
    SensorMount roof{Sensor{1}};
    SensorMount front{Sensor{2}};
    std::cout << "end of scope\n";
  }
  auto rear{std::make_unique<SensorMount>(Sensor{3})};
  rear.reset();
  std::cout << "after reset\n";
}
}  // namespace no_destructor

// [Slide 64] Double Free, Measured
// The SensorMount of [Slide 60], copied by the compiler's copy constructor
// ([Slide 63]). AddressSanitizer stops the program:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace double_free {
class SensorMount {  // owns one Sensor on the heap
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~SensorMount() { delete sensor_; }
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  SensorMount front{Sensor{2, SensorType::camera, 80.0}};
  SensorMount copy{front};  // the compiler's copy constructor
  std::cout << copy.get().get_id() << '\n';  // 2
}  // copy's destructor deletes the Sensor, then front's deletes it again
}  // namespace double_free

// [Slide 69] Discussion: Who Deletes What?
// The move assignment does not set other.sensor_ to nullptr, so after line 9
// roof and front point at the same Sensor (id 2), and the Sensor with id 1
// was deleted. At the end of the scope both destructors delete id 2:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace move_bug {
class SensorMount {
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~SensorMount() { delete sensor_; }
  SensorMount(const SensorMount&) = delete;
  SensorMount& operator=(const SensorMount&) = delete;
  SensorMount(SensorMount&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  SensorMount& operator=(SensorMount&& other) noexcept {
    delete sensor_;
    sensor_ = other.sensor_;
    return *this;
  }
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  SensorMount roof{Sensor{1}};
  SensorMount front{Sensor{2}};
  roof = std::move(front);
  std::cout << roof.get().get_id() << '\n';  // 2
}
}  // namespace move_bug

}  // namespace undefined
