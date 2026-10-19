/**
 * @file weak.cpp
 * @brief L7 Section 3, Smart Pointers, std::weak_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 80    # only [Slide 80]
 * @endcode
 *
 * Two robots that work as a pair point at each other; the dashboard shows a
 * robot without owning it. The cycle that leaks is in
 * ../undefined/undefined.cpp, built with the sanitizers, so that
 * LeakSanitizer can report it. A full run skips it.
 */
#include <iostream>
#include <memory>
#include <vector>

#include "fleet.hpp"
#include "sections.hpp"
#include "undefined.hpp"

namespace weak {

// [Slide 78] A Cycle
// The struct and the program are undefined::shared_cycle in
// ../undefined/undefined.cpp. [Slide 79] runs it.

// [Slide 80] Breaking It
namespace breaking_cycle {
struct Robot {
  RobotStatus status;
  std::weak_ptr<Robot> partner;  // watches its partner, does not own it
};

void run() {
  auto a{std::make_shared<Robot>(RobotStatus{1, 82.5, {0.0, 0.0}, false})};
  auto b{std::make_shared<Robot>(RobotStatus{2, 35.0, {4.0, 1.0}, false})};
  a->partner = b;
  b->partner = a;
  std::cout << "a.use_count(): " << a.use_count() << ", b.use_count(): " << b.use_count()
            << '\n';  // 1 1
}  // a and b go out of scope: each count drops to 0, both are deleted
}  // namespace breaking_cycle

// [Slide 81] lock
namespace reading_weak {
struct Dashboard {
  std::weak_ptr<RobotStatus> shown;  // the robot on screen
};

void show(const Dashboard& dashboard) {
  if (std::shared_ptr<RobotStatus> robot{dashboard.shown.lock()}) {
    std::cout << "  robot " << robot->id << ": " << robot->battery_pct << " %\n";
  } else {
    std::cout << "  the robot is gone\n";
  }
}

void run() {
  std::vector<std::shared_ptr<RobotStatus>> fleet;
  fleet.push_back(std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false));
  Dashboard dashboard{fleet[0]};
  show(dashboard);  // robot 3: 14 %
  fleet.clear();    // the only owner lets go: robot 3 is deleted
  show(dashboard);  // the robot is gone
}
}  // namespace reading_weak

// [Slide 82] expired and use_count
namespace expired_use_count {
void run() {
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  std::weak_ptr<RobotStatus> watch{robot};
  std::cout << std::boolalpha << "watch.use_count(): " << watch.use_count()
            << ", watch.expired(): " << watch.expired() << '\n';  // 1 false
  robot.reset();
  std::cout << "watch.use_count(): " << watch.use_count() << ", watch.expired(): "
            << watch.expired() << std::noboolalpha << '\n';  // 0 true
}
}  // namespace expired_use_count

// [Slide 83] Exercise 3: Use Counts
namespace exercise_3 {
void run() {
  // Write the two values for each line before you run it.
  std::cout << std::boolalpha;
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  std::weak_ptr<RobotStatus> watch{robot};
  std::cout << "1. use_count " << robot.use_count() << ", expired " << watch.expired() << '\n';
  auto a{robot};
  std::cout << "2. use_count " << robot.use_count() << ", expired " << watch.expired() << '\n';
  auto b{std::move(a)};
  std::cout << "3. use_count " << robot.use_count() << ", expired " << watch.expired() << '\n';
  std::weak_ptr<RobotStatus> watch_2{b};
  std::cout << "4. use_count " << robot.use_count() << ", expired " << watch.expired() << '\n';
  robot.reset();
  std::cout << "5. use_count " << watch.use_count() << ", expired " << watch.expired() << '\n';
  b.reset();
  std::cout << "6. use_count " << watch.use_count() << ", expired " << watch.expired() << '\n';
  std::cout << std::noboolalpha;
}
}  // namespace exercise_3

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {79, "The Cycle, Measured", undefined::shared_cycle::run, true},
      {80, "Breaking It", breaking_cycle::run},
      {81, "lock", reading_weak::run},
      {82, "expired and use_count", expired_use_count::run},
      {83, "Exercise 3: Use Counts", exercise_3::run},
  };
}

}  // namespace weak
