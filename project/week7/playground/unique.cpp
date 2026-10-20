/**
 * @file unique.cpp
 * @brief L7 Section 3, Smart Pointers, std::unique_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 46    # only [Slide 46]
 * @endcode
 *
 * The tracker owns each Obstacle it tracks through a std::unique_ptr. Code
 * that does not compile is commented out where its slide shows it: uncomment
 * it, build, and you get the slide's error.
 */
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "sections.hpp"

namespace unique {

// [Slide 45] The Syntax
// 1 and 2 declare the same type; 3 compiles too, but writes new (R.23, slide 46).
namespace syntax {
void run() {
  std::unique_ptr<int> one{std::make_unique<int>(10)};  // 1
  auto two{std::make_unique<int>(10)};                  // 2
  std::unique_ptr<int> three{new int(10)};              // 3
  static_assert(std::is_same_v<decltype(two), std::unique_ptr<int>>);
  std::cout << "*one, *two, *three: " << *one << ' ' << *two << ' ' << *three << '\n';  // 10 10 10

  auto count{std::make_unique<int>(10)};  // an int on the heap
  *count += 1;                            // as with an int*
  std::cout << *count << '\n';            // 11
}
}  // namespace syntax

// [Slide 46] Creating One
namespace creating {
void run() {
  Pose at{12.0, -1.0, 0.0};  // the cyclist's pose, from the table
  std::unique_ptr<Obstacle> track{std::make_unique<Obstacle>(2, at, 4.0)};
  std::cout << track->id << ' ' << (*track).speed << '\n';  // 2 4
  auto empty{std::make_unique<Obstacle>()};  // members take defaults
  std::cout << empty->id << ' ' << empty->speed << '\n';    // 0 0
}
}  // namespace creating

// [Slide 47] In Memory
namespace in_memory {
void run() {
  auto track{std::make_unique<Obstacle>(2, Pose{12.0, -1.0, 0.0}, 4.0)};
  std::cout << "sizeof(track): " << sizeof(track) << ", sizeof(Obstacle*): " << sizeof(Obstacle*)
            << ", sizeof(Obstacle): " << sizeof(Obstacle) << '\n';  // 8 8 48
  std::cout << "&track: " << &track << " (stack), track.get(): " << track.get() << " (heap)\n";
}
}  // namespace in_memory

// [Slide 48] The Leak, Fixed
// The slide's left version, with Obstacle* and delete, leaks 48 bytes: the
// cyclist is 12 m ahead, so the early return skips the delete. This is the
// right version.
namespace leak_fixed {
bool needs_braking(int id) {
  auto track{std::make_unique<Obstacle>(id, Pose{12.0, -1.0}, 4.0)};
  if (track->pose.x > 10.0) {
    return false;  // deleted here
  }
  return true;     // and here
}

void run() {
  std::cout << std::boolalpha << "needs_braking(2): " << needs_braking(2) << std::noboolalpha
            << '\n';  // false, and LeakSanitizer reports nothing at exit
}
}  // namespace leak_fixed

// [Slide 49] One Owner Only
namespace one_owner {
void run() {
  auto track{std::make_unique<Obstacle>()};
  // Does not compile: use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const
  //                   std::unique_ptr<_Tp, _Dp>&) [with _Tp = Obstacle; _Dp =
  //                   std::default_delete<Obstacle>]'
  // std::unique_ptr<Obstacle> copy{track};
  std::cout << "track->id: " << track->id << '\n';  // 0
}
}  // namespace one_owner

// [Slide 50] get and reset
namespace get_and_reset {
void print_obstacle(const Obstacle* target) {  // only looks
  std::cout << "  obstacle " << target->id << ": " << target->speed << " m/s\n";
}

void run() {
  auto track{std::make_unique<Obstacle>(2, Pose{12.0, -1.0, 0.0}, 4.0)};
  print_obstacle(track.get());  // track still owns it
  track.reset();                // deletes it now: track == nullptr
  std::cout << std::boolalpha << "after reset(), track == nullptr: " << (track == nullptr)
            << std::noboolalpha << '\n';  // true
  track = std::make_unique<Obstacle>(3, Pose{30.0, 2.5, 1.57}, 1.4);
  print_obstacle(track.get());  // obstacle 3: 1.4 m/s
}
}  // namespace get_and_reset

// [Slide 51] Moving Ownership
namespace moving_ownership {
void run() {
  auto track{std::make_unique<Obstacle>(2, Pose{12.0, -1.0, 0.0}, 4.0)};
  Obstacle* where{track.get()};
  std::unique_ptr<Obstacle> other{std::move(track)};
  std::cout << std::boolalpha << "track == nullptr: " << (track == nullptr)
            << ", other.get() == where: " << (other.get() == where) << std::noboolalpha
            << ", other->id: " << other->id << '\n';  // true true 2
}
}  // namespace moving_ownership

// [Slide 54] Exercise 3: Handing Over a Track
// The Obstacle with id 1 is deleted on line 4: q = std::move(p) calls
// q.reset(p.release()), and reset deletes the old pointer when it is not null
// (C++20 [unique.ptr.single.asgn] paragraph 3, [unique.ptr.single.modifiers]
// paragraph 4).
namespace exercise_handover {
std::string owns(const std::unique_ptr<Obstacle>& ptr) {
  return ptr ? "obstacle " + std::to_string(ptr->id) : std::string{"empty"};
}

void run() {
  auto p{std::make_unique<Obstacle>(1)};
  std::cout << "1: p: " << owns(p) << '\n';
  auto q{std::move(p)};
  std::cout << "2: p: " << owns(p) << ", q: " << owns(q) << '\n';
  p = std::make_unique<Obstacle>(2);
  std::cout << "3: p: " << owns(p) << ", q: " << owns(q) << '\n';
  q = std::move(p);  // q gives up the Obstacle with id 1: reset deletes it
  std::cout << "4: p: " << owns(p) << ", q: " << owns(q) << ", obstacle 1 deleted here\n";
}
}  // namespace exercise_handover

// [Slide 55] The Tracker's List
namespace trackers_list {
void run() {
  std::vector<std::unique_ptr<Obstacle>> tracker;
  tracker.push_back(std::make_unique<Obstacle>(2, Pose{12.0, -1.0}, 4.0));
  auto track{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  tracker.push_back(std::move(track));  // track is now empty
  // Does not compile: no matching function for call to 'construct_at(...)', then
  //                   use of deleted function 'std::unique_ptr<...>::unique_ptr(const ...&)'
  // tracker.push_back(track);
  for (const auto& t : tracker) {  // by reference: a copy would not compile
    std::cout << "obstacle " << t->id << ": " << t->speed << " m/s\n";
  }
  // Does not compile either: copying the vector copies every unique_ptr.
  //   error: static assertion failed: result type must be constructible from input type
  // std::vector<std::unique_ptr<Obstacle>> backup{tracker};
}
}  // namespace trackers_list

// [Slide 56] Passing: Borrow
namespace borrow {
void print_obstacle(const Obstacle& target) {  // only reads it
  std::cout << "  obstacle " << target.id << ": " << target.speed << " m/s\n";
}
void set_speed(Obstacle* target, double speed) {  // may get nullptr
  if (target != nullptr) { target->speed = speed; }
}

void run() {
  auto track{std::make_unique<Obstacle>(2, Pose{12.0, -1.0, 0.0}, 4.0)};
  print_obstacle(*track);       // 4 m/s
  set_speed(track.get(), 2.0);
  print_obstacle(*track);       // 2 m/s: the caller still owns it
}
}  // namespace borrow

// [Slide 57] Passing: Sink
namespace sink {
std::vector<std::unique_ptr<Obstacle>> lost;  // lost tracks, kept

void keep_lost(std::unique_ptr<Obstacle> track) {  // takes ownership
  lost.push_back(std::move(track));
}

void run() {
  auto track{std::make_unique<Obstacle>(2)};
  // Does not compile: use of deleted function 'std::unique_ptr<...>::unique_ptr(const ...&)'
  // keep_lost(track);
  keep_lost(std::move(track));
  std::cout << std::boolalpha << "track == nullptr: " << (track == nullptr) << std::noboolalpha
            << ", lost.size(): " << lost.size() << '\n';  // true 1
}
}  // namespace sink

// [Slide 58] Returning
namespace returning {
std::unique_ptr<Obstacle> make_track(int id) {
  auto track{std::make_unique<Obstacle>(id)};
  return track;  // no std::move: the implicit move of Lecture 5
}

void run() {
  std::unique_ptr<Obstacle> track{make_track(5)};
  std::cout << "track->id: " << track->id << '\n';  // 5
}
}  // namespace returning

// [Slide 59] Exercise 4: Lines That Compile
namespace exercise_compile {
void inspect(const Obstacle& target) { std::cout << "  inspect: obstacle " << target.id << '\n'; }
void adopt(std::unique_ptr<Obstacle> track) { std::cout << "  adopt: obstacle " << track->id << '\n'; }

void run() {
  auto a{std::make_unique<Obstacle>(1)};
  auto b{std::make_unique<Obstacle>(2)};
  // For each line, say whether it compiles. The lines that do not compile are
  // commented out: uncomment one to check your answer.
  // auto c{a};                                    // line 1
  auto d{std::move(a)};                            // line 2
  inspect(*d);                                     // line 3
  // adopt(b);                                     // line 4
  adopt(std::move(b));                             // line 5
  std::unique_ptr<Obstacle> e{new Obstacle{}};     // line 6, see the note below
  Obstacle* f{d.get()};                            // line 7
  // std::unique_ptr<Obstacle> g{f};               // line 8, see the note below
  std::cout << "  f->id: " << f->id << ", e->id: " << e->id << '\n';
  // Line 6 compiles and is correct, but skips make_unique (R.23). Line 8
  // compiles too, and is a bug: d and g would both delete the same Obstacle,
  // so it stays commented out.
}
}  // namespace exercise_compile

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {45, "The Syntax", syntax::run},
      {46, "Creating One", creating::run},
      {47, "In Memory", in_memory::run},
      {48, "The Leak, Fixed", leak_fixed::run},
      {49, "One Owner Only", one_owner::run},
      {50, "get and reset", get_and_reset::run},
      {51, "Moving Ownership", moving_ownership::run},
      {54, "Exercise 3: Handing Over a Track", exercise_handover::run},
      {55, "The Tracker's List", trackers_list::run},
      {56, "Passing: Borrow", borrow::run},
      {57, "Passing: Sink", sink::run},
      {58, "Returning", returning::run},
      {59, "Exercise 4: Lines That Compile", exercise_compile::run},
  };
}

}  // namespace unique
