/**
 * @file shared.cpp
 * @brief L7 Section 3, Smart Pointers, std::shared_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 64    # only [Slide 64]
 * @endcode
 *
 * The cyclist, id 2, is near the planner's path: the tracker and the planner's
 * hazard list both own its Obstacle through std::shared_ptr. Code that does
 * not compile is commented out where its slide shows it: uncomment it, build,
 * and you get the slide's error.
 */
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"

namespace shared {

using Hazards = std::vector<std::shared_ptr<Obstacle>>;

// [Slide 61] A Hazard in Two Lists
// The slide states the problem: hazards holds tracker[1].get(), and erasing
// tracker[1] deletes the Obstacle, so hazards[0] dangles. A read through it is
// undefined behavior; [Appendix iii] runs one with the sanitizers.

// [Slide 62] Creating One
namespace creating {
void run() {
  std::vector<std::shared_ptr<Obstacle>> tracker;
  std::vector<std::shared_ptr<Obstacle>> hazards;
  auto hazard{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  tracker.push_back(hazard);  // a copy: one more owner
  hazards.push_back(hazard);  // a copy: one more owner
  std::cout << "hazard.use_count(): " << hazard.use_count() << '\n';  // 3
  std::cout << std::boolalpha << "tracker[0] == hazards[0]: " << (tracker[0] == hazards[0])
            << std::noboolalpha << '\n';  // true: one Obstacle object
}
}  // namespace creating

// [Slide 63] The Control Block
namespace control_block {
void run() {
  auto track{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  std::shared_ptr<Obstacle> hazard{track};
  std::cout << "sizeof(track): " << sizeof(track) << ", sizeof(Obstacle): " << sizeof(Obstacle)
            << '\n';                                                   // 16 48
  std::cout << "track.use_count(): " << track.use_count() << '\n';     // 2
}
}  // namespace control_block

// [Slide 64] use_count
namespace use_count_steps {
void run() {
  auto track{std::make_shared<Obstacle>(2)};
  std::cout << "created: " << track.use_count() << '\n';                // 1
  std::shared_ptr<Obstacle> hazard{track};
  std::cout << "copied: " << track.use_count() << '\n';                 // 2
  std::shared_ptr<Obstacle> moved{std::move(hazard)};
  std::cout << std::boolalpha << "moved: " << track.use_count()
            << ", hazard == nullptr: " << (hazard == nullptr) << std::noboolalpha
            << '\n';                                                     // 2 true
  moved.reset();
  std::cout << "reset: " << track.use_count() << '\n';                  // 1
}
}  // namespace use_count_steps

// [Slide 65] Three Ways to Let Go
// Three separate runs, each from step 2; start() makes steps 1 and 2 and prints
// their counts once, in the first run. [Slides 66 to 70] show each step.
namespace let_go {
std::shared_ptr<Obstacle> start(bool show) {
  auto track{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  {
    std::shared_ptr<Obstacle> hazard{track};  // 1
    if (show) {
      std::cout << "1: " << track.use_count() << '\n';  // 2
    }
  }  // 2: hazard's scope ends
  if (show) {
    std::cout << "2: " << track.use_count() << '\n';  // 1
  }
  return track;
}

void run() {
  {
    auto track{start(true)};
    track = std::make_shared<Obstacle>(4);  // 3a: the cyclist's Obstacle is deleted
    std::cout << "3a: track->id: " << track->id << ", use_count: " << track.use_count()
              << '\n';                                                     // 4 1
  }
  {
    auto track{start(false)};
    track.reset();  // 3b: the cyclist's Obstacle is deleted
    std::cout << std::boolalpha << "3b: track == nullptr: " << (track == nullptr)
              << std::noboolalpha << '\n';  // true
  }
  {
    auto track{start(false)};
  }  // 3c: track's scope ends, and the cyclist's Obstacle is deleted
}
}  // namespace let_go

// [Slide 71] Deletion by the Last Owner
namespace last_owner {
void run() {
  std::vector<std::shared_ptr<Obstacle>> tracker;
  Hazards hazards;
  tracker.push_back(std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0));
  hazards.push_back(tracker[0]);
  std::cout << "both lists: " << hazards[0].use_count() << '\n';  // 2
  tracker.clear();  // the track is lost; the planner still owns it
  std::cout << "after tracker.clear(): " << hazards[0].use_count()
            << ", hazards[0]->id: " << hazards[0]->id << '\n';  // 1, 2
  hazards.clear();  // the last owner deletes the Obstacle
  std::cout << "after hazards.clear(): no owner left\n";
}  // LeakSanitizer reports nothing at exit: the Obstacle was freed
}  // namespace last_owner

// [Slide 72] Passing: Borrow
namespace borrow {
void print_obstacle(const Obstacle& target) {  // only reads it
  std::cout << "  obstacle " << target.id << ": " << target.speed << " m/s\n";
}

void run() {
  auto hazard{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  print_obstacle(*hazard);
  std::cout << hazard.use_count() << '\n';  // 1: no owner added
}
}  // namespace borrow

// [Slide 73] Passing: Share
namespace share {
void add_hazard(Hazards& hazards, std::shared_ptr<Obstacle> hazard) {
  hazards.push_back(std::move(hazard));  // the list keeps this owner
}

void run() {
  Hazards hazards;
  auto track{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  add_hazard(hazards, track);
  std::cout << track.use_count() << '\n';  // 2: the hazard list owns it too
}
}  // namespace share

// [Slide 74] Passing: const shared_ptr&
// The 25 m limit is chosen for this example.
namespace maybe_share {
void add_if_close(Hazards& hazards, const std::shared_ptr<Obstacle>& track) {
  if (track->pose.x < 25.0) {
    hazards.push_back(track);  // copies only when it keeps it
  }
}

void run() {
  Hazards hazards;
  auto near{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};             // the cyclist
  auto far{std::make_shared<Obstacle>(4, Pose{38.0, 3.5, 3.14}, 13.4)};        // the oncoming car
  add_if_close(hazards, near);  // x = 12 m: a hazard
  add_if_close(hazards, far);   // x = 38 m: not a hazard
  std::cout << "near.use_count(): " << near.use_count() << ", far.use_count(): "
            << far.use_count() << ", hazards.size(): " << hazards.size() << '\n';  // 2 1 1
}
}  // namespace maybe_share

// [Slide 75] Returning
namespace returning {
std::unique_ptr<Obstacle> make_track(int id) {  // [Slide 58]
  auto track{std::make_unique<Obstacle>(id)};
  return track;
}

void run() {
  std::shared_ptr<Obstacle> hazard{make_track(5)};  // unique_ptr to shared_ptr: compiles
  std::cout << "hazard->id: " << hazard->id << ", hazard.use_count(): " << hazard.use_count()
            << '\n';  // 5 1
  // Does not compile: no matching function for call to
  //                   'std::unique_ptr<Obstacle>::unique_ptr(<brace-enclosed initializer list>)'
  // std::unique_ptr<Obstacle> back{hazard};
}
}  // namespace returning

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {62, "Creating One", creating::run},
      {63, "The Control Block", control_block::run},
      {64, "use_count", use_count_steps::run},
      {65, "Three Ways to Let Go", let_go::run},
      {71, "Deletion by the Last Owner", last_owner::run},
      {72, "Passing: Borrow", borrow::run},
      {73, "Passing: Share", share::run},
      {74, "Passing: const shared_ptr&", maybe_share::run},
      {75, "Returning", returning::run},
  };
}

}  // namespace shared
