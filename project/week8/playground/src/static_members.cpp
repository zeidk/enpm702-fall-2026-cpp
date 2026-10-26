/**
 * @file static_members.cpp
 * @brief L8 Section 9, static Members: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. [Slide 75] uses a RoboTaxi with a counter
 * in each object; [Slides 76 and 77] use the RoboTaxi of
 * ../../common/include/robotaxi.hpp, whose counter is static.
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include "robotaxi.hpp"
#include "sections.hpp"
#include "sensor.hpp"

namespace static_members {

// [Slide 75] Two RoboTaxis, the Same Id
namespace same_ids {
class RoboTaxi {  // the RoboTaxi of [Slide 73]: one counter in each object
 public:
  int add_sensor(SensorType type, double range) {
    const int id{next_id_++};
    sensors_.push_back(std::make_unique<Sensor>(id, type, range));
    return id;
  }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
  int next_id_{1};  // the id add_sensor() gives next
};

void run() {
  RoboTaxi taxi_a;  // two robotaxis of one fleet
  RoboTaxi taxi_b;
  const int id_a{taxi_a.add_sensor(SensorType::lidar, 100.0)};
  const int id_b{taxi_b.add_sensor(SensorType::lidar, 100.0)};
  std::cout << id_a << ' ' << id_b << '\n';  // 1 1
  std::cout << sizeof(RoboTaxi) << '\n';  // 32: the vector (24) and the counter
}
}  // namespace same_ids

// [Slide 76] One Counter for the Class
namespace one_counter {
void run() {
  ::RoboTaxi taxi_a;
  ::RoboTaxi taxi_b;
  std::cout << taxi_a.add_sensor(SensorType::lidar, 100.0) << ' '
            << taxi_a.add_sensor(SensorType::camera, 80.0) << ' '
            << taxi_b.add_sensor(SensorType::lidar, 100.0) << '\n';  // 1 2 3
  std::cout << ::RoboTaxi::next_id() << '\n';                         // 4
  std::cout << sizeof(::RoboTaxi) << '\n';                            // 24: the vector only
}
}  // namespace one_counter

// [Slide 77] A static Method Has No this
// Inside RoboTaxi:
// static std::size_t count() noexcept {  // in RoboTaxi: an error
//   return sensors_.size();
// }
// error: invalid use of member 'RoboTaxi::sensors_' in static member function

std::vector<Slide> slides() {
  return {
      {75, "Two RoboTaxis, the Same Id", same_ids::run},
      {76, "One Counter for the Class", one_counter::run},
  };
}

}  // namespace static_members
