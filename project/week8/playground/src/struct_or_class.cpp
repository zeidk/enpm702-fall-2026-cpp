/**
 * @file struct_or_class.cpp
 * @brief L8, Struct or Class: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile is commented out
 * where its slide shows it: uncomment it, build, and you get the slide's error.
 *
 * [Slide 25] uses the Vehicle of ../../common/include/vehicle.hpp, whose
 * set_speed() checks the invariant. [Slide 27] uses the Sensor of
 * ../../common/include/sensor.hpp; its constructor, defined in the class there,
 * runs in constructors.cpp ([Slide 72]).
 */
#include <iostream>
#include <stdexcept>

#include "sections.hpp"
#include "sensor.hpp"
#include "vehicle.hpp"

namespace struct_or_class {

// [Slide 25] The Invariant of Vehicle: 0.0 <= speed_ && speed_ <= max_speed_.
// set_speed(-50.0) would break it, so set_speed() throws and the speed keeps
// its old value. The constructor, Vehicle{13.4}, comes in the Constructors
// section; 13.4 m/s is the maximum speed of the examples.
namespace invariant {
void run() {
  Vehicle car_a{13.4};
  try {
    car_a.set_speed(-50.0);  // a negative speed breaks the invariant
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << car_a.get_speed() << '\n';  // 0: unchanged
}
}  // namespace invariant

// [Slide 27] The Sensor Class: the defaults make a lidar that sees 100 m.
namespace sensor_class {
void run() {
  const Sensor lidar{1};
  std::cout << "id " << lidar.get_id() << ", "
            << (lidar.get_type() == SensorType::lidar ? "lidar" : "not a lidar")
            << ", range " << lidar.get_range() << '\n';  // id 1, lidar, range 100
}
}  // namespace sensor_class

// [Slide 28] Default Access
namespace default_access {
struct Position {  // no access specifier
  double x{0.0};
  double y{0.0};
};
class Sensor {     // no access specifier
  double range_{100.0};
};

void run() {
  Position waypoint;
  waypoint.x = 25.0;    // OK: a struct's members are public
  Sensor lidar;
  // lidar.range_ = -5.0;  // does not compile:
  // error: 'double struct_or_class::default_access::Sensor::range_'
  //        is private within this context
  // (the slide's code has no namespaces, so the slide's error names Sensor only)
  (void)lidar;
  std::cout << waypoint.x << '\n';  // 25
}
}  // namespace default_access

// [Slide 29] A Rule Nothing Checks
namespace nothing_checks {
struct VehicleData {
  double max_speed{13.4};  // m/s
  double speed{0.0};       // m/s, must stay from 0 to max_speed
};

void run() {
  VehicleData data;
  data.speed = 20.0;  // compiles: any code can break the rule
  std::cout << data.speed << " > "
            << data.max_speed << '\n';  // 20 > 13.4
}
}  // namespace nothing_checks

std::vector<Slide> slides() {
  return {
      {25, "The Invariant of Vehicle", invariant::run},
      {27, "The Sensor Class", sensor_class::run},
      {28, "Default Access", default_access::run},
      {29, "A Rule Nothing Checks", nothing_checks::run},
  };
}

}  // namespace struct_or_class
