/**
 * @file exercises.cpp
 * @brief L8: one answer to each exercise, runnable. Try the exercise first.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. The answers are explained in
 * L8_ENPM702_FALL2026_v1.0_answers.pdf. Exercise 4's bug is in
 * ../../undefined/src/undefined.cpp, built with the sanitizers.
 */
#include <iostream>
#include <stdexcept>

#include "sections.hpp"

namespace exercises {

// [Slide 32] Exercise 1: A New Speed Limit. One answer: a limit that is not
// greater than 0 throws; a target speed above the new limit is lowered to it.
namespace speed_limit {
class CruiseControl {
 public:
  explicit CruiseControl(double speed_limit) : speed_limit_{speed_limit} {
    if (speed_limit_ <= 0.0) {
      throw std::invalid_argument{"speed limit must be greater than 0"};
    }
  }
  [[nodiscard]] double get_speed_limit() const noexcept { return speed_limit_; }
  [[nodiscard]] double get_target_speed() const noexcept { return target_speed_; }
  void set_target_speed(double target_speed) {
    if (target_speed < 0.0 || target_speed > speed_limit_) {
      throw std::invalid_argument{
          "target speed must be from 0 to the speed limit"};
    }
    target_speed_ = target_speed;
  }

  // Not const: it changes speed_limit_. Not noexcept: it can throw.
  void set_speed_limit(double speed_limit) {
    if (speed_limit <= 0.0) {
      throw std::invalid_argument{"speed limit must be greater than 0"};
    }
    speed_limit_ = speed_limit;
    if (target_speed_ > speed_limit_) {
      target_speed_ = speed_limit_;  // slow down to the new limit
    }
  }

 private:
  double speed_limit_;
  double target_speed_{0.0};
};

void run() {
  CruiseControl ego_cruise{13.4};
  ego_cruise.set_target_speed(12.0);
  ego_cruise.set_speed_limit(8.9);  // the school zone
  std::cout << ego_cruise.get_speed_limit() << ' '
            << ego_cruise.get_target_speed() << '\n';  // 8.9 8.9
  ego_cruise.set_speed_limit(13.4);  // out of the zone: the target stays 8.9
  std::cout << ego_cruise.get_speed_limit() << ' '
            << ego_cruise.get_target_speed() << '\n';  // 13.4 8.9
  try {
    ego_cruise.set_speed_limit(-1.0);
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << ego_cruise.get_speed_limit() << '\n';  // 13.4: unchanged
}
}  // namespace speed_limit

// [Slide 42] Exercise 2: The Following Gap. The class as given:
//   explicit FollowingGap(double speed)
//       : time_{2.0}, distance_{time_ * speed} {}
//   ...
//   double distance_;  // m
//   double time_;      // s
// g++ warns (even at -O0):
//   warning: 'FollowingGap::time_' will be initialized after [-Wreorder]
//   warning: member 'FollowingGap::time_' is used uninitialized [-Wuninitialized]
// distance_ is created first and reads time_ before it has a value: undefined
// behavior. Fixed by the first way, changing line 4:
namespace following_gap {
class FollowingGap {  // the distance kept to the car ahead
 public:
  explicit FollowingGap(double speed)
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

std::vector<Slide> slides() {
  return {
      {32, "Exercise 1: A New Speed Limit", speed_limit::run},
      {42, "Exercise 2: The Following Gap", following_gap::run},
  };
}

}  // namespace exercises
