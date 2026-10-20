/**
 * @file weak.cpp
 * @brief L7 Section 3, Smart Pointers, std::weak_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 79    # only [Slide 79]
 * @endcode
 *
 * The dashboard shows one obstacle and owns none: it watches the Obstacle
 * through a std::weak_ptr and must notice when the tracker and the planner
 * have let it go. The cycle of [Slide 77] leaks on purpose, so it is in
 * ../undefined/undefined.cpp and runs only when asked for by its number.
 */
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"
#include "undefined.hpp"

namespace weak {

// [Slide 74] A Cycle
// The struct and the program are undefined::shared_cycle in
// ../undefined/undefined.cpp. [Slide 77] runs it.

// [Slide 78] Breaking It
namespace breaking_cycle {
struct Towing {
  Obstacle obstacle;
  std::weak_ptr<Towing> other;  // watches, does not own
};

void run() {
  auto a{std::make_shared<Towing>(Obstacle{5, {60.0, 0.0}, 8.0})};  // the tractor
  auto b{std::make_shared<Towing>(Obstacle{6, {54.0, 0.0}, 8.0})};  // the trailer
  a->other = b;  // b's count stays 1
  b->other = a;  // a's count stays 1
  std::cout << "a.use_count(): " << a.use_count() << ", b.use_count(): " << b.use_count()
            << '\n';  // 1 1
}  // a and b go out of scope: each count drops to 0, both are deleted
}  // namespace breaking_cycle

// [Slide 79] lock
namespace reading_weak {
struct Dashboard {
  std::weak_ptr<Obstacle> shown;  // the obstacle on screen
};

void show(const Dashboard& dashboard) {
  if (std::shared_ptr<Obstacle> target{dashboard.shown.lock()}) {
    std::cout << "  obstacle " << target->id << ": " << target->speed << " m/s\n";
  } else {
    std::cout << "  the obstacle is gone\n";
  }
}

void run() {
  std::vector<std::shared_ptr<Obstacle>> tracker;
  std::vector<std::shared_ptr<Obstacle>> hazards;
  tracker.push_back(std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0));
  hazards.push_back(tracker[0]);
  Dashboard dashboard{tracker[0]};
  show(dashboard);   // obstacle 2: 4 m/s
  tracker.clear();   // the track is lost
  hazards.clear();   // the planner lets go: the Obstacle is deleted
  show(dashboard);   // the obstacle is gone
}
}  // namespace reading_weak

// [Slide 80] expired and use_count
namespace expired_use_count {
void run() {
  auto track{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  std::weak_ptr<Obstacle> shown{track};
  std::cout << std::boolalpha << "shown.use_count(): " << shown.use_count()
            << ", shown.expired(): " << shown.expired() << '\n';  // 1 false
  track.reset();  // the only owner lets go
  std::cout << "shown.use_count(): " << shown.use_count() << ", shown.expired(): "
            << shown.expired() << std::noboolalpha << '\n';  // 0 true
}
}  // namespace expired_use_count

// [Slide 81] Exercise 5: Use Counts
namespace exercise_use_counts {
void run() {
  // Write the two values for each line before you run it.
  std::cout << std::boolalpha;
  auto track{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  std::weak_ptr<Obstacle> shown{track};
  std::cout << "1. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  auto a{track};
  std::cout << "2. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  auto b{std::move(a)};
  std::cout << "3. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  std::weak_ptr<Obstacle> shown_2{b};
  std::cout << "4. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  track.reset();
  std::cout << "5. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  b.reset();
  std::cout << "6. use_count " << shown.use_count() << ", expired " << shown.expired() << '\n';
  std::cout << std::noboolalpha;
}
}  // namespace exercise_use_counts

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {77, "The Cycle, Measured", undefined::shared_cycle::run, true},
      {78, "Breaking It", breaking_cycle::run},
      {79, "lock", reading_weak::run},
      {80, "expired and use_count", expired_use_count::run},
      {81, "Exercise 5: Use Counts", exercise_use_counts::run},
  };
}

}  // namespace weak
