/**
 * @file accessors.cpp
 * @brief L8 Section 4, Accessors and Mutators: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile, or that ends the
 * program, is commented out where its slide shows it: uncomment it, build, and
 * you get the slide's error or output.
 */
#include <iostream>
#include <stdexcept>

#include "sections.hpp"

namespace accessors {

// The class of this section: the setter checks the maximum speed, the getter
// is [[nodiscard]], const and noexcept.
namespace with_check {
class Vehicle {
 public:
  void set_speed(double speed);

  // [Slide 33] [[nodiscard]] and noexcept
  [[nodiscard]] double get_speed() const noexcept {
    return speed_;
  }

  // [Slide 32] The Compiler Checks const
  // double get_speed_bad() const {
  //   speed_ = 0.0;  // does not compile
  //   return speed_;
  // }
  // error: assignment of member 'Vehicle::speed_' in read-only object

  // [Slide 36] Not const, Not noexcept: the same setter, marked noexcept.
  // void set_speed_noexcept(double speed) noexcept {
  //   if (speed < 0.0 || speed > max_speed_) {
  //     throw std::invalid_argument{
  //         "speed must be from 0 to the maximum speed"};
  //   }
  //   speed_ = speed;
  // }
  // warning: 'throw' will always call 'terminate' [-Wterminate]
  // Called with 20.0, the program stops (exit status 134):
  // terminate called after throwing an instance of 'std::invalid_argument'
  //   what():  speed must be from 0 to the maximum speed

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

// [Slide 34] Mutators
void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{
        "speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}
}  // namespace with_check

// [Slide 33] [[nodiscard]] and noexcept
// downtown_car.get_speed();  // the result is thrown away: a warning
// warning: ignoring return value of 'double Vehicle::get_speed() const',
//          declared with attribute 'nodiscard' [-Wunused-result]

// [Slide 35] A Broken Rule Is Caught
namespace rule_caught {
void run() {
  with_check::Vehicle downtown_car;
  downtown_car.set_speed(12.0);
  try {
    downtown_car.set_speed(20.0);  // above the maximum, 13.4
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << downtown_car.get_speed() << '\n';  // 12
}
}  // namespace rule_caught

// [Slide 37] A Getter and Setter That Add Nothing
namespace trivial_pair {
class Position {  // no rule about x
 public:
  double get_x() const { return x_; }
  void set_x(double x) { x_ = x; }  // takes any value

 private:
  double x_{0.0};
};

void run() {
  Position position;
  position.set_x(-1000.0);  // nothing stops it
  std::cout << position.get_x() << '\n';  // -1000
}
}  // namespace trivial_pair

std::vector<Slide> slides() {
  return {
      {35, "A Broken Rule Is Caught", rule_caught::run},
      {37, "A Getter and Setter That Add Nothing", trivial_pair::run},
  };
}

}  // namespace accessors
