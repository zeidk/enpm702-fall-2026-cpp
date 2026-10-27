/**
 * @file discussions.cpp
 * @brief L8: the code behind the discussion slides, runnable. Discuss first.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The appendix discussion Will It Compile? is
 * listed in appendix_slides(), which week8_appendix runs.
 */
#include <iostream>
#include <stdexcept>
#include <string>

#include "sections.hpp"

namespace discussions {

// [Slide 73] Discussion: Spot the Bug. The class as given, with its line
// numbers on the slide:
//    3  FollowingGap(double speed): time_{2.0}, distance_{time_ * speed} {}
//       ...
//   10  double distance_;  // m
//   11  double time_;      // s: the gap in seconds
// Compiled on its own, as on the slide, g++ warns (even at -O0):
//   warning: 'FollowingGap::time_' will be initialized after [-Wreorder]
//   warning: member 'FollowingGap::time_' is used uninitialized [-Wuninitialized]
// distance_ is created first and reads time_ before it has a value: undefined
// behavior. Fixed by the first way, changing line 3:
namespace following_gap {
class FollowingGap {  // the distance the robotaxi keeps to the car ahead
 public:
  FollowingGap(double speed)
      : distance_{2.0 * speed}, time_{2.0} {}
  [[nodiscard]] double get_distance() const noexcept {
    return distance_;
  }

 private:
  double distance_;  // m
  double time_;      // s: the gap in seconds
};

void run() {
  FollowingGap gap{12.0};
  std::cout << gap.get_distance() << '\n';  // 24
}
}  // namespace following_gap

// [Appendix xviii] Discussion: Will It Compile? The class of [Appendix ix],
// The Class So Far, copied here. It owns nothing by hand, so the compiler writes its copy
// constructor and copy assignment, and lines 7 and 8 compile. The Vehicle of
// ../../common/include/vehicle.hpp also has sensors_, a std::vector of
// std::unique_ptr, so it is move-only: with it, lines 7 and 8 do not compile.
namespace will_it_compile {
class Vehicle {
 public:
  explicit Vehicle(double max_speed, const std::string& color = "white")
      : max_speed_{max_speed}, color_{color} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }
  Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
    set_speed(speed);
  }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  void set_speed(double speed) {
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }
  void add_mileage(double meters) {
    if (meters < 0.0) {
      throw std::invalid_argument{"a trip's distance must be at least 0"};
    }
    mileage_ += meters;
  }

 private:
  double max_speed_;     // m/s, greater than 0
  double speed_{0.0};    // m/s, from 0 to max_speed_
  std::string color_;    // from the constructor
  double mileage_{0.0};  // m driven, never goes down
};

void show(const Vehicle& vehicle) {
  std::cout << vehicle.get_speed() << '\n';
}

void run() {
  const Vehicle logged_car{13.4, 12.0};
  Vehicle car_a{13.4};

  double a{logged_car.get_speed()};  // 1
  // logged_car.set_speed(5.0);      // 2
  show(car_a);                       // 3: prints 0
  // show(13.4);                     // 4
  // Vehicle other_car;              // 5
  // Vehicle fast_car{13.4, 20.0};   // 6
  Vehicle copy_car{logged_car};      // 7
  car_a = logged_car;                // 8
  std::cout << a << ' ' << copy_car.get_speed() << ' '
            << car_a.get_speed() << '\n';  // 12 12 12
  // Uncommented, line by line (the slide's code has no namespaces, so there the
  // errors name Vehicle only):
  // 2: error: passing 'const discussions::will_it_compile::Vehicle' as 'this'
  //    argument discards qualifiers [-fpermissive]
  // 4: error: invalid initialization of reference of type
  //    'const discussions::will_it_compile::Vehicle&' from expression of type 'double'
  // 5: error: no matching function for call to
  //    'discussions::will_it_compile::Vehicle::Vehicle()'
  // 6: compiles; when it runs, nothing catches the exception and the program
  //    stops with exit status 134:
  //    terminate called after throwing an instance of 'std::invalid_argument'
  //      what():  speed must be from 0 to the maximum speed
}
}  // namespace will_it_compile

// [Slide 49] Discussion: Returning a Reference. With double&, code outside
// the class writes through the reference: car_a.get_speed() = 50.0; compiles
// with no warning, no check runs, and the speed is 50, above the maximum of
// 13.4. With const double& get_speed() const instead, the line
//   const double& speed = Vehicle{13.4}.get_speed();
// gives "warning: possibly dangling reference to a temporary
// [-Wdangling-reference]" with -Wextra, and AddressSanitizer reports a
// stack-use-after-scope when speed is read (measured with g++ 13.3).
namespace return_ref {
class Vehicle {
 public:
  // returns a reference to speed_
  double& get_speed() { return speed_; }

 private:
  double max_speed_{13.4};  // m/s
  double speed_{0.0};       // m/s
};

void run() {
  Vehicle car_a;
  car_a.get_speed() = 50.0;  // compiles: no check runs
  std::cout << car_a.get_speed() << '\n';  // 50, above the maximum of 13.4
}
}  // namespace return_ref

std::vector<Slide> slides() {
  return {
      {49, "Discussion: Returning a Reference", return_ref::run},
      {73, "Discussion: Spot the Bug", following_gap::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
      {18, "Discussion: Will It Compile?", will_it_compile::run},
  };
}

}  // namespace discussions
