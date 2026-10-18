/**
 * @file unique.cpp
 * @brief L7 Section 3, Smart Pointers, std::unique_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 48    # only [Slide 48]
 * @endcode
 *
 * Code that does not compile is commented out where its slide shows it:
 * uncomment it, build, and you get the slide's error.
 */
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "fleet.hpp"
#include "sections.hpp"

namespace unique {

// [Slide 47] The Syntax
// 1 and 2 declare the same type; 3 compiles too, but writes new (R.23, slide 48).
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

// [Slide 48] Creating One
namespace creating {
void run() {
  std::unique_ptr<RobotStatus> robot{
      std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  std::cout << "robot->id: " << robot->id << ", (*robot).battery_pct: " << (*robot).battery_pct
            << '\n';  // 3 64
  auto other{std::make_unique<RobotStatus>()};  // every member takes its default
  std::cout << "other->battery_pct: " << other->battery_pct << '\n';  // 100
}
}  // namespace creating

// [Slide 49] In Memory
namespace in_memory {
void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  std::cout << "sizeof(robot): " << sizeof(robot)
            << ", sizeof(RobotStatus*): " << sizeof(RobotStatus*)
            << ", sizeof(RobotStatus): " << sizeof(RobotStatus) << '\n';  // 8 8 40
  std::cout << "&robot: " << &robot << " (stack), robot.get(): " << robot.get()
            << " (heap)\n";
}
}  // namespace in_memory

// [Slide 50] The Leak, Fixed
namespace leak_fixed {
bool needs_charge(int id) {
  auto robot{std::make_unique<RobotStatus>(id, 64.0, Position{0.0, 0.0}, false)};
  if (robot->battery_pct > 20.0) {
    return false;  // robot is destroyed here, and the RobotStatus with it
  }
  return true;
}

void run() {
  std::cout << std::boolalpha << "needs_charge(3): " << needs_charge(3) << std::noboolalpha
            << '\n';  // false, and LeakSanitizer reports nothing at exit
}
}  // namespace leak_fixed

// [Slide 51] One Owner Only
namespace one_owner {
void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  // Does not compile: use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const
  //                   std::unique_ptr<_Tp, _Dp>&) [with _Tp = RobotStatus; _Dp =
  //                   std::default_delete<RobotStatus>]'
  // std::unique_ptr<RobotStatus> copy{robot};
  std::cout << "robot->id: " << robot->id << '\n';  // 3
}
}  // namespace one_owner

// [Slide 52] Moving Ownership
namespace moving_ownership {
void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  RobotStatus* where{robot.get()};
  std::unique_ptr<RobotStatus> other{std::move(robot)};
  std::cout << std::boolalpha << "robot == nullptr: " << (robot == nullptr)
            << ", other.get() == where: " << (other.get() == where) << std::noboolalpha
            << ", other->id: " << other->id << '\n';  // true true 3
}
}  // namespace moving_ownership

// [Slide 53] A Fleet of unique_ptr
namespace fleet_of_unique {
void run() {
  std::vector<std::unique_ptr<RobotStatus>> fleet;
  fleet.push_back(std::make_unique<RobotStatus>(1, 82.5, Position{0.0, 0.0}, false));
  auto robot{std::make_unique<RobotStatus>(2, 35.0, Position{4.0, 1.0}, true)};
  fleet.push_back(std::move(robot));  // robot is now empty
  // Does not compile: no matching function for call to 'construct_at(...)', then
  //                   use of deleted function 'std::unique_ptr<...>::unique_ptr(const ...&)'
  // fleet.push_back(robot);
  for (const auto& r : fleet) {  // by reference: a copy would not compile
    std::cout << "robot " << r->id << ": " << r->battery_pct << " %\n";
  }
  // Does not compile either: copying the vector copies every unique_ptr.
  //   error: static assertion failed: result type must be constructible from input type
  // std::vector<std::unique_ptr<RobotStatus>> backup{fleet};
}
}  // namespace fleet_of_unique

// [Slide 54] get and reset
namespace get_and_reset {
void print_status(const RobotStatus* robot) {  // only looks
  std::cout << "  robot " << robot->id << ": " << robot->battery_pct << " %\n";
}

void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  print_status(robot.get());                   // robot still owns it
  robot.reset();                               // deletes the RobotStatus now
  std::cout << std::boolalpha << "after reset(), robot == nullptr: " << (robot == nullptr)
            << std::noboolalpha << '\n';       // true
  robot = std::make_unique<RobotStatus>(4, 18.0, Position{6.0, 2.0}, false);  // a new robot
  print_status(robot.get());
}
}  // namespace get_and_reset

// [Slide 55] release
namespace release {
void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  RobotStatus* raw{robot.release()};  // robot gives up ownership, deletes nothing
  std::cout << std::boolalpha << "robot == nullptr: " << (robot == nullptr) << std::noboolalpha
            << ", raw->id: " << raw->id << '\n';  // true 3
  delete raw;  // now you are the owner: leave this out and it leaks
}
}  // namespace release

