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
 * 702run week7_playground 9    # only [Slide 9]
 * @endcode
 *
 * The double delete and the dangling read are undefined behavior, so their
 * code is in ../../undefined/src/undefined.cpp, built with the sanitizers,
 * and runs from week7_appendix.
 */
#include <iostream>

#include "obstacle.hpp"
#include "sections.hpp"

namespace ownership {

// [Slide 9] An Obstacle on the Heap
namespace obstacle_on_heap {
Obstacle* make_track(int id, Pose pose, double speed) {
  return new Obstacle{id, pose, speed};  // Lecture 3: new
}

void run() {
  Obstacle* cyclist{make_track(2, Pose{12.0, -1.0, 0.0}, 4.0)};
  std::cout << cyclist->id << ' ' << cyclist->speed << '\n';  // 2 4
  delete cyclist;  // the caller must remember this
  cyclist = nullptr;
}
}  // namespace obstacle_on_heap

// [Slide 10] RAII
// The slide has no code: it states the idea. The leak fixed with a
// std::unique_ptr is [Slide 46], once unique_ptr has been taught.

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {9, "An Obstacle on the Heap", obstacle_on_heap::run},
  };
}

}  // namespace ownership
