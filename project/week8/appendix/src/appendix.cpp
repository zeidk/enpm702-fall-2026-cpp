/**
 * @file appendix.cpp
 * @brief L8: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_appendix.
 *
 * @code
 * 702run week8_appendix        # every appendix frame
 * 702run week8_appendix iii    # only [Appendix iii]
 * @endcode
 */
#include <iostream>
#include <memory>
#include <vector>

#include "obstacle.hpp"
#include "slides.hpp"

namespace {

// [Appendix iii] Returning a Large Member
namespace large_member {
class Tracker {
 public:
  int add(Pose pose, double speed, bool is_static = false) {
    const int id{next_id_++};
    tracks_.push_back(std::make_unique<Obstacle>(id, pose, speed, is_static));
    return id;
  }

  [[nodiscard]] const std::vector<std::unique_ptr<Obstacle>>&
  get_tracks() const noexcept {
    return tracks_;
  }

  // Returning a copy does not compile: each std::unique_ptr would be copied.
  // [[nodiscard]] std::vector<std::unique_ptr<Obstacle>> get_tracks_copy() const {
  //   return tracks_;
  // }

 private:
  std::vector<std::unique_ptr<Obstacle>> tracks_;
  inline static int next_id_{1};
};

void run() {
  Tracker front;
  front.add(Pose{20.0, -3.0, 0.0}, 0.0, true);
  front.add(Pose{12.0, -1.0, 0.0}, 4.0);
  for (const std::unique_ptr<Obstacle>& track : front.get_tracks()) {
    std::cout << track->id << ' ' << track->speed << '\n';
  }  // prints 1 0, then 2 4
}
}  // namespace large_member

}  // namespace

int main(int argc, char* argv[]) {
  return run_slides({{3, "Returning a Large Member", large_member::run}}, argc, argv,
                    Part::appendix);
}
