/**
 * @file appendix.cpp
 * @brief L7, Smart Pointers and Move Semantics: the code of every appendix
 *        frame, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week7_appendix.
 *
 * @code
 * 702build week7_appendix
 * 702run week7_appendix         # every appendix frame, in order
 * 702run week7_appendix iv      # only [Appendix iv]; 702run week7_appendix 4 works too
 * @endcode
 *
 * The appendix frames are numbered i, ii, iii, ... in their top-left corner,
 * and @c "[Appendix iv]" is that number. Each frame's code is in its own
 * namespace, with a @c run() that does what the frame does in @c main(). The
 * table in main() lists them, and run_slides() in @c ../../common/src/slides.cpp runs
 * them. Code with undefined behavior is in @c ../../undefined/src/undefined.cpp,
 * built with the sanitizers; a full run skips it.
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "obstacle.hpp"
#include "slides.hpp"
#include "undefined.hpp"

namespace {
// A path of n poses, one meter apart along the road.
std::vector<Pose> make_path(int n) {
  std::vector<Pose> path;
  path.reserve(n);  // room for n poses: one allocation
  for (int i{0}; i < n; ++i) {
    path.push_back({static_cast<double>(i), 0.0, 0.0});
  }
  return path;
}
}  // namespace

// [Appendix ii] Double Delete, Measured: undefined::double_delete
// [Appendix iii] Dangling Read, Measured: undefined::dangling_read

// [Appendix iv] make_path
namespace make_path_frame {
void run() {
  std::vector<Pose> path{make_path(3)};
  for (const Pose& p : path) {
    std::cout << '(' << p.x << ", " << p.y << ", " << p.heading << ") ";
  }
  std::cout << '\n';  // (0, 0, 0) (1, 0, 0) (2, 0, 0)
  std::vector<Pose> long_path{make_path(1000)};
  std::cout << "make_path(1000): size " << long_path.size() << ", capacity "
            << long_path.capacity() << '\n';  // 1000 1000: reserve made one block
}
}  // namespace make_path_frame

// [Appendix v] The Moved-from std::string
namespace moved_string {
void run() {
  std::string note{"cyclist ahead, keeping to the right"};
  std::string name{"cyclist"};
  std::string taken_note{std::move(note)};
  std::string taken_name{std::move(name)};
  std::cout << "note.size(): " << note.size() << ", name.size(): " << name.size()
            << '\n';  // 0 0 with libstdc++; the standard does not promise it
  note = "pedestrian crossing at 30 m";  // a known value again
  std::cout << "note: " << note << '\n';
}
}  // namespace moved_string

// [Appendix vi] Move Assignment
namespace move_assignment {
void run() {
  std::vector<Pose> current{make_path(3)};
  std::vector<Pose> next{make_path(1000)};
  const Pose* where{next.data()};
  current = std::move(next);  // current's 3 poses are destroyed
  std::cout << std::boolalpha << "current.size(): " << current.size()
            << ", current.data() == where: " << (current.data() == where)
            << ", next.size(): " << next.size() << std::noboolalpha
            << '\n';  // 1000 true 0 (libstdc++)
}
}  // namespace move_assignment

// [Appendix vii] std::exchange
namespace exchange {
void run() {
  std::vector<Pose> path{make_path(1000)};
  // take the path, and leave an empty one behind
  std::vector<Pose> done{std::exchange(path, {})};
  std::cout << "done.size(): " << done.size() << ", path.size(): " << path.size()
            << '\n';  // 1000 0, guaranteed
  int track_id{7};
  int lost{std::exchange(track_id, 0)};
  std::cout << "lost: " << lost << ", track_id: " << track_id << '\n';  // 7 0
}
}  // namespace exchange

// [Appendix viii] The Definition of std::move
namespace move_written {
// The standard's std::move, under another name ([forward] para 5).
template <typename T>
constexpr std::remove_reference_t<T>&& my_move(T&& t) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(t);
}

void run() {
  std::vector<Pose> path{make_path(1000)};
  static_assert(std::is_same_v<decltype(my_move(path)), std::vector<Pose>&&>);
  static_assert(std::is_same_v<decltype(my_move(path)), decltype(std::move(path))>);
  std::vector<Pose> taken{my_move(path)};
  std::cout << "taken.size(): " << taken.size() << ", path.size(): " << path.size()
            << '\n';  // 1000 0
}
}  // namespace move_written

// [Appendix ix] A unique_ptr with Its Own Deleter
namespace own_deleter {
void run() {
  auto say_and_delete{[](Obstacle* target) {
    std::cout << "  deleting obstacle " << target->id << '\n';
    delete target;
  }};
  {
    std::unique_ptr<Obstacle, decltype(say_and_delete)> track{
        new Obstacle{1}, say_and_delete};
    std::cout << "sizeof(track): " << sizeof(track) << '\n';  // 8
  }  // track is destroyed here: "deleting obstacle 1"
  std::cout << "after the block\n";
  std::cout << "with a function pointer: "
            << sizeof(std::unique_ptr<Obstacle, void (*)(Obstacle*)>) << '\n';  // 16
}
}  // namespace own_deleter

// [Appendix x] std::auto_ptr
// No code: std::auto_ptr is not part of C++17 or C++20. g++ 13.3 still
// accepts it with -std=c++20, with a -Wdeprecated-declarations warning, and
// after std::auto_ptr<Obstacle> b{a}; the pointer a is null.

// [Appendix xi] Testing for Empty
namespace testing_empty {
void run() {
  std::unique_ptr<Obstacle> track;  // empty
  std::cout << std::boolalpha << "track == nullptr: " << (track == nullptr) << '\n';  // true
  track = std::make_unique<Obstacle>(1);
  if (track) {
    std::cout << "obstacle " << track->id << '\n';  // obstacle 1
  }
  auto other{std::move(track)};
  std::cout << "after the move, !track: " << !track << std::noboolalpha << '\n';  // true
  // Does not compile: cannot convert 'std::unique_ptr<Obstacle,
  //                   std::default_delete<Obstacle> >' to 'bool' in initialization
  // bool has_track = other;
}
}  // namespace testing_empty

// [Appendix xii] unique_ptr to an Array
namespace unique_array {
void run() {
  auto ranges{std::make_unique<double[]>(5)};  // five distances, each 0.0
  ranges[2] = 12.0;
  for (int i{0}; i < 5; ++i) {
    std::cout << ranges[i] << ' ';
  }
  std::cout << '\n';  // 0 0 12 0 0
  std::cout << "sizeof(ranges): " << sizeof(ranges)
            << ", sizeof(std::vector<double>): " << sizeof(std::vector<double>)
            << '\n';  // 8 24
}
}  // namespace unique_array

// [Appendix xiii] swap
namespace swapping {
void run() {
  auto first{std::make_unique<Obstacle>(1)};
  auto second{std::make_unique<Obstacle>(4)};
  const Obstacle* id_1{first.get()};
  first.swap(second);  // the same as std::swap(first, second)
  std::cout << std::boolalpha << "first->id: " << first->id << ", second->id: " << second->id
            << ", second.get() == id_1: " << (second.get() == id_1) << std::noboolalpha
            << '\n';  // 4 1 true
}
}  // namespace swapping

// [Appendix xiv] One Allocation with make_shared
// Counting allocations needs a replaced global operator new, which clashes
// with the sanitizers this program is built with. The count is its own
// program, allocations.cpp: 702run week7_allocations

// [Appendix xv] A Dashboard of weak_ptr
namespace dashboard {
void run() {
  std::vector<std::shared_ptr<Obstacle>> tracker;
  tracker.push_back(std::make_shared<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true));
  tracker.push_back(std::make_shared<Obstacle>(2, Pose{12.0, -1.0}, 4.0));
  tracker.push_back(std::make_shared<Obstacle>(3, Pose{30.0, 2.5, 1.57}, 1.4));
  std::vector<std::weak_ptr<Obstacle>> shown;
  for (const auto& track : tracker) {  // ids 1, 2, 3
    shown.push_back(track);  // watches, does not own
  }
  tracker.erase(tracker.begin() + 1);  // deletes the Obstacle with id 2
  for (const auto& watched : shown) {
    if (auto target{watched.lock()}) {
      std::cout << "  obstacle " << target->id << ": " << target->speed << " m/s\n";
    } else {
      std::cout << "  (gone)\n";
    }
  }
  std::erase_if(shown, [](const std::weak_ptr<Obstacle>& w) { return w.expired(); });
  std::cout << "shown.size(): " << shown.size() << '\n';  // 2
}
}  // namespace dashboard

// The value category of an expression, for the frames moved from the main deck
// on 2026-10-19 ([Appendix xxi] to [Appendix xxiv]). decltype((e)) is T& for an
// lvalue, T&& for an xvalue and T for a prvalue; the same test as moves.cpp.
template <typename T>
constexpr std::string_view category() {
  if constexpr (std::is_lvalue_reference_v<T>) {
    return "lvalue";
  } else if constexpr (std::is_rvalue_reference_v<T>) {
    return "xvalue";
  } else {
    return "prvalue";
  }
}
// A macro, because decltype must see the expression itself, not a copy of it.
#define CATEGORY(e) category<decltype((e))>()

// [Appendix xxi] A Reference, an Element, *p
namespace lvalue_more {
void run() {
  int count{3};
  int& alias{count};  // another name for count (L3)
  int numbers[3]{10, 20, 30};
  int* p{&numbers[1]};  // p holds the address of numbers[1]
  std::cout << "alias: " << CATEGORY(alias) << '\n';            // lvalue
  std::cout << "numbers[2]: " << CATEGORY(numbers[2]) << '\n';  // lvalue
  std::cout << "*p: " << CATEGORY(*p) << '\n';                  // lvalue
  std::cout << std::boolalpha << "&alias == &count: " << (&alias == &count)
            << ", &*p == &numbers[1]: " << (&*p == &numbers[1]) << std::noboolalpha
            << '\n';  // true true
}
}  // namespace lvalue_more

// [Appendix xxii] Literals, Results, Calls
namespace prvalue_more {
int twice(int n) { return 2 * n; }  // returns an int by value

void run() {
  int count{3};
  int doubled{count * 2};
  int again{twice(count)};
  std::cout << "count: " << count << ", doubled: " << doubled << ", again: " << again
            << '\n';  // 3 6 6
  // Does not compile, for each of the three: lvalue required as unary '&' operand
  // int* a{&3};  int* b{&(count * 2)};  int* c{&twice(count)};
  std::cout << "3: " << CATEGORY(3) << '\n';                        // prvalue
  std::cout << "count * 2: " << CATEGORY(count * 2) << '\n';        // prvalue
  std::cout << "twice(count): " << CATEGORY(twice(count)) << '\n';  // prvalue
}
}  // namespace prvalue_more

// [Appendix xxiii] Ways to Mark
namespace xvalue_ways {
struct Point {
  int x;
  int y;
};

void run() {
  int count{3};
  Point pt{1, 2};
  // Does not compile, for each: taking address of rvalue
  // int* a{&std::move(count)};
  std::cout << "std::move(count): " << CATEGORY(std::move(count)) << '\n';                    // xvalue
  std::cout << "static_cast<int&&>(count): " << CATEGORY(static_cast<int&&>(count)) << '\n';  // xvalue
  std::cout << "std::move(pt).x: " << CATEGORY(std::move(pt).x) << '\n';                      // xvalue
}
}  // namespace xvalue_ways

// [Appendix xxiv] Giving It a Name
namespace named_rvalue_ref {
void run() {
  std::vector<Pose>&& r{make_path(10)};
  std::cout << "decltype(r) is std::vector<Pose>&&: " << std::boolalpha
            << std::is_same_v<decltype(r), std::vector<Pose>&&> << std::noboolalpha
            << '\n';                              // true: its type
  std::cout << "r: " << CATEGORY(r) << '\n';      // lvalue: it has a name
}
}  // namespace named_rvalue_ref

// [Appendix xxv] std::move on a const Object
namespace move_const {
void run() {
  const std::vector<Pose> path{make_path(1000)};
  std::vector<Pose> controller_path{std::move(path)};  // compiles: a copy
  std::cout << std::boolalpha << "controller_path.data() == path.data(): "
            << (controller_path.data() == path.data()) << std::noboolalpha
            << ", path.size(): " << path.size() << '\n';  // false 1000
}
}  // namespace move_const

// [Appendix xxvi] release
namespace release {
void run() {
  auto track{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  Obstacle* raw{track.release()};  // gives up ownership, deletes nothing
  std::cout << std::boolalpha << "track == nullptr: " << (track == nullptr) << std::noboolalpha
            << ", raw->id: " << raw->id << '\n';  // true 1
  delete raw;  // you are the owner now: leave this out and it leaks
}
}  // namespace release

// [Appendix xxvii] Passing: Reseat
namespace reseat {
void replace_track(std::unique_ptr<Obstacle>& track, int new_id) {
  track = std::make_unique<Obstacle>(new_id);
}  // the assignment deleted the old Obstacle object

void run() {
  auto track{std::make_unique<Obstacle>(1, Pose{20.0, -3.0}, 0.0, true)};
  replace_track(track, 7);
  std::cout << "track->id: " << track->id << ", track->speed: " << track->speed
            << '\n';  // 7 0
}
}  // namespace reseat

// [Appendix xxix] A Parameter the Function Moves From
// Core Guidelines F.18: when a function always moves from a parameter, take it
// by X&& and std::move it. The caller must then say std::move for an lvalue,
// so a copy never happens by accident.
namespace will_move_from {
struct Controller {
  std::vector<Pose> path;  // the part that steers
};

void set_path(Controller& controller, std::vector<Pose>&& path) {
  controller.path = std::move(path);  // path has a name: it is an lvalue, so move it
}

void run() {
  Controller controller{};
  std::vector<Pose> path{make_path(1000)};
  const Pose* where{path.data()};  // where path's block is
  // Does not compile: cannot bind rvalue reference of type 'std::vector<Pose>&&'
  //                   to lvalue of type 'std::vector<Pose>'
  // set_path(controller, path);
  set_path(controller, std::move(path));  // the caller says: take it
  std::cout << std::boolalpha << "controller.path.data() == where: "
            << (controller.path.data() == where) << std::noboolalpha
            << ", path.size(): " << path.size() << '\n';  // true 0
  set_path(controller, make_path(5));     // a prvalue binds with no std::move
  std::cout << "controller.path.size(): " << controller.path.size() << '\n';  // 5
}
}  // namespace will_move_from

// [Appendix xxx] Returning a Local
namespace return_by_value {
std::vector<Pose> plan_path(int n) {
  std::vector<Pose> path{make_path(n)};
  std::cout << "  inside plan_path, path.data(): " << path.data() << '\n';
  return path;  // elided, or moved: never copied (Lecture 5)
  // Warns: moving a local object in a return statement prevents copy elision
  //        [-Wpessimizing-move]
  // return std::move(path);
}

void run() {
  std::vector<Pose> path{plan_path(1000)};
  std::cout << "  in the caller, path.data(): " << path.data() << '\n';  // the same address
}
}  // namespace return_by_value

#undef CATEGORY

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (2 is ii), its
  // title, and the function that runs it.
  const std::vector<Slide> slides{
      {2, "Double Delete, Measured", undefined::double_delete::run, true},
      {3, "Dangling Read, Measured", undefined::dangling_read::run, true},
      {4, "make_path", make_path_frame::run},
      {5, "The Moved-from std::string", moved_string::run},
      {6, "Move Assignment", move_assignment::run},
      {7, "std::exchange", exchange::run},
      {8, "The Definition of std::move", move_written::run},
      {9, "A unique_ptr with Its Own Deleter", own_deleter::run},
      {11, "Testing for Empty", testing_empty::run},
      {12, "unique_ptr to an Array", unique_array::run},
      {13, "swap", swapping::run},
      {15, "A Dashboard of weak_ptr", dashboard::run},
      {21, "A Reference, an Element, *p", lvalue_more::run},
      {22, "Literals, Results, Calls", prvalue_more::run},
      {23, "Ways to Mark", xvalue_ways::run},
      {24, "Giving It a Name", named_rvalue_ref::run},
      {25, "std::move on a const Object", move_const::run},
      {26, "release", release::run},
      {27, "Passing: Reseat", reseat::run},
      {29, "A Parameter the Function Moves From", will_move_from::run},
      {30, "Returning a Local", return_by_value::run},
  };
  return run_slides(slides, argc, argv, Part::appendix);
}
