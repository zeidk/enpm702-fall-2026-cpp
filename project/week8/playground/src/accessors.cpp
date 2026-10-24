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

// The class of this section: the setter checks the speed limit, the getter is
// [[nodiscard]], const and noexcept.
namespace with_check {
class CruiseControl {
 public:
  void set_target_speed(double target_speed);

  // [Slide 27] [[nodiscard]] and noexcept
  [[nodiscard]] double get_target_speed() const noexcept {
    return target_speed_;
  }

  // [Slide 26] The Compiler Checks const
  // double get_target_speed_bad() const {
  //   target_speed_ = 0.0;  // does not compile
  //   return target_speed_;
  // }
  // error: assignment of member 'CruiseControl::target_speed_' in read-only object

  // [Slide 30] Not const, Not noexcept: the same setter, marked noexcept.
  // void set_target_speed_noexcept(double target_speed) noexcept {
  //   if (target_speed < 0.0 || target_speed > speed_limit_) {
  //     throw std::invalid_argument{
  //         "target speed must be from 0 to the speed limit"};
  //   }
  //   target_speed_ = target_speed;
  // }
  // warning: 'throw' will always call 'terminate' [-Wterminate]
  // Called with 20.0, the program stops (exit status 134):
  // terminate called after throwing an instance of 'std::invalid_argument'
  //   what():  target speed must be from 0 to the speed limit

 private:
  double speed_limit_{13.4};  // m/s
  double target_speed_{0.0};  // m/s
};

// [Slide 28] Mutators
void CruiseControl::set_target_speed(double target_speed) {
  if (target_speed < 0.0 || target_speed > speed_limit_) {
    throw std::invalid_argument{
        "target speed must be from 0 to the speed limit"};
  }
  target_speed_ = target_speed;
}
}  // namespace with_check

// [Slide 27] [[nodiscard]] and noexcept
// ego_cruise.get_target_speed();  // the result is thrown away: a warning
// warning: ignoring return value of
//          'double CruiseControl::get_target_speed() const',
//          declared with attribute 'nodiscard' [-Wunused-result]

// [Slide 29] A Broken Rule Is Caught
namespace rule_caught {
void run() {
  with_check::CruiseControl ego_cruise;
  ego_cruise.set_target_speed(12.0);
  try {
    ego_cruise.set_target_speed(20.0);  // above the limit, 13.4
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << ego_cruise.get_target_speed() << '\n';  // 12
}
}  // namespace rule_caught

// [Slide 31] A Getter and Setter That Add Nothing
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
      {29, "A Broken Rule Is Caught", rule_caught::run},
      {31, "A Getter and Setter That Add Nothing", trivial_pair::run},
  };
}

}  // namespace accessors
