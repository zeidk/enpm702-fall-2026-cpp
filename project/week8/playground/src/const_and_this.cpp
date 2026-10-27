/**
 * @file const_and_this.cpp
 * @brief L8, The this Pointer and the appendix's const Objects: the code of
 *        every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. const Objects is a part of the appendix: its
 * frames are listed in appendix_slides(), which week8_appendix runs. Code that
 * does not compile is commented out where its slide shows it, with what the
 * compiler says.
 */
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "position.hpp"
#include "sections.hpp"
#include "vehicle.hpp"

namespace const_and_this {

// [Appendix vi] What a const Object Cannot Do. Each line that does
// not compile is commented out, with what g++ 13.3 says (the slide's code has no
// namespaces, so there the errors name Vehicle only). The Vehicle here is the
// class of The Class So Far, cut to what this slide uses: it owns nothing by
// hand, so it can be copied. The Vehicle of ../../common/include/vehicle.hpp
// also has sensors_, so it cannot be copied.
namespace const_cannot {
class Vehicle {
 public:
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  void set_speed(double speed) {
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }

 private:
  double max_speed_;     // m/s, greater than 0
  double speed_{0.0};    // m/s, from 0 to max_speed_
  std::string color_;    // from the constructor
  double mileage_{0.0};  // m driven
};

void run() {
  const Position stop{2.0, 3.0};
  const Vehicle car_a{13.4};
  Vehicle car_b{13.4};
  // stop.x = 5.0;
  // error: assignment of member 'Position::x' in read-only object
  // car_a.set_speed(12.0);
  // error: passing 'const const_and_this::const_cannot::Vehicle' as 'this' argument discards qualifiers [-fpermissive]
  // car_a = car_b;
  // error: passing 'const const_and_this::const_cannot::Vehicle' as 'this' argument discards qualifiers [-fpermissive]
  // Vehicle& ref{car_a};
  // error: binding reference of type 'const_and_this::const_cannot::Vehicle&' to 'const const_and_this::const_cannot::Vehicle' discards qualifiers
  // Vehicle* ptr{&car_a};
  // error: invalid conversion from 'const const_and_this::const_cannot::Vehicle*' to 'const_and_this::const_cannot::Vehicle*' [-fpermissive]
  std::cout << stop.x << ' ' << car_b.get_speed() << '\n';  // 2 0
  Vehicle copy{car_a};  // a copy that is not const
  copy.set_speed(5.0);  // compiles: the copy can change
  std::cout << copy.get_speed() << ' ' << car_a.get_speed() << '\n';  // 5 0
}
}  // namespace const_cannot

// [Appendix vii] Calling a Mutator
namespace const_object {
void run() {
  const Vehicle logged_car{13.4, 12.0};
  std::cout << logged_car.get_speed() << '\n';  // 12
  // logged_car.set_speed(10.0);  // does not compile:
  // error: passing 'const Vehicle' as 'this' argument discards qualifiers [-fpermissive]
}
}  // namespace const_object

// [Appendix viii] Through a const Reference
// namespace const_ref {
// class Vehicle {
//  public:
//   double get_speed() { return speed_; }  // const left out
//   // ...
//  private:
//   double max_speed_;     // m/s, greater than 0
//   double speed_{0.0};    // m/s
//   std::string color_;    // from the constructor
//   double mileage_{0.0};  // m driven
// };
// void show(const Vehicle& vehicle) {
//   std::cout << vehicle.get_speed() << '\n';  // does not compile
// }
// }  // namespace const_ref
// error: passing 'const const_and_this::const_ref::Vehicle' as 'this' argument discards qualifiers [-fpermissive]
// (the slide's code has no namespaces, so the slide's error names Vehicle only)

// [Slide 82] One Method, Many Objects
// A copy of the class whose setter prints this.
namespace one_method {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  void set_speed(double speed) {
    std::cout << "this:   " << this << '\n';
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
  Vehicle car_a{13.4};
  std::cout << "&car_a: " << &car_a << '\n';
  car_a.set_speed(12.0);  // the two addresses match
}
}  // namespace one_method

// [Slide 83] The Type of this and [Slide 84] Returning *this: the class
// checks the type of this in both kinds of method, and has speed_up(), which
// returns *this.
namespace this_pointer {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
    set_speed(speed);
  }

  void set_speed(double speed) {
    // [Slide 83] The Type of this: in a method that is not const
    static_assert(std::is_same_v<decltype(this), Vehicle*>);
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{
          "speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;  // means this->speed_
  }

  [[nodiscard]] double get_speed() const noexcept {
    // [Slide 83] The Type of this: in a const method
    static_assert(std::is_same_v<decltype(this), const Vehicle*>);
    // speed_ = 1.0;  // the const is on the Vehicle, not on the pointer:
    // error: assignment of member 'const_and_this::this_pointer::Vehicle::speed_' in read-only object
    // (the slide's code has no namespaces, so the slide's error names Vehicle only)
    return speed_;
  }

  // [Slide 84] Returning *this
  Vehicle& speed_up(double step) {
    set_speed(speed_ + step);  // the check runs here
    return *this;              // the object itself
  }

  // [Slide 83] The Type of this: this cannot be assigned:
  // void reseat() { this = nullptr; }
  // error: lvalue required as left operand of assignment

 private:
  double max_speed_;
  double speed_{0.0};
};

// [Slide 83] The Type of this: the two static_asserts in the class are
// checked when this file compiles. If either type were wrong, the build would
// stop, so the run only reports what they check.
void run_type_of_this() {
  std::cout << "set_speed: this is a Vehicle*\n"
               "get_speed() const: this is a const Vehicle*\n";
}

// [Slide 84] Returning *this
void run_return_this() {
  Vehicle car_a{13.4, 12.0};
  car_a.speed_up(0.5).speed_up(0.5);
  std::cout << car_a.get_speed() << '\n';  // 13
}
}  // namespace this_pointer

void run_const_ref() { print_commented_out("playground/src/const_and_this.cpp"); }

std::vector<Slide> slides() {
  return {
      {82, "One Method, Many Objects", one_method::run},
      {83, "The Type of this", this_pointer::run_type_of_this},
      {84, "Returning *this", this_pointer::run_return_this},
  };
}

std::vector<Slide> appendix_slides() {
  return {
      {6, "What a const Object Cannot Do", const_cannot::run},
      {7, "Calling a Mutator", const_object::run},
      {8, "Through a const Reference", run_const_ref},
  };
}

}  // namespace const_and_this
