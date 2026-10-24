/**
 * @file const_and_this.cpp
 * @brief L8 Sections 6 and 7, const Objects and The this Pointer: the code of
 *        every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. Code that does not compile is commented
 * out where its slide shows it, with what the compiler says.
 */
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include "cruise_control.hpp"
#include "sections.hpp"

namespace const_and_this {

// [Slide 52] Calling a Mutator
namespace const_object {
void run() {
  const CruiseControl city_cruise{13.4, 12.0};
  std::cout << city_cruise.get_target_speed() << '\n';  // 12
  // city_cruise.set_target_speed(10.0);  // does not compile:
  // error: passing 'const CruiseControl' as 'this' argument discards qualifiers
}
}  // namespace const_object

// [Slide 53] Through a const Reference
// class CruiseControl {
//  public:
//   double get_target_speed() { return target_speed_; }  // const left out
//   ...
// };
// void show(const CruiseControl& cruise) {
//   std::cout << cruise.get_target_speed() << '\n';  // does not compile
// }
// error: passing 'const CruiseControl' as 'this' argument discards qualifiers

// [Slide 56] One Method, Many Objects
// A copy of the class whose setter prints this.
namespace one_method {
class CruiseControl {
 public:
  explicit CruiseControl(double speed_limit) : speed_limit_{speed_limit} {}
  void set_target_speed(double target_speed) {
    std::cout << "this:        " << this << '\n';
    if (target_speed < 0.0 || target_speed > speed_limit_) {
      throw std::invalid_argument{
          "target speed must be from 0 to the speed limit"};
    }
    target_speed_ = target_speed;  // means this->target_speed_
  }

 private:
  double speed_limit_;
  double target_speed_{0.0};
};

void run() {
  CruiseControl ego_cruise{13.4};
  std::cout << "&ego_cruise: " << &ego_cruise << '\n';
  ego_cruise.set_target_speed(12.0);  // the two addresses match
}
}  // namespace one_method

// [Slides 57 and 58] The class checks the type of this in both kinds of
// method, and has increase(), which returns *this.
namespace this_pointer {
class CruiseControl {
 public:
  explicit CruiseControl(double speed_limit) : speed_limit_{speed_limit} {}
  CruiseControl(double speed_limit, double target_speed)
      : CruiseControl{speed_limit} {
    set_target_speed(target_speed);
  }

  void set_target_speed(double target_speed) {
    // [Slide 57] The Type of this: in a method that is not const
    static_assert(std::is_same_v<decltype(this), CruiseControl*>);
    if (target_speed < 0.0 || target_speed > speed_limit_) {
      throw std::invalid_argument{
          "target speed must be from 0 to the speed limit"};
    }
    target_speed_ = target_speed;  // means this->target_speed_
  }

  [[nodiscard]] double get_target_speed() const noexcept {
    // [Slide 57] The Type of this: in a const method
    static_assert(std::is_same_v<decltype(this), const CruiseControl*>);
    return target_speed_;
  }

  // [Slide 58] Returning *this
  CruiseControl& increase(double step) {
    set_target_speed(target_speed_ + step);  // the check runs here
    return *this;                            // the object itself
  }

  // [Slide 57] this cannot be assigned:
  // void reseat() { this = nullptr; }
  // error: lvalue required as left operand of assignment

 private:
  double speed_limit_;
  double target_speed_{0.0};
};

void run_return_this() {
  CruiseControl ego_cruise{13.4, 12.0};
  ego_cruise.increase(0.5).increase(0.5);
  std::cout << ego_cruise.get_target_speed() << '\n';  // 13
}
}  // namespace this_pointer

std::vector<Slide> slides() {
  return {
      {52, "Calling a Mutator", const_object::run},
      {56, "One Method, Many Objects", one_method::run},
      {58, "Returning *this", this_pointer::run_return_this},
  };
}

}  // namespace const_and_this
