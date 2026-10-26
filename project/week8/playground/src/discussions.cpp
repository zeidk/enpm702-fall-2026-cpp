/**
 * @file discussions.cpp
 * @brief L8: the code behind the discussion slides, runnable. Discuss first.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. The bug of Who Deletes What? is in
 * ../../undefined/src/undefined.cpp, built with the sanitizers.
 */
#include <iostream>
#include <stdexcept>

#include "sections.hpp"

namespace discussions {

// [Slide 38] Discussion: The School Zone. The option the notes pick: a maximum
// that is not greater than 0 throws; a speed above the new maximum is lowered
// to it.
namespace school_zone {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
  }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  void set_speed(double speed) {
    if (speed < 0.0 || speed > max_speed_) {
      throw std::invalid_argument{
          "speed must be from 0 to the maximum speed"};
    }
    speed_ = speed;
  }

  // Not const: it changes max_speed_. Not noexcept: it can throw.
  void set_max_speed(double max_speed) {
    if (max_speed <= 0.0) {
      throw std::invalid_argument{"maximum speed must be greater than 0"};
    }
    max_speed_ = max_speed;
    if (speed_ > max_speed_) {
      speed_ = max_speed_;  // slow down to the new maximum
    }
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

void run() {
  Vehicle downtown_car{13.4};
  downtown_car.set_speed(12.0);
  downtown_car.set_max_speed(8.9);  // the school zone
  std::cout << downtown_car.get_max_speed() << ' '
            << downtown_car.get_speed() << '\n';  // 8.9 8.9
  downtown_car.set_max_speed(13.4);  // out of the zone: the speed stays 8.9
  std::cout << downtown_car.get_max_speed() << ' '
            << downtown_car.get_speed() << '\n';  // 13.4 8.9
  try {
    downtown_car.set_max_speed(-1.0);
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
  std::cout << downtown_car.get_max_speed() << '\n';  // 13.4: unchanged
}
}  // namespace school_zone

// [Slide 48] Discussion: Spot the Bug. The class as given:
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
class FollowingGap {  // the distance the robotaxi keeps to the car ahead
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
      {38, "Discussion: The School Zone", school_zone::run},
      {48, "Discussion: Spot the Bug", following_gap::run},
  };
}

}  // namespace discussions
