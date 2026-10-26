/**
 * @file destructors.cpp
 * @brief L8 Section 8, Destructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. Each slide's SensorMount is its own class,
 * in its own namespace, because the class changes from slide to slide: a raw
 * pointer and a destructor ([Slide 60]), the rule of five ([Slides 66 to 68]),
 * a std::unique_ptr ([Slides 70 and 71]), deleted copies ([Slide 72]).
 *
 * The leak of [Slide 62] and the double free of [Slide 64] are in
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

// [Slide 61] When It Runs: the SensorMount of [Slide 60], and ~SensorMount
// also prints.
namespace when_it_runs {
class SensorMount {  // owns one Sensor on the heap
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~SensorMount() {
    std::cout << "~SensorMount: id " << sensor_->get_id() << '\n';
    delete sensor_;
  }
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  {
    SensorMount roof{Sensor{1}};   // id 1; the
    SensorMount front{Sensor{2}};  // rest default
    std::cout << "end of scope\n";
  }
  auto rear{
      std::make_unique<SensorMount>(Sensor{3})};
  rear.reset();
  std::cout << "after reset\n";
}
}  // namespace when_it_runs

// [Slides 66 to 68] The Rule of Five. ~SensorMount prints as on [Slide 61],
// and 0 when the SensorMount owns nothing (after a move).
namespace five {
class SensorMount {
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}

  ~SensorMount() {
    std::cout << "~SensorMount: id " << (sensor_ != nullptr ? sensor_->get_id() : 0) << '\n';
    delete sensor_;
  }

  // [Slide 66] The Copies
  SensorMount(const SensorMount& other)  // a new Sensor with other's values
      : sensor_{new Sensor{*other.sensor_}} {}

  SensorMount& operator=(const SensorMount& other) {
    *sensor_ = *other.sensor_;  // into our own Sensor
    return *this;
  }

  // [Slide 67] The Moves
  SensorMount(SensorMount&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;  // the source owns nothing now
  }

  SensorMount& operator=(SensorMount&& other) noexcept {
    if (this != &other) {   // m = std::move(m) must not delete
      delete sensor_;       // give back what we owned
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

// [Slide 68] All Five, Run
void run() {
  SensorMount front{Sensor{2}};
  SensorMount copy{front};
  std::cout << "copy: id " << copy.get().get_id() << ", own Sensor: "
            << (&copy.get() != &front.get()) << '\n';  // copy: id 2, own Sensor: 1
  SensorMount roof{Sensor{1}};
  copy = roof;
  std::cout << "after copy assignment: id " << copy.get().get_id() << '\n';  // id 1
  SensorMount moved{std::move(front)};
  std::cout << "moved: id " << moved.get().get_id() << '\n';  // id 2
  roof = std::move(moved);
  std::cout << "after move assignment: id " << roof.get().get_id() << '\n';  // id 2
}  // ~SensorMount: id 0, id 2, id 1, id 0
}  // namespace five

// [Slides 70 and 71] The Rule of Zero
namespace zero {
class SensorMount {
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{std::make_unique<Sensor>(sensor)} {}
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  std::unique_ptr<Sensor> sensor_;  // deletes the Sensor
};

void run() {
  SensorMount front{Sensor{2, SensorType::camera, 80.0}};
  SensorMount moved{std::move(front)};  // OK: moves the unique_ptr
  std::cout << moved.get().get_id() << '\n';  // 2
  // SensorMount copy{moved};           // does not compile:
  // error: use of deleted function 'SensorMount::SensorMount(const SensorMount&)'
  // note: 'SensorMount::SensorMount(const SensorMount&)' is implicitly deleted
  //       because the default definition would be ill-formed:
}
}  // namespace zero

// [Slide 72] = default and = delete: the raw-pointer SensorMount, move-only.
// Remove the two move lines and SensorMount moved{std::move(front)}; picks the
// deleted copy: error: use of deleted function
// 'SensorMount::SensorMount(const SensorMount&)'
namespace move_only {
class SensorMount {  // the raw-pointer SensorMount, move-only on purpose
 public:
  explicit SensorMount(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~SensorMount() { delete sensor_; }
  SensorMount(const SensorMount&) = delete;  // no copies
  SensorMount& operator=(const SensorMount&) = delete;
  SensorMount(SensorMount&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  SensorMount& operator=(SensorMount&& other) noexcept {
    if (this != &other) {
      delete sensor_;
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }
  [[nodiscard]] const Sensor& get() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  SensorMount front{Sensor{2}};
  SensorMount moved{std::move(front)};
  std::cout << moved.get().get_id() << '\n';  // 2
}
}  // namespace move_only

// [Slide 73] The RoboTaxi, with an id counter in each object. Section 9 makes
// the counter static: ../../common/include/robotaxi.hpp.
namespace robotaxi {
class RoboTaxi {  // owns one Sensor for each sensor it carries
 public:
  int add_sensor(SensorType type, double range) {
    const int id{next_id_++};
    sensors_.push_back(std::make_unique<Sensor>(id, type, range));
    return id;
  }
  bool remove_sensor(int id) {
    return std::erase_if(sensors_, [id](const std::unique_ptr<Sensor>& sensor) {
             return sensor->get_id() == id;
           }) > 0;
  }
  [[nodiscard]] const Sensor* find_sensor(int id) const {
    for (const std::unique_ptr<Sensor>& sensor : sensors_) {
      if (sensor->get_id() == id) {
        return sensor.get();
      }
    }
    return nullptr;
  }
  [[nodiscard]] std::size_t sensor_count() const noexcept {
    return sensors_.size();
  }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
  int next_id_{1};  // the id add_sensor() gives next
};

void run() {
  RoboTaxi taxi;
  const int lidar{taxi.add_sensor(SensorType::lidar, 100.0)};
  const int camera{taxi.add_sensor(SensorType::camera, 80.0)};
  std::cout << lidar << ' ' << camera << ' ' << taxi.sensor_count() << '\n';  // 1 2 2
  if (const Sensor* found{taxi.find_sensor(camera)}) {
    std::cout << found->get_range() << '\n';  // 80
  }
  taxi.remove_sensor(camera);
  std::cout << taxi.sensor_count() << ' '
            << (taxi.find_sensor(camera) == nullptr) << '\n';  // 1 1
  // RoboTaxi copy{taxi};  // does not compile: errors from inside std::vector,
  // use of deleted function 'std::unique_ptr<...>::unique_ptr(const std::unique_ptr<...>&)'
}
}  // namespace robotaxi

std::vector<Slide> slides() {
  return {
      {61, "When It Runs", when_it_runs::run},
      {62, "Without the Destructor", undefined::no_destructor::run, true},
      {64, "Double Free, Measured", undefined::double_free::run, true},
      {68, "All Five, Run", five::run},
      {71, "What the Compiler Writes Now", zero::run},
      {72, "= default and = delete", move_only::run},
      {73, "The RoboTaxi", robotaxi::run},
      {69, "Discussion: Who Deletes What?", undefined::move_bug::run, true},
  };
}

}  // namespace destructors
