/**
 * @file encapsulation.cpp
 * @brief L8, Encapsulation: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The appendix frames of this section (Header and
 * Source) are listed in appendix_slides(), which week8_appendix runs. Code that
 * does not compile or link is commented out where its slide shows it:
 * uncomment it, build, and you get the slide's error.
 */
#include <iostream>
#include <string>

#include "sections.hpp"

namespace encapsulation {

// [Slide 34] Access Specifiers: the class of [Slide 34], with its methods
// defined outside it with Vehicle::, as vehicle.hpp and vehicle.cpp would hold
// them.
namespace source_file {
class Vehicle {
 public:  // any code may call these
  void set_speed(double speed);
  double get_speed() const;

 private:  // only Vehicle's own methods may use these
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
  std::string color_{"white"};
  double mileage_{0.0};     // m driven
};

void Vehicle::set_speed(double speed) {
  speed_ = speed;
}

double Vehicle::get_speed() const {
  return speed_;
}

void run() {
  Vehicle car_a;
  car_a.set_speed(12.0);  // a public method: any code may call it
  std::cout << car_a.get_speed() << '\n';  // 12
}

}  // namespace source_file

// [Slide 40] Changing the Inside
// The data members hold km/h; the methods still take and give m/s. The speed
// calls are those of Live Coding Step 1 ([Slide 18]), unchanged.
namespace changing_inside {
class Vehicle {
 public:
  void set_speed(double speed);
  double get_speed() const;

 private:
  double max_speed_kmh_{48.24};  // 13.4 * 3.6
  double speed_kmh_{0.0};
  std::string color_{"white"};
  double mileage_{0.0};  // m driven
};

void Vehicle::set_speed(double speed) {
  speed_kmh_ = speed * 3.6;
}

double Vehicle::get_speed() const {
  return speed_kmh_ / 3.6;
}

void run() {
  Vehicle car_a;
  Vehicle car_b;
  car_a.set_speed(12.0);
  car_b.set_speed(10.0);
  std::cout << car_a.get_speed() << ' '
            << car_b.get_speed() << '\n';  // 12 10
}
}  // namespace changing_inside

// [Slide 41] A Body in the Class: get_speed() is defined in the class,
// set_speed() outside it, as vehicle.cpp would hold it. The course's own code
// defines every method in vehicle.cpp and only the constructors in the class.
namespace short_methods {
class Vehicle {  // in vehicle.hpp
 public:
  double get_speed() const { return speed_; }  // body here
  void set_speed(double speed);  // body in vehicle.cpp

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
  std::string color_{"white"};
  double mileage_{0.0};     // m driven
};

void Vehicle::set_speed(double speed) {
  speed_ = speed;
}

void run() {
  Vehicle car_a;
  car_a.set_speed(12.0);
  std::cout << car_a.get_speed() << '\n';  // 12
}
}  // namespace short_methods

std::vector<Slide> slides() {
  return {
      {34, "Access Specifiers", source_file::run},
      {40, "Changing the Inside", changing_inside::run},
      {41, "A Body in the Class", short_methods::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
  };
}

}  // namespace encapsulation
