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
 * table in main() lists them, and run_slides() in @c ../common/slides.cpp runs
 * them. Code with undefined behavior is in @c ../undefined/undefined.cpp,
 * built with the sanitizers; a full run skips it.
 */
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "fleet.hpp"
#include "slides.hpp"
#include "undefined.hpp"

namespace {
// A route of n waypoints, one meter apart along an aisle.
std::vector<Position> make_route(int n) {
  std::vector<Position> route;
  for (int i{0}; i < n; ++i) {
    route.push_back({static_cast<double>(i), 0.0});
  }
  return route;
}
}  // namespace

// [Appendix ii] Double Delete, Measured: undefined::double_delete
// [Appendix iii] Dangling Read, Measured: undefined::dangling_read

// [Appendix iv] make_route
namespace make_route_frame {
void run() {
  std::vector<Position> route{make_route(3)};
  for (const Position& p : route) {
    std::cout << '(' << p.x << ", " << p.y << ") ";
  }
  std::cout << '\n';  // (0, 0) (1, 0) (2, 0)
}
}  // namespace make_route_frame

// [Appendix v] The Moved-from std::string
namespace moved_string {
void run() {
  std::string note{"robot 3 waits at the charging dock"};
  std::string name{"robot 3"};
  std::string taken_note{std::move(note)};
  std::string taken_name{std::move(name)};
  std::cout << "note.size(): " << note.size() << ", name.size(): " << name.size()
            << '\n';  // 0 0 with libstdc++; the standard does not promise it
  note = "robot 4 is charging";  // assigning gives it a known value again
  std::cout << "note: " << note << '\n';
}
}  // namespace moved_string

// [Appendix vi] Move Assignment
namespace move_assignment {
void run() {
  std::vector<Position> current{make_route(3)};
  std::vector<Position> next{make_route(1000)};
  const Position* where{next.data()};
  current = std::move(next);  // current's 3 waypoints are destroyed
  std::cout << std::boolalpha << "current.size(): " << current.size()
            << ", current.data() == where: " << (current.data() == where)
            << ", next.size(): " << next.size() << std::noboolalpha
            << '\n';  // 1000 true 0 (libstdc++)
}
}  // namespace move_assignment

// [Appendix vii] std::exchange
namespace exchange {
void run() {
  std::vector<Position> route{make_route(1000)};
  // take the route, and leave an empty one behind
  std::vector<Position> done{std::exchange(route, {})};
  std::cout << "done.size(): " << done.size() << ", route.size(): " << route.size()
            << '\n';  // 1000 0, guaranteed
  int task_id{7};
  int finished{std::exchange(task_id, 0)};
  std::cout << "finished: " << finished << ", task_id: " << task_id << '\n';  // 7 0
}
}  // namespace exchange

// [Appendix viii] How std::move Is Written
namespace move_written {
// The standard's std::move, under another name ([forward] para 5).
template <typename T>
constexpr std::remove_reference_t<T>&& my_move(T&& t) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(t);
}

void run() {
  std::vector<Position> route{make_route(1000)};
  static_assert(std::is_same_v<decltype(my_move(route)), std::vector<Position>&&>);
  static_assert(std::is_same_v<decltype(my_move(route)), decltype(std::move(route))>);
  std::vector<Position> taken{my_move(route)};
  std::cout << "taken.size(): " << taken.size() << ", route.size(): " << route.size()
            << '\n';  // 1000 0
}
}  // namespace move_written

// [Appendix ix] A unique_ptr with Its Own Deleter
namespace own_deleter {
void run() {
  auto say_and_delete{[](RobotStatus* robot) {
    std::cout << "  deleting robot " << robot->id << '\n';
    delete robot;
  }};
  {
    std::unique_ptr<RobotStatus, decltype(say_and_delete)> robot{
        new RobotStatus{3, 64.0}, say_and_delete};
    std::cout << "sizeof(robot): " << sizeof(robot) << '\n';  // 8
  }  // robot is destroyed here: "deleting robot 3"
  std::cout << "after the block\n";
  std::cout << "with a function pointer: "
            << sizeof(std::unique_ptr<RobotStatus, void (*)(RobotStatus*)>) << '\n';  // 16
}
}  // namespace own_deleter

