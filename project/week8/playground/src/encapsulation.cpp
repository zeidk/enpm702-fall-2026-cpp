/**
 * @file encapsulation.cpp
 * @brief L8 Section 3, Encapsulation: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile or link is commented
 * out where its slide shows it: uncomment it, build, and you get the slide's
 * error.
 */
#include <iostream>

#include "sections.hpp"

namespace encapsulation {

// [Slide 27] The Source File
// The class of [Slide 23], and its methods defined outside it with
// Vehicle::, as vehicle.hpp and vehicle.cpp would hold them.
namespace source_file {
class Vehicle {
 public:  // any code may call these
  void set_speed(double speed);
  double get_speed() const;

 private:  // only Vehicle's own methods may use these
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

void Vehicle::set_speed(double speed) {
  speed_ = speed;
}

double Vehicle::get_speed() const {
  return speed_;
}

// [Slide 28] Without Vehicle::
// void set_speed(double speed) {
//   speed_ = speed;
// }
// error: 'speed_' was not declared in this scope; did you mean 'speed'?

void run() {
  Vehicle downtown_car;
  downtown_car.set_speed(12.0);
  std::cout << downtown_car.get_speed() << '\n';  // 12
}
}  // namespace source_file

// [Slide 29] Changing the Inside
// The data members hold km/h; the methods still take and give m/s. The calls
// are those of [Slide 11], unchanged.
namespace changing_inside {
class Vehicle {
 public:
  void set_speed(double speed) {
    speed_kmh_ = speed * 3.6;
  }
  double get_speed() const {
    return speed_kmh_ / 3.6;
  }

 private:
  double max_speed_kmh_{48.24};  // 13.4 * 3.6
  double speed_kmh_{0.0};
};

void run() {
  Vehicle downtown_car;
  Vehicle airport_car;
  downtown_car.set_speed(12.0);
  airport_car.set_speed(10.0);
  std::cout << downtown_car.get_speed() << ' '
            << airport_car.get_speed() << '\n';  // 12 10
}
}  // namespace changing_inside

std::vector<Slide> slides() {
  return {
      {27, "The Source File", source_file::run},
      {29, "Changing the Inside", changing_inside::run},
  };
}

}  // namespace encapsulation
