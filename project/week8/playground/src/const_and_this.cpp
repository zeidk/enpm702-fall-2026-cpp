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

#include "sections.hpp"
#include "vehicle.hpp"

namespace const_and_this {

// [Slide 54] Calling a Mutator
namespace const_object {
void run() {
  const Vehicle logged_car{13.4, 12.0};
  std::cout << logged_car.get_speed() << '\n';  // 12
  // logged_car.set_speed(10.0);  // does not compile:
  // error: passing 'const Vehicle' as 'this' argument discards qualifiers
}
}  // namespace const_object

// [Slide 55] Through a const Reference
// class Vehicle {
//  public:
//   double get_speed() { return speed_; }  // const left out
//   ...
// };
// void show(const Vehicle& vehicle) {
//   std::cout << vehicle.get_speed() << '\n';  // does not compile
// }
// error: passing 'const Vehicle' as 'this' argument discards qualifiers

// [Slide 58] One Method, Many Objects
// A copy of the class whose setter prints this.
namespace one_method {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  void set_speed(double speed) {
    std::cout << "this:          " << this << '\n';
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{
          "speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;  // means this->speed_
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

void run() {
  Vehicle downtown_car{13.4};
  std::cout << "&downtown_car: " << &downtown_car << '\n';
  downtown_car.set_speed(12.0);  // the two addresses match
}
}  // namespace one_method

// [Slides 59 and 60] The class checks the type of this in both kinds of
// method, and has speed_up(), which returns *this.
namespace this_pointer {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
    set_speed(speed);
  }

  void set_speed(double speed) {
    // [Slide 59] The Type of this: in a method that is not const
    static_assert(std::is_same_v<decltype(this), Vehicle*>);
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{
          "speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;  // means this->speed_
  }

  [[nodiscard]] double get_speed() const noexcept {
    // [Slide 59] The Type of this: in a const method
    static_assert(std::is_same_v<decltype(this), const Vehicle*>);
    return speed_;
  }

  // [Slide 60] Returning *this
  Vehicle& speed_up(double step) {
    set_speed(speed_ + step);  // the check runs here
    return *this;              // the object itself
  }

  // [Slide 59] this cannot be assigned:
  // void reseat() { this = nullptr; }
  // error: lvalue required as left operand of assignment

 private:
  double max_speed_;
  double speed_{0.0};
};

void run_return_this() {
  Vehicle downtown_car{13.4, 12.0};
  downtown_car.speed_up(0.5).speed_up(0.5);
  std::cout << downtown_car.get_speed() << '\n';  // 13
}
}  // namespace this_pointer

std::vector<Slide> slides() {
  return {
      {54, "Calling a Mutator", const_object::run},
      {58, "One Method, Many Objects", one_method::run},
      {60, "Returning *this", this_pointer::run_return_this},
  };
}

}  // namespace const_and_this
