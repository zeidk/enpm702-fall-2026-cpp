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
#include <type_traits>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"

namespace shared {

using Hazards = std::vector<std::shared_ptr<Obstacle>>;

// [Slide 60] A Hazard in Two Lists
// The slide states the problem: hazards holds tracker[1].get(), and erasing
// tracker[1] deletes the Obstacle, so hazards[0] dangles. A read through it is
// undefined behavior; [Appendix iii] runs one with the sanitizers.

// [Slide 61] The Syntax
// 1 and 2 declare the same type; 3 compiles too, but writes new and makes two
// allocations ([Appendix xiv]); 4 copies: a second owner of the same int.
namespace syntax {
void run() {
  std::shared_ptr<int> one{std::make_shared<int>(10)};  // 1
  auto two{std::make_shared<int>(10)};                  // 2
  std::shared_ptr<int> three{new int(10)};              // 3
  std::shared_ptr<int> four{one};                       // 4: a second owner
  static_assert(std::is_same_v<decltype(two), std::shared_ptr<int>>);
  std::cout << "*one, *two, *three, *four: " << *one << ' ' << *two << ' ' << *three << ' '
            << *four << '\n';  // 10 10 10 10

  auto count{std::make_shared<int>(10)};  // an int on the heap
  auto other{count};                      // a copy: a second owner
  *other += 1;                            // through either owner
  std::cout << *count << '\n';            // 11: the same int
}
}  // namespace syntax

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
  auto cyclist{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  std::shared_ptr<Obstacle> hazard{cyclist};
  std::cout << "sizeof(cyclist): " << sizeof(cyclist) << ", sizeof(Obstacle): " << sizeof(Obstacle)
            << '\n';                                                   // 16 48
  std::cout << "cyclist.use_count(): " << cyclist.use_count() << '\n';  // 2
}
}  // namespace control_block

// [Slide 64] use_count
namespace use_count_steps {
void run() {
  auto cyclist{std::make_shared<Obstacle>(2)};
  std::cout << "created: " << cyclist.use_count() << '\n';              // 1
  std::shared_ptr<Obstacle> hazard{cyclist};
  std::cout << "copied: " << cyclist.use_count() << '\n';               // 2
  std::shared_ptr<Obstacle> moved{std::move(hazard)};
  std::cout << std::boolalpha << "moved: " << cyclist.use_count()
            << ", hazard == nullptr: " << (hazard == nullptr) << std::noboolalpha
            << '\n';                                                     // 2 true
  moved.reset();
  std::cout << "reset: " << cyclist.use_count() << '\n';                // 1
}
}  // namespace use_count_steps

// [Slide 65] Three Ways to Let Go
// Three separate runs, each from step 2; start() makes steps 1 and 2 and prints
// their counts once, in the first run. [Slides 60 to 64] show each step.
namespace let_go {
std::shared_ptr<Obstacle> start(bool show) {
  auto cyclist{std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0)};
  {
    std::shared_ptr<Obstacle> hazard{cyclist};  // 1
    if (show) {
      std::cout << "1: " << cyclist.use_count() << '\n';  // 2
    }
  }  // 2: hazard's scope ends
  if (show) {
    std::cout << "2: " << cyclist.use_count() << '\n';  // 1
  }
  return cyclist;
}

void run() {
  {
    auto cyclist{start(true)};
    cyclist = std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0);  // 3a: seen again; the first Obstacle is deleted
    std::cout << "3a: cyclist->id: " << cyclist->id << ", use_count: " << cyclist.use_count()
              << '\n';                                                     // 2 1
  }
  {
    auto cyclist{start(false)};
    cyclist.reset();  // 3b: the cyclist's Obstacle is deleted
    std::cout << std::boolalpha << "3b: cyclist == nullptr: " << (cyclist == nullptr)
              << std::noboolalpha << '\n';  // true
  }
  {
    auto cyclist{start(false)};
  }  // 3c: cyclist's scope ends, and the cyclist's Obstacle is deleted
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

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {61, "The Syntax", syntax::run},
      {62, "Creating One", creating::run},
      {63, "The Control Block", control_block::run},
      {64, "use_count", use_count_steps::run},
      {65, "Three Ways to Let Go", let_go::run},
      {71, "Deletion by the Last Owner", last_owner::run},
  };
}

}  // namespace shared
