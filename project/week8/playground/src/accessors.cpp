/**
 * @file accessors.cpp
 * @brief L8, Accessors and Mutators: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The appendix frames of this section are listed
 * in appendix_slides(), which week8_appendix runs. Code that does not compile
 * is commented out where its slide shows it: uncomment it, build, and you get
 * the slide's error.
 *
 * [Slide 52] ends the program on purpose, so a full run skips it. Run it on
 * its own: 702run week8_playground 53
 *
 * The Live Coding slide (Step 4) has no code here: we write that code together
 * in ../../live/src/main.cpp (week8_live). The discussion slides of this
 * section are in discussions.cpp.
 */
#include <iostream>
#include <stdexcept>

#include "sections.hpp"

namespace accessors {

// The class of this section: the setter checks the maximum speed, the getter
// is const ([Slide 44]), [[nodiscard]] ([Slide 45]) and noexcept ([Slide 46]).
namespace with_check {
class Vehicle {
 public:
  void set_speed(double speed);

  // [Slide 44] Accessors, [Slide 45] [[nodiscard]] and [Slide 46] noexcept
  [[nodiscard]] double get_speed() const noexcept {
    return speed_;
  }

  // [Appendix vi] The Compiler Checks const
  // double get_speed_bad() const {
  //   speed_ = 0.0;  // does not compile
  //   return speed_;
  // }
  // error: assignment of member 'accessors::with_check::Vehicle::speed_' in read-only object
  // (the slide's code has no namespaces, so the slide's error names Vehicle only)

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

// [Slide 49] Mutators: the mutator that checks
void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{
        "speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}
}  // namespace with_check

using with_check::Vehicle;

// [Slide 44] Accessors: get_speed() reads the state and changes nothing.
namespace const_accessor {
void run() {
  const Vehicle car_a;  // a const object can call a const method
  std::cout << car_a.get_speed() << '\n';  // 0
}
}  // namespace const_accessor

// [Slide 45] [[nodiscard]]
namespace nodiscard {
void run() {
  Vehicle car_a;
  car_a.set_speed(12.0);
  // car_a.get_speed();  // the result is thrown away: a warning
  // warning: ignoring return value of
  //          'double accessors::with_check::Vehicle::get_speed() const',
  //          declared with attribute 'nodiscard' [-Wunused-result]
  // (the slide's code has no namespaces, so the slide's warning names Vehicle only)
  std::cout << car_a.get_speed() << '\n';  // 12: the result is used
}
}  // namespace nodiscard

// [Slide 46] noexcept: the promise, checked by code. noexcept( ) does not call
// get_speed(); it asks the compiler whether the call can throw. It is true
// here, false without the noexcept. For get_speed(), g++ 13.3 at -O2 writes the
// same machine code with or without it.
namespace promise {
void run() {
  Vehicle car_a;
  std::cout << std::boolalpha << noexcept(car_a.get_speed()) << '\n';  // true
}
}  // namespace promise

// [Slide 47] Where noexcept Goes: code that depends on the promise. Remove the
// noexcept of get_speed() and the static_assert stops compiling. set_speed()
// throws for a bad speed, so it is not noexcept.
namespace where_noexcept {
void run() {
  Vehicle car_a;
  static_assert(noexcept(car_a.get_speed()));
  std::cout << std::boolalpha
            << "get_speed: " << noexcept(car_a.get_speed()) << '\n'     // get_speed: true
            << "set_speed: " << noexcept(car_a.set_speed(12.0)) << '\n';  // set_speed: false
}
}  // namespace where_noexcept

// [Slide 49] Mutators and [Appendix vii] A Broken Rule Is Caught
namespace rule_caught {
void run() {
  Vehicle car_a;
  car_a.set_speed(12.0);
  try {
    car_a.set_speed(20.0);  // above the maximum, 13.4
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << car_a.get_speed() << '\n';  // 12
}
}  // namespace rule_caught

// [Slide 52] Not const, Not noexcept: the setter of [Slide 49], marked
// noexcept. It still throws, so the promise is false: called with 20.0, the
// program stops, and the catch of the Step 4 block never runs. g++ warns at
// the throw:
//   warning: 'throw' will always call 'terminate' [-Wterminate]
// The two pragma lines turn that warning off for this one function only, so
// the programs build with no warning. Run it on its own (a full run skips it):
//   702run week8_playground 53
// It prints, then stops with exit status 134:
//   terminate called after throwing an instance of 'std::invalid_argument'
//     what():  speed must be from 0 to the maximum speed
namespace false_promise {
class Vehicle {
 public:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
  void set_speed(double speed) noexcept {  // a false promise
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{
          "speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }
#pragma GCC diagnostic pop

  [[nodiscard]] double get_speed() const noexcept {
    return speed_;
  }

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

void run() {
  Vehicle car_a;
  car_a.set_speed(12.0);
  try {
    car_a.set_speed(20.0);  // above the maximum: std::terminate
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';  // never runs
  }
  std::cout << car_a.get_speed() << '\n';  // never runs
}
}  // namespace false_promise

// [Slide 53] A Getter and Setter That Add Nothing
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

void run_const_checked() { print_commented_out("playground/src/accessors.cpp"); }

std::vector<Slide> slides() {
  return {
      {44, "Accessors", const_accessor::run},
      {45, "[[nodiscard]]", nodiscard::run},
      {46, "noexcept", promise::run},
      {47, "Where noexcept Goes", where_noexcept::run},
      {49, "Mutators", rule_caught::run},
      {52, "Not const, Not noexcept", false_promise::run, true},
      {53, "A Getter and Setter That Add Nothing", trivial_pair::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
      {6, "The Compiler Checks const", run_const_checked},
      {7, "A Broken Rule Is Caught", rule_caught::run},
  };
}

}  // namespace accessors