// [Slide 56] Passing: Borrow
namespace borrow {
void print_status(const RobotStatus& robot) {  // only reads the robot
  std::cout << "  robot " << robot.id << ": " << robot.battery_pct << " %\n";
}
void charge(RobotStatus* robot) {  // changes the robot, may receive nullptr
  if (robot != nullptr) { robot->battery_pct = 100.0; }
}

void run() {
  auto robot{std::make_unique<RobotStatus>(3, 64.0, Position{2.0, 3.0}, false)};
  print_status(*robot);
  charge(robot.get());
  print_status(*robot);  // 100: the caller still owns it
}
}  // namespace borrow

// [Slide 57] Passing: Sink
namespace sink {
std::vector<std::unique_ptr<RobotStatus>> retired;

void retire(std::unique_ptr<RobotStatus> robot) {  // takes ownership
  retired.push_back(std::move(robot));
}

void run() {
  auto robot{std::make_unique<RobotStatus>(3, 4.0, Position{2.0, 3.0}, false)};
  // Does not compile: use of deleted function 'std::unique_ptr<...>::unique_ptr(const ...&)'
  // retire(robot);
  retire(std::move(robot));
  std::cout << std::boolalpha << "robot == nullptr: " << (robot == nullptr) << std::noboolalpha
            << ", retired.size(): " << retired.size() << '\n';  // true 1
}
}  // namespace sink

// [Slide 58] Passing: Reseat
namespace reseat {
void replace_robot(std::unique_ptr<RobotStatus>& robot, int new_id) {
  robot = std::make_unique<RobotStatus>(new_id, 100.0, Position{0.0, 0.0}, false);
}  // the old robot was deleted by the assignment

void run() {
  auto robot{std::make_unique<RobotStatus>(3, 4.0, Position{2.0, 3.0}, false)};
  replace_robot(robot, 7);
  std::cout << "robot->id: " << robot->id << ", robot->battery_pct: " << robot->battery_pct
            << '\n';  // 7 100
}
}  // namespace reseat

// [Slide 59] Returning
namespace returning {
std::unique_ptr<RobotStatus> make_robot(int id) {
  auto robot{std::make_unique<RobotStatus>(id, 100.0, Position{0.0, 0.0}, false)};
  robot->busy = false;
  return robot;  // no std::move: the implicit move of Lecture 5
}

void run() {
  std::unique_ptr<RobotStatus> robot{make_robot(5)};
  std::cout << "robot->id: " << robot->id << '\n';  // 5
}
}  // namespace returning

// [Slide 60] Exercise 2: Lines That Compile
namespace exercise_2 {
void inspect(const RobotStatus& robot) { std::cout << "  inspect: robot " << robot.id << '\n'; }
void adopt(std::unique_ptr<RobotStatus> robot) { std::cout << "  adopt: robot " << robot->id << '\n'; }

void run() {
  auto a{std::make_unique<RobotStatus>(1, 50.0, Position{0.0, 0.0}, false)};
  auto b{std::make_unique<RobotStatus>(2, 60.0, Position{0.0, 0.0}, false)};
  // For each line, say whether it compiles. The lines that do not compile are
  // commented out: uncomment one to check your answer.
  // auto c{a};                                    // line 1
  auto d{std::move(a)};                            // line 2
  inspect(*d);                                     // line 3
  // adopt(b);                                     // line 4
  adopt(std::move(b));                             // line 5
  std::unique_ptr<RobotStatus> e{new RobotStatus{}};    // line 6, see the note below
  RobotStatus* f{d.get()};                         // line 7
  // std::unique_ptr<RobotStatus> g{f};            // line 8, see the note below
  std::cout << "  f->id: " << f->id << ", e->id: " << e->id << '\n';
  // Line 6 compiles and is correct, but skips make_unique (R.23). Line 8
  // compiles too, and is a bug: d and g would both delete the same robot, so
  // it stays commented out.
}
}  // namespace exercise_2

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {47, "The Syntax", syntax::run},
      {48, "Creating One", creating::run},
      {49, "In Memory", in_memory::run},
      {50, "The Leak, Fixed", leak_fixed::run},
      {51, "One Owner Only", one_owner::run},
      {52, "Moving Ownership", moving_ownership::run},
      {53, "A Fleet of unique_ptr", fleet_of_unique::run},
      {54, "get and reset", get_and_reset::run},
      {55, "release", release::run},
      {56, "Passing: Borrow", borrow::run},
      {57, "Passing: Sink", sink::run},
      {58, "Passing: Reseat", reseat::run},
      {59, "Returning", returning::run},
      {60, "Exercise 2: Lines That Compile", exercise_2::run},
  };
}

}  // namespace unique
