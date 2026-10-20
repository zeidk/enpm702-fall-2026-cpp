/**
 * @file unique.cpp
 * @brief L7 Section 3, Smart Pointers, std::unique_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 44    # only [Slide 44]
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

// [Slide 43] The Syntax
// 1 and 2 declare the same type; 3 compiles too, but writes new (R.23, slide 44).
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

// [Slide 44] Creating One
namespace creating {
void run() {
  Pose parked_pose{20.0, -3.0, 0.0};  // from the table
  auto parked_car{std::make_unique<Obstacle>(1, parked_pose, 0.0, true)};
  std::cout << parked_car->id << ' ' << (*parked_car).speed << '\n';  // 1 0
  auto empty{std::make_unique<Obstacle>()};  // members take defaults
  std::cout << empty->id << ' ' << empty->speed << '\n';    // 0 0
}
}  // namespace creating

// [Slide 45] In Memory
namespace in_memory {
void run() {
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  std::cout << "sizeof(parked_car): " << sizeof(parked_car) << ", sizeof(Obstacle*): " << sizeof(Obstacle*)
            << ", sizeof(Obstacle): " << sizeof(Obstacle) << '\n';  // 8 8 48
  std::cout << "&parked_car: " << &parked_car << " (stack), parked_car.get(): " << parked_car.get() << " (heap)\n";
}
}  // namespace in_memory

// [Slide 46] The Leak, Fixed
// The slide's left version, with Obstacle* and delete, leaks 48 bytes: the
// parked car is 20 m ahead, so the early return skips the delete. This is the
// right version.
namespace leak_fixed {
bool needs_braking(int id) {
  auto track{std::make_unique<Obstacle>(id, Pose{20.0, -3.0}, 0.0)};
  if (track->pose.x > 10.0) {
    return false;  // deleted here
  }
  return true;     // and here
}

void run() {
  std::cout << std::boolalpha << "needs_braking(1): " << needs_braking(1) << std::noboolalpha
            << '\n';  // false, and LeakSanitizer reports nothing at exit
}
}  // namespace leak_fixed

// [Slide 47] One Owner Only
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

// [Slide 48] get
namespace get_frame {
void print_obstacle(const Obstacle* target) {  // only looks
  std::cout << "  obstacle " << target->id << ": " << target->speed << " m/s\n";
}

void run() {
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  print_obstacle(parked_car.get());  // parked_car still owns it
  std::cout << std::boolalpha << "after get(), parked_car owns it: " << (parked_car != nullptr)
            << std::noboolalpha << '\n';  // true
}
}  // namespace get_frame

// [Slide 49] release
namespace release_frame {
void run() {
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  Obstacle* raw{parked_car.release()};  // gives up ownership, deletes nothing
  std::cout << std::boolalpha << "parked_car == nullptr: " << (parked_car == nullptr) << std::noboolalpha
            << ", raw->id: " << raw->id << '\n';  // true 1
  delete raw;  // you are the owner now: leave this out and it leaks
}
}  // namespace release_frame

// [Slide 51] reset
namespace reset_frame {
void run() {
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  parked_car.reset();  // deletes it now: parked_car == nullptr
  std::cout << std::boolalpha << "after reset(), parked_car == nullptr: " << (parked_car == nullptr)
            << std::noboolalpha << '\n';  // true
  parked_car = std::make_unique<Obstacle>(1, Pose{20.0, -3.0, 0.0}, 0.0, true);  // seen again
  std::cout << "after the assignment: obstacle " << parked_car->id << ": " << parked_car->speed
            << " m/s\n";  // obstacle 1: 0 m/s
}
}  // namespace reset_frame

// [Slide 53] Moving Ownership
namespace moving_ownership {
void run() {
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  Obstacle* where{parked_car.get()};
  std::unique_ptr<Obstacle> other{std::move(parked_car)};
  std::cout << std::boolalpha << "parked_car == nullptr: " << (parked_car == nullptr)
            << ", other.get() == where: " << (other.get() == where) << std::noboolalpha
            << ", other->id: " << other->id << '\n';  // true true 2
}
}  // namespace moving_ownership

// [Slide 56] Exercise 3: Handing Over a Track
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
  p = std::make_unique<Obstacle>(3);
  std::cout << "3: p: " << owns(p) << ", q: " << owns(q) << '\n';
  q = std::move(p);  // q gives up the Obstacle with id 1: reset deletes it
  std::cout << "4: p: " << owns(p) << ", q: " << owns(q) << ", obstacle 1 deleted here\n";
}
}  // namespace exercise_handover

// [Slide 58] The Tracker's List
namespace trackers_list {
void run() {
  std::vector<std::unique_ptr<Obstacle>> tracker;
  tracker.push_back(std::make_unique<Obstacle>(2, Pose{12.0, -1.0}, 4.0));
  auto parked_car{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  tracker.push_back(std::move(parked_car));  // parked_car is now empty
  // Does not compile: no matching function for call to 'construct_at(...)', then
  //                   use of deleted function 'std::unique_ptr<...>::unique_ptr(const ...&)'
  // tracker.push_back(parked_car);
  for (const auto& t : tracker) {  // by reference: a copy would not compile
    std::cout << "obstacle " << t->id << ": " << t->speed << " m/s\n";
  }
  // Does not compile either: copying the vector copies every unique_ptr.
  //   error: static assertion failed: result type must be constructible from input type
  // std::vector<std::unique_ptr<Obstacle>> backup{tracker};
}
}  // namespace trackers_list

// [Slide 61] Exercise 4: Lines That Compile
namespace exercise_compile {
void inspect(const Obstacle& target) { std::cout << "  inspect: obstacle " << target.id << '\n'; }
void adopt(std::unique_ptr<Obstacle> track) { std::cout << "  adopt: obstacle " << track->id << '\n'; }

void run() {
  auto a{std::make_unique<Obstacle>(1)};
  auto b{std::make_unique<Obstacle>(3)};
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
      {43, "The Syntax", syntax::run},
      {44, "Creating One", creating::run},
      {45, "In Memory", in_memory::run},
      {46, "The Leak, Fixed", leak_fixed::run},
      {47, "One Owner Only", one_owner::run},
      {48, "get", get_frame::run},
      {49, "release", release_frame::run},
      {51, "reset", reset_frame::run},
      {53, "Moving Ownership", moving_ownership::run},
      {56, "Exercise 3: Handing Over a Track", exercise_handover::run},
      {58, "The Tracker's List", trackers_list::run},
      {61, "Exercise 4: Lines That Compile", exercise_compile::run},
  };
}

}  // namespace unique