// [Appendix x] std::auto_ptr
// No code: std::auto_ptr is not part of C++17 or C++20. g++ 13.3 still
// accepts it with -std=c++20, with a -Wdeprecated-declarations warning, and
// after std::auto_ptr<RobotStatus> b{a}; the pointer a is null.

// [Appendix xi] Testing for Empty
namespace testing_empty {
void run() {
  std::unique_ptr<RobotStatus> robot;  // empty
  std::cout << std::boolalpha << "robot == nullptr: " << (robot == nullptr) << '\n';  // true
  robot = std::make_unique<RobotStatus>(3, 64.0);
  if (robot) {
    std::cout << "robot " << robot->id << '\n';  // robot 3
  }
  auto other{std::move(robot)};
  std::cout << "after the move, !robot: " << !robot << std::noboolalpha << '\n';  // true
  // Does not compile: cannot convert 'std::unique_ptr<RobotStatus,
  //                   std::default_delete<RobotStatus> >' to 'bool' in initialization
  // bool has_robot = other;
}
}  // namespace testing_empty

// [Appendix xii] unique_ptr to an Array
namespace unique_array {
void run() {
  auto readings{std::make_unique<double[]>(5)};  // five doubles, each 0.0
  readings[2] = 14.0;
  for (int i{0}; i < 5; ++i) {
    std::cout << readings[i] << ' ';
  }
  std::cout << '\n';  // 0 0 14 0 0
  std::cout << "sizeof(readings): " << sizeof(readings)
            << ", sizeof(std::vector<double>): " << sizeof(std::vector<double>)
            << '\n';  // 8 24
}
}  // namespace unique_array

// [Appendix xiii] swap
namespace swapping {
void run() {
  auto first{std::make_unique<RobotStatus>(3, 64.0)};
  auto second{std::make_unique<RobotStatus>(4, 90.0)};
  const RobotStatus* robot_3{first.get()};
  first.swap(second);  // the same as std::swap(first, second)
  std::cout << std::boolalpha << "first->id: " << first->id << ", second->id: " << second->id
            << ", second.get() == robot_3: " << (second.get() == robot_3) << std::noboolalpha
            << '\n';  // 4 3 true
}
}  // namespace swapping

// [Appendix xiv] make_shared Allocates Once
// Counting allocations needs a replaced global operator new, which clashes
// with the sanitizers this program is built with. The count is its own
// program, allocations.cpp: 702run week7_allocations

// [Appendix xv] A Dashboard of weak_ptr
namespace dashboard {
void run() {
  std::vector<std::shared_ptr<RobotStatus>> fleet;
  fleet.push_back(std::make_shared<RobotStatus>(1, 82.5));
  fleet.push_back(std::make_shared<RobotStatus>(2, 35.0));
  fleet.push_back(std::make_shared<RobotStatus>(3, 14.0));
  std::vector<std::weak_ptr<RobotStatus>> shown;
  for (const auto& robot : fleet) {
    shown.push_back(robot);  // watches, does not own
  }
  fleet.erase(fleet.begin() + 1);  // deletes the RobotStatus with id 2
  for (const auto& watched : shown) {
    if (auto robot{watched.lock()}) {
      std::cout << "  robot " << robot->id << ": " << robot->battery_pct << " %\n";
    } else {
      std::cout << "  (gone)\n";
    }
  }
  std::erase_if(shown, [](const std::weak_ptr<RobotStatus>& w) { return w.expired(); });
  std::cout << "shown.size(): " << shown.size() << '\n';  // 2
}
}  // namespace dashboard

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (2 is ii), its
  // title, and the function that runs it.
  const std::vector<Slide> slides{
      {2, "Double Delete, Measured", undefined::double_delete::run, true},
      {3, "Dangling Read, Measured", undefined::dangling_read::run, true},
      {4, "make_route", make_route_frame::run},
      {5, "The Moved-from std::string", moved_string::run},
      {6, "Move Assignment", move_assignment::run},
      {7, "std::exchange", exchange::run},
      {8, "How std::move Is Written", move_written::run},
      {9, "A unique_ptr with Its Own Deleter", own_deleter::run},
      {11, "Testing for Empty", testing_empty::run},
      {12, "unique_ptr to an Array", unique_array::run},
      {13, "swap", swapping::run},
      {15, "A Dashboard of weak_ptr", dashboard::run},
  };
  return run_slides(slides, argc, argv, Part::appendix);
}
