/**
 * @file static_members.cpp
 * @brief L8 Section 9, static Members: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. [Slide 71] uses a Tracker with a counter
 * in each object; [Slides 72 and 73] use the Tracker of
 * ../../common/include/tracker.hpp, whose counter is static.
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"
#include "tracker.hpp"

namespace static_members {

// [Slide 71] Two Trackers, the Same Id
namespace same_ids {
class Tracker {  // the Tracker of [Slide 69]: one counter in each object
 public:
  int add(Pose pose, double speed, bool is_static = false) {
    const int id{next_id_++};
    tracks_.push_back(std::make_unique<Obstacle>(id, pose, speed, is_static));
    return id;
  }

 private:
  std::vector<std::unique_ptr<Obstacle>> tracks_;
  int next_id_{1};  // the id add() gives next
};

void run() {
  Tracker front;  // the front camera's tracker
  Tracker rear;   // the rear camera's tracker
  std::cout << front.add(Pose{12.0, -1.0, 0.0}, 4.0) << ' '
            << rear.add(Pose{-8.0, 0.0, 0.0}, 12.0) << '\n';  // 1 1
  std::cout << sizeof(Tracker) << '\n';  // 32: the vector (24) and the counter
}
}  // namespace same_ids

// [Slide 72] One Counter for the Class
namespace one_counter {
void run() {
  ::Tracker front;
  ::Tracker rear;
  std::cout << front.add(Pose{20.0, -3.0, 0.0}, 0.0, true) << ' '
            << front.add(Pose{12.0, -1.0, 0.0}, 4.0) << ' '
            << rear.add(Pose{-8.0, 0.0, 0.0}, 12.0) << '\n';  // 1 2 3
  std::cout << ::Tracker::next_id() << '\n';                   // 4
  std::cout << sizeof(::Tracker) << '\n';                      // 24: the vector only
}
}  // namespace one_counter

// [Slide 73] A static Method Has No this
// Inside Tracker:
// static std::size_t count() noexcept {  // in Tracker: an error
//   return tracks_.size();
// }
// error: invalid use of member 'Tracker::tracks_' in static member function

std::vector<Slide> slides() {
  return {
      {71, "Two Trackers, the Same Id", same_ids::run},
      {72, "One Counter for the Class", one_counter::run},
  };
}

}  // namespace static_members
