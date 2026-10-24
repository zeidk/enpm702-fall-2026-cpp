/**
 * @file undefined.cpp
 * @brief L8: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with -fsanitize=address,undefined (see
 * ../../CMakeLists.txt). The reports on the slides come from these runs.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "obstacle.hpp"

namespace undefined {

// [Slide 62] Without the Destructor
// The Track of [Slide 61] with ~Track removed. LeakSanitizer reports, at exit:
// Direct leak of 48 byte(s) in 1 object(s) allocated from: (three times)
// SUMMARY: AddressSanitizer: 144 byte(s) leaked in 3 allocation(s).
namespace no_destructor {
class Track {  // owns one Obstacle on the heap, and never gives it back
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

void run() {
  {
    Track parked{Obstacle{1}};
    Track cyclist{Obstacle{2}};
    std::cout << "end of scope\n";
  }
  auto pedestrian{std::make_unique<Track>(Obstacle{3})};
  pedestrian.reset();
  std::cout << "after reset\n";
}
}  // namespace no_destructor

// [Slide 64] Double Free, Measured
// The Track of [Slide 60], copied by the compiler's copy constructor
// ([Slide 63]). AddressSanitizer stops the program:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace double_free {
class Track {  // owns one Obstacle on the heap
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}
  ~Track() { delete obstacle_; }
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

void run() {
  Track cyclist{Obstacle{2, Pose{12.0, -1.0, 0.0}, 4.0}};
  Track copy{cyclist};  // the compiler's copy constructor
  std::cout << copy.get().id << '\n';  // 2
}  // copy's destructor deletes the Obstacle, then cyclist's deletes it again
}  // namespace double_free

// [Slide 69] Discussion: Who Deletes What?
// The move assignment does not set other.obstacle_ to nullptr, so after line 9
// parked and cyclist point at the same Obstacle (id 2), and the Obstacle with
// id 1 was deleted. At the end of the scope both destructors delete id 2:
// ERROR: AddressSanitizer: attempting double-free on 0x... in thread T0:
namespace move_bug {
class Track {
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}
  ~Track() { delete obstacle_; }
  Track(const Track&) = delete;
  Track& operator=(const Track&) = delete;
  Track(Track&& other) noexcept : obstacle_{other.obstacle_} {
    other.obstacle_ = nullptr;
  }
  Track& operator=(Track&& other) noexcept {
    delete obstacle_;
    obstacle_ = other.obstacle_;
    return *this;
  }
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

void run() {
  Track parked{Obstacle{1}};
  Track cyclist{Obstacle{2}};
  parked = std::move(cyclist);
  std::cout << parked.get().id << '\n';  // 2
}
}  // namespace move_bug

}  // namespace undefined
