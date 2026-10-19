/**
 * @file undefined.cpp
 * @brief L7: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer (set_source_files_properties in CMakeLists.txt).
 * The sanitizer reports name lines of this file, and the slides quote them: a
 * change above a slide's code moves its line numbers.
 *
 * @warning Every slide here is a bug on purpose. Without the sanitizers a
 *          leak is silent, and a double delete or a dangling read may crash,
 *          print garbage, or appear to work.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "obstacle.hpp"

namespace undefined {

namespace {
Obstacle* make_track(int id, Pose pose, double speed) { return new Obstacle{id, pose, speed}; }
}  // namespace

// [Slide 89] The Cycle, Measured: the code of [Slide 86]
namespace shared_cycle {
struct Towing {
  Obstacle obstacle;
  std::shared_ptr<Towing> other;
};

void run() {
  auto a{std::make_shared<Towing>(Obstacle{5, {60.0, 0.0}, 8.0})};  // the tractor
  auto b{std::make_shared<Towing>(Obstacle{6, {54.0, 0.0}, 8.0})};  // the trailer
  a->other = b;  // a's Towing owns b's
  b->other = a;  // b's Towing owns a's
  std::cout << "a.use_count(): " << a.use_count() << ", b.use_count(): " << b.use_count()
            << '\n';  // 2 2
}  // a and b go out of scope: each count drops to 1, never 0, so neither is deleted
}  // namespace shared_cycle

// [Appendix ii] Double Delete, Measured
namespace double_delete {
void run() {
  Obstacle* track{make_track(2, Pose{12.0, -1.0}, 4.0)};
  std::vector<Obstacle*> tracker{track};
  std::vector<Obstacle*> hazards{track};  // the same Obstacle
  for (Obstacle* t : tracker) { delete t; }
  for (Obstacle* t : hazards) { delete t; }  // a second delete
}
}  // namespace double_delete

// [Appendix iii] Dangling Read, Measured
namespace dangling_read {
void run() {
  Obstacle* track{make_track(2, Pose{12.0, -1.0}, 4.0)};
  std::vector<Obstacle*> tracker{track};
  std::vector<Obstacle*> hazards{track};
  for (Obstacle* t : tracker) { delete t; }
  std::cout << hazards[0]->speed << '\n';  // freed memory
}
}  // namespace dangling_read

}  // namespace undefined
