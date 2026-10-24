/**
 * @file destructors.cpp
 * @brief L8 Section 8, Destructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground. Each slide's Track is its own class, in
 * its own namespace, because the class changes from slide to slide: a raw
 * pointer and a destructor ([Slide 56]), the rule of five ([Slides 62 to 64]),
 * a std::unique_ptr ([Slides 66 and 67]), deleted copies ([Slide 68]).
 *
 * The leak of [Slide 58] and the double free of [Slide 60] are in
 * ../../undefined/src/undefined.cpp, built with the sanitizers. They run only
 * when asked for by number.
 */
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"
#include "undefined.hpp"

namespace destructors {

// [Slide 57] When It Runs: the Track of [Slide 56], and ~Track also prints.
namespace when_it_runs {
class Track {  // owns one Obstacle on the heap
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}
  ~Track() {
    std::cout << "~Track: id " << obstacle_->id << '\n';
    delete obstacle_;
  }
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

void run() {
  {
    Track parked{Obstacle{1}};   // id 1; the
    Track cyclist{Obstacle{2}};  // rest default
    std::cout << "end of scope\n";
  }
  auto pedestrian{
      std::make_unique<Track>(Obstacle{3})};
  pedestrian.reset();
  std::cout << "after reset\n";
}
}  // namespace when_it_runs

// [Slides 62 to 64] The Rule of Five. ~Track prints as on [Slide 57], and 0
// when the Track owns nothing (after a move).
namespace five {
class Track {
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}

  ~Track() {
    std::cout << "~Track: id " << (obstacle_ != nullptr ? obstacle_->id : 0) << '\n';
    delete obstacle_;
  }

  // [Slide 62] The Copies
  Track(const Track& other)  // a new Obstacle with other's values
      : obstacle_{new Obstacle{*other.obstacle_}} {}

  Track& operator=(const Track& other) {
    *obstacle_ = *other.obstacle_;  // into our own Obstacle
    return *this;
  }

  // [Slide 63] The Moves
  Track(Track&& other) noexcept : obstacle_{other.obstacle_} {
    other.obstacle_ = nullptr;  // the source owns nothing now
  }

  Track& operator=(Track&& other) noexcept {
    if (this != &other) {       // t = std::move(t) must not delete
      delete obstacle_;         // give back what we owned
      obstacle_ = other.obstacle_;
      other.obstacle_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

// [Slide 64] All Five, Run
void run() {
  Track cyclist{Obstacle{2}};
  Track copy{cyclist};
  std::cout << "copy: id " << copy.get().id << ", own Obstacle: "
            << (&copy.get() != &cyclist.get()) << '\n';  // copy: id 2, own Obstacle: 1
  Track parked{Obstacle{1}};
  copy = parked;
  std::cout << "after copy assignment: id " << copy.get().id << '\n';  // id 1
  Track moved{std::move(cyclist)};
  std::cout << "moved: id " << moved.get().id << '\n';  // id 2
  parked = std::move(moved);
  std::cout << "after move assignment: id " << parked.get().id << '\n';  // id 2
}  // ~Track: id 0, id 2, id 1, id 0
}  // namespace five

// [Slides 66 and 67] The Rule of Zero
namespace zero {
class Track {
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{std::make_unique<Obstacle>(seen)} {}
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  std::unique_ptr<Obstacle> obstacle_;  // deletes the Obstacle
};

void run() {
  Track cyclist{Obstacle{2, Pose{12.0, -1.0, 0.0}, 4.0}};
  Track moved{std::move(cyclist)};  // OK: moves the unique_ptr
  std::cout << moved.get().id << '\n';  // 2
  // Track copy{moved};             // does not compile:
  // error: use of deleted function 'Track::Track(const Track&)'
  // note: 'Track::Track(const Track&)' is implicitly deleted because the
  //       default definition would be ill-formed:
}
}  // namespace zero

// [Slide 68] = default and = delete: the raw-pointer Track, move-only.
// Remove the two move lines and Track moved{std::move(cyclist)}; picks the
// deleted copy: error: use of deleted function 'Track::Track(const Track&)'
namespace move_only {
class Track {  // the raw-pointer Track, move-only on purpose
 public:
  explicit Track(const Obstacle& seen)
      : obstacle_{new Obstacle{seen}} {}
  ~Track() { delete obstacle_; }
  Track(const Track&) = delete;             // no copies
  Track& operator=(const Track&) = delete;
  Track(Track&& other) noexcept : obstacle_{other.obstacle_} {
    other.obstacle_ = nullptr;
  }
  Track& operator=(Track&& other) noexcept {
    if (this != &other) {
      delete obstacle_;
      obstacle_ = other.obstacle_;
      other.obstacle_ = nullptr;
    }
    return *this;
  }
  [[nodiscard]] const Obstacle& get() const noexcept {
    return *obstacle_;
  }

 private:
  Obstacle* obstacle_;
};

void run() {
  Track cyclist{Obstacle{2}};
  Track moved{std::move(cyclist)};
  std::cout << moved.get().id << '\n';  // 2
}
}  // namespace move_only

// [Slide 69] The Tracker, with an id counter in each object. Section 9 makes
// the counter static: ../../common/include/tracker.hpp.
namespace tracker {
class Tracker {  // owns one Obstacle for each obstacle it sees
 public:
  int add(Pose pose, double speed, bool is_static = false) {
    const int id{next_id_++};
    tracks_.push_back(std::make_unique<Obstacle>(id, pose, speed, is_static));
    return id;
  }
  bool lose(int id) {
    return std::erase_if(tracks_, [id](const std::unique_ptr<Obstacle>& track) {
             return track->id == id;
           }) > 0;
  }
  [[nodiscard]] const Obstacle* find(int id) const {
    for (const std::unique_ptr<Obstacle>& track : tracks_) {
      if (track->id == id) {
        return track.get();
      }
    }
    return nullptr;
  }
  [[nodiscard]] std::size_t size() const noexcept {
    return tracks_.size();
  }

 private:
  std::vector<std::unique_ptr<Obstacle>> tracks_;
  int next_id_{1};  // the id add() gives next
};

void run() {
  Tracker front;
  const int parked{front.add(Pose{20.0, -3.0, 0.0}, 0.0, true)};
  const int cyclist{front.add(Pose{12.0, -1.0, 0.0}, 4.0)};
  std::cout << parked << ' ' << cyclist << ' ' << front.size() << '\n';  // 1 2 2
  if (const Obstacle* seen{front.find(cyclist)}) {
    std::cout << seen->speed << '\n';  // 4
  }
  front.lose(cyclist);
  std::cout << front.size() << ' ' << (front.find(cyclist) == nullptr) << '\n';  // 1 1
  // Tracker copy{front};  // does not compile: errors from inside std::vector,
  // use of deleted function 'std::unique_ptr<...>::unique_ptr(const std::unique_ptr<...>&)'
}
}  // namespace tracker

std::vector<Slide> slides() {
  return {
      {57, "When It Runs", when_it_runs::run},
      {58, "Without the Destructor", undefined::no_destructor::run, true},
      {60, "Double Free, Measured", undefined::double_free::run, true},
      {64, "All Five, Run", five::run},
      {67, "What the Compiler Writes Now", zero::run},
      {68, "= default and = delete", move_only::run},
      {69, "The Tracker", tracker::run},
      {65, "Exercise 4: A Move Assignment with a Bug", undefined::move_bug::run, true},
  };
}

}  // namespace destructors
