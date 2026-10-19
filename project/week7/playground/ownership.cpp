/**
 * @file ownership.cpp
 * @brief L7 Section 1, Ownership: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week7_playground
 * 702run week7_playground       # every slide of every section, in order
 * 702run week7_playground 8     # only [Slide 8]
 * @endcode
 *
 * The double delete and the dangling read are undefined behavior, so their
 * code is in ../undefined/undefined.cpp, built with the sanitizers, and runs
 * from week7_appendix.
 */
#include <iostream>

#include "fleet.hpp"
#include "sections.hpp"

namespace ownership {

// [Slide 8] A Robot on the Heap
namespace robot_on_heap {
RobotStatus* make_robot(int id) {
  return new RobotStatus{id, 100.0, {0.0, 0.0}, false};  // Lecture 3: new
}

void run() {
  RobotStatus* robot{make_robot(3)};
  std::cout << "robot->id: " << robot->id << ", robot->battery_pct: " << robot->battery_pct
            << '\n';  // 3 100
  delete robot;       // the caller must remember this
  robot = nullptr;
}
}  // namespace robot_on_heap

// [Slide 9] Three Ways to Get It Wrong
// The slide shows three bugs. Each is a program of its own in
// ../undefined/undefined.cpp, built with the sanitizers:
//   leak:          no program; [Slide 52] shows the leak and its fix
//   double delete: undefined::double_delete, week7_appendix
//   dangling read: undefined::dangling_read, week7_appendix

// [Slide 10] RAII for One Object
// The slide has no code: it states the idea. The leak fixed with a
// std::unique_ptr is [Slide 52], once unique_ptr has been taught.

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {8, "A Robot on the Heap", robot_on_heap::run},
  };
}

}  // namespace ownership
