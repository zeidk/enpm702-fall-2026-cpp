/**
 * @file destructors.cpp
 * @brief L8 Section 8, Destructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. The slides build Vehicle's ownership of its
 * sensors in steps, and each step's Vehicle is its own class, in its own
 * namespace: one raw Sensor* and a destructor ([Slide 63]), the rule of five
 * ([Slides 69 to 71]), a std::unique_ptr ([Slides 73 and 74]), deleted copies
 * ([Slide 75]), then the sensors_ of the class diagram ([Slide 76]). Each step
 * leaves out the speed members of the earlier sections. The finished class is
 * ../../common/include/vehicle.hpp.
 *
 * The leak of [Slide 65] and the double free of [Slide 67] are in
 * ../../undefined/src/undefined.cpp, built with the sanitizers. They run only
 * when asked for by number.
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "sections.hpp"
#include "sensor.hpp"
#include "undefined.hpp"

namespace destructors {

// [Slide 64] When It Runs: the Vehicle of [Slide 63], and ~Vehicle also prints.
namespace when_it_runs {
class Vehicle {  // a first owner: one Sensor on the heap
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() {
    std::cout << "~Vehicle: sensor " << sensor_->get_id() << '\n';
    delete sensor_;
  }
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  {
    Vehicle downtown_car{Sensor{1}};  // sensor id 1;
    Vehicle airport_car{Sensor{2}};   // the rest default
    std::cout << "end of scope\n";
  }
  auto spare_car{
      std::make_unique<Vehicle>(Sensor{3})};
  spare_car.reset();
  std::cout << "after reset\n";
}
}  // namespace when_it_runs

// [Slides 69 to 71] The Rule of Five. ~Vehicle prints as on [Slide 64], and 0
// when the Vehicle owns nothing (after a move).
namespace five {
class Vehicle {
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}

  ~Vehicle() {
    std::cout << "~Vehicle: sensor " << (sensor_ != nullptr ? sensor_->get_id() : 0) << '\n';
    delete sensor_;
  }

  // [Slide 69] The Copies
  Vehicle(const Vehicle& other)  // a new Sensor with other's values
      : sensor_{new Sensor{*other.sensor_}} {}

  Vehicle& operator=(const Vehicle& other) {
    *sensor_ = *other.sensor_;  // into our own Sensor
    return *this;
  }

  // [Slide 70] The Moves
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;  // the source owns nothing now
  }

  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {   // v = std::move(v) must not delete
      delete sensor_;       // give back what we owned
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

// [Slide 71] All Five, Run
void run() {
  Vehicle airport_car{Sensor{2}};
  Vehicle copy{airport_car};
  std::cout << "copy: sensor " << copy.get_sensor().get_id() << ", own Sensor: "
            << (&copy.get_sensor() != &airport_car.get_sensor()) << '\n';  // copy: sensor 2, own Sensor: 1
  Vehicle downtown_car{Sensor{1}};
  copy = downtown_car;
  std::cout << "after copy assignment: sensor " << copy.get_sensor().get_id() << '\n';  // 1
  Vehicle moved{std::move(airport_car)};
  std::cout << "moved: sensor " << moved.get_sensor().get_id() << '\n';  // 2
  downtown_car = std::move(moved);
  std::cout << "after move assignment: sensor " << downtown_car.get_sensor().get_id() << '\n';  // 2
}  // ~Vehicle: sensor 0, 2, 1, 0
}  // namespace five

// [Slides 73 and 74] The Rule of Zero
namespace zero {
class Vehicle {
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{std::make_unique<Sensor>(sensor)} {}
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  std::unique_ptr<Sensor> sensor_;  // deletes the Sensor
};

void run() {
  Vehicle airport_car{Sensor{2, SensorType::camera, 80.0}};
  Vehicle moved{std::move(airport_car)};  // OK: moves the unique_ptr
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
  // Vehicle copy{moved};                 // does not compile:
  // error: use of deleted function 'Vehicle::Vehicle(const Vehicle&)'
  // note: 'Vehicle::Vehicle(const Vehicle&)' is implicitly deleted because the
  //       default definition would be ill-formed:
}
}  // namespace zero

// [Slide 75] = default and = delete: the raw-pointer Vehicle, move-only.
// Remove the two move lines and Vehicle moved{std::move(airport_car)}; picks the
// deleted copy: error: use of deleted function 'Vehicle::Vehicle(const Vehicle&)'
namespace move_only {
class Vehicle {  // the raw-pointer Vehicle, move-only on purpose
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() { delete sensor_; }
  Vehicle(const Vehicle&) = delete;             // no copies
  Vehicle& operator=(const Vehicle&) = delete;
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {
      delete sensor_;
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  Vehicle airport_car{Sensor{2}};
  Vehicle moved{std::move(airport_car)};
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
}
}  // namespace move_only

// [Slide 76] Many Sensors: the sensors_ of the class diagram. The finished
// class, with the speed members and the id, is ../../common/include/vehicle.hpp.
namespace many_sensors {
class Vehicle {  // owns one Sensor for each sensor it carries
 public:
  int add_sensor(SensorType type, double range) {
    const int id{static_cast<int>(sensors_.size()) + 1};
    sensors_.push_back(std::make_unique<Sensor>(id, type, range));
    return id;
  }
  [[nodiscard]] std::size_t sensor_count() const noexcept {
    return sensors_.size();
  }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
};

void run() {
  Vehicle downtown_car;
  const int lidar{downtown_car.add_sensor(SensorType::lidar, 100.0)};
  const int camera{downtown_car.add_sensor(SensorType::camera, 80.0)};
  std::cout << lidar << ' ' << camera << ' ' << downtown_car.sensor_count() << '\n';  // 1 2 2
  // Vehicle copy{downtown_car};  // does not compile: errors from inside std::vector,
  // use of deleted function 'std::unique_ptr<...>::unique_ptr(const std::unique_ptr<...>&)'
}
}  // namespace many_sensors

std::vector<Slide> slides() {
  return {
      {64, "When It Runs", when_it_runs::run},
      {65, "Without the Destructor", undefined::no_destructor::run, true},
      {67, "Double Free, Measured", undefined::double_free::run, true},
      {71, "All Five, Run", five::run},
      {74, "What the Compiler Writes Now", zero::run},
      {75, "= default and = delete", move_only::run},
      {76, "Many Sensors", many_sensors::run},
      {72, "Discussion: Who Deletes What?", undefined::move_bug::run, true},
  };
}

}  // namespace destructors
