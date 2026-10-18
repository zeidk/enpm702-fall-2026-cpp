/**
 * @file moves.cpp
 * @brief L7 Section 2, Move Semantics: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 12    # only [Slide 12]
 * @endcode
 *
 * The value category slides print the category of each expression with
 * CATEGORY(e), which reads decltype((e)): T& for an lvalue, T&& for an xvalue,
 * T for a prvalue ([Slide 31]). Code that does not compile, or that warns, is
 * commented out where its slide shows it: uncomment it, build, and you get the
 * slide's message.
 */
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "fleet.hpp"
#include "sections.hpp"

namespace moves {

// decltype((e)) is T& for an lvalue, T&& for an xvalue and T for a prvalue.
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

// A route of n waypoints, one meter apart along an aisle.
std::vector<Position> make_route(int n) {
  std::vector<Position> route;
  for (int i{0}; i < n; ++i) {
    route.push_back({static_cast<double>(i), 0.0});
  }
  return route;
}

RobotStatus make_robot(int id) { return {id, 100.0, {0.0, 0.0}, false}; }

// [Slide 12] Handing a Route to a Robot
namespace handing_route {
void run() {
  std::vector<Position> route{make_route(1000)};
  const Position* where{route.data()};

  std::vector<Position> copied{route};             // a copy
  std::cout << "copy: copied.data() == route.data(): " << std::boolalpha
            << (copied.data() == route.data()) << ", route.size(): " << route.size()
            << '\n';                                // false 1000

  std::vector<Position> moved{std::move(route)};   // a move
  std::cout << "move: moved.data() == where: " << (moved.data() == where)
            << ", route.size(): " << route.size() << std::noboolalpha << '\n';  // true 0
  std::cout << "bytes the copy allocated: " << copied.capacity() * sizeof(Position)
            << '\n';                                // 16000
}
}  // namespace handing_route

// [Slide 13] An Implicit Move
namespace implicit_move {
const Position* made{nullptr};          // where plan's block is made

std::vector<Position> plan(int n) {     // a custom function
  std::vector<Position> route{make_route(n)};
  made = route.data();
  return route;                         // no std::move
}

void run() {
  std::vector<std::vector<Position>> plans;
  plans.push_back(plan(500));           // no std::move
  std::cout << "plans[0].data() == made: " << std::boolalpha << (plans[0].data() == made)
            << std::noboolalpha << ", plans[0].size(): " << plans[0].size() << '\n';  // true 500
}
}  // namespace implicit_move

// [Slide 16] The Moved-from Object
namespace moved_from {
void run() {
  std::vector<Position> route{make_route(1000)};
  std::vector<Position> robot_route{std::move(route)};
  std::cout << std::boolalpha << "after the move, route.empty(): " << route.empty()
            << std::noboolalpha << '\n';  // true with libstdc++; the standard says only
                                         // "valid but unspecified" [lib.types.movedfrom]
  route = make_route(5);                  // assign before you use it again
  std::cout << "after assignment, route.size(): " << route.size()
            << ", robot_route.size(): " << robot_route.size() << '\n';  // 5 1000
}
}  // namespace moved_from

// [Slide 19] lvalues, Step 1: A Name for an Object
namespace lvalue_name {
void run() {
  int count{3};        // makes an int object that holds 3
  count = 4;           // the same object now holds 4
  int* where{&count};  // where holds that object's address
  std::cout << std::boolalpha << "count: " << count << ", where == &count: " << (where == &count)
            << std::noboolalpha << '\n';  // 4 true
}
}  // namespace lvalue_name

// [Slide 20] lvalues, Step 2: The & Test
namespace lvalue_test {
void run() {
  int count{3};
  int* a{&count};  // compiles: count names an object
  // Does not compile: lvalue required as unary '&' operand
  // int* b{&(count + 1)};
  std::cout << std::boolalpha << "a == &count: " << (a == &count) << std::noboolalpha
            << ", count + 1: " << CATEGORY(count + 1) << '\n';  // true prvalue
}
}  // namespace lvalue_test

// [Slide 21] lvalues, Step 3: Three More, with Why
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

// [Slide 22] lvalues, Step 4: In the Fleet Manager
namespace lvalues {
RobotStatus& first_robot(std::vector<RobotStatus>& fleet) { return fleet[0]; }

void run() {
  std::vector<Position> route{make_route(3)};
  std::vector<RobotStatus> fleet{make_robot(1), make_robot(2)};
  RobotStatus* robot_ptr{&fleet[1]};
  std::cout << "route: " << CATEGORY(route) << '\n';                           // lvalue
  std::cout << "*robot_ptr: " << CATEGORY(*robot_ptr) << '\n';                 // lvalue
  std::cout << "fleet[0]: " << CATEGORY(fleet[0]) << '\n';                     // lvalue
  std::cout << "first_robot(fleet): " << CATEGORY(first_robot(fleet)) << '\n'; // lvalue
  std::cout << "\"dock\": " << CATEGORY("dock") << '\n';                       // lvalue
  std::cout << "&fleet[0] works: " << (&fleet[0] != nullptr) << '\n';          // 1
}
}  // namespace lvalues

// [Slide 23] prvalues, Step 1: A Value, Not an Object
namespace prvalue_value {
void run() {
  int count{3};
  int next{count + 1};  // the 4 initializes next
  std::cout << "next: " << next << ", count: " << count << '\n';  // 4 3
  std::cout << "count + 1: " << CATEGORY(count + 1) << '\n';    // prvalue
}
}  // namespace prvalue_value

// [Slide 24] prvalues, Step 2: Three More, with Why
namespace prvalue_more {
int twice(int n) { return 2 * n; }  // returns an int by value

void run() {
  int count{3};
  // Does not compile, for each of the three: lvalue required as unary '&' operand
  // int* a{&42};  int* b{&(count * 2)};  int* c{&twice(count)};
  std::cout << "42: " << CATEGORY(42) << '\n';                      // prvalue
  std::cout << "count * 2: " << CATEGORY(count * 2) << '\n';        // prvalue
  std::cout << "twice(count): " << CATEGORY(twice(count)) << '\n';  // prvalue
}
}  // namespace prvalue_more

// [Slide 25] prvalues, Step 3: In the Fleet Manager
namespace prvalues {
void run() {
  int a{2};
  int b{3};
  std::cout << "42: " << CATEGORY(42) << '\n';                             // prvalue
  std::cout << "a + b: " << CATEGORY(a + b) << '\n';                       // prvalue
  std::cout << "make_route(3): " << CATEGORY(make_route(3)) << '\n';       // prvalue
  std::cout << "RobotStatus{}: " << CATEGORY(RobotStatus{}) << '\n';       // prvalue
  std::cout << "[] { return 1; }: " << CATEGORY([] { return 1; }) << '\n'; // prvalue
  // Does not compile: lvalue required as unary '&' operand
  // std::cout << &(a + b);
}
}  // namespace prvalues

// [Slide 26] xvalues, Step 1: Marked for Taking
namespace xvalue_marked {
void run() {
  std::string name{"charging dock, aisle 4"};  // 22 chars: on the heap
  const char* chars{name.data()};              // their address
  std::string other{std::move(name)};
  std::cout << std::boolalpha << "other: " << other << ", name.size(): " << name.size()
            << ", other.data() == chars: " << (other.data() == chars) << std::noboolalpha
            << '\n';  // charging dock, aisle 4, 0, true (libstdc++)
}
}  // namespace xvalue_marked

// [Slide 27] xvalues, Step 2: Three Ways to Mark
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

// [Slide 28] xvalues, Step 3: In the Fleet Manager
namespace xvalues {
struct Robot {
  RobotStatus status;
  std::vector<Position> route;
};

void run() {
  std::vector<Position> route{make_route(3)};
  Robot robot{make_robot(1), make_route(3)};
  std::cout << "std::move(route): " << CATEGORY(std::move(route)) << '\n';  // xvalue
  std::cout << "static_cast<std::vector<Position>&&>(route): "
            << CATEGORY(static_cast<std::vector<Position>&&>(route)) << '\n';  // xvalue
  std::cout << "std::move(robot).route: " << CATEGORY(std::move(robot).route)
            << '\n';                                                         // xvalue
  std::cout << "route.size(): " << route.size() << '\n';  // 3: naming an xvalue moves nothing
}
}  // namespace xvalues

// [Slide 29] Temporary Materialization
namespace materialization {
void run() {
  std::cout << "make_robot(3): " << CATEGORY(make_robot(3)) << '\n';        // prvalue
  std::cout << "make_robot(3).id: " << CATEGORY(make_robot(3).id) << '\n';  // xvalue
  int id{make_robot(3).id};  // the robot is created, read, and destroyed
  std::cout << "id: " << id << '\n';  // 3
}
}  // namespace materialization

// [Slide 31] The decltype Test
namespace testing_category {
std::vector<Position> route{make_route(3)};
static_assert(std::is_same_v<decltype((route)), std::vector<Position>&>);              // lvalue
static_assert(std::is_same_v<decltype((std::move(route))), std::vector<Position>&&>);  // xvalue
static_assert(std::is_same_v<decltype((make_route(3))), std::vector<Position>>);       // prvalue

void run() {
  std::cout << "the three static_asserts compiled\n";
}
}  // namespace testing_category

// [Slide 32] Exercise 1: Name the Category
namespace exercise_1 {
struct Robot {
  RobotStatus status;
  std::vector<Position> route;
};

Robot& lookup(std::vector<Robot>& robots, int i) { return robots[i]; }

void run() {
  std::vector<Robot> robots{{make_robot(1), make_route(2)}, {make_robot(2), make_route(4)}};
  Robot* p{&robots[0]};
  int n{4};
  // Write your answer for each line before you run it.
  std::cout << " 1. robots: " << CATEGORY(robots) << '\n';
  std::cout << " 2. robots[1].route: " << CATEGORY(robots[1].route) << '\n';
  std::cout << " 3. p->status.id: " << CATEGORY(p->status.id) << '\n';
  std::cout << " 4. n * 2: " << CATEGORY(n * 2) << '\n';
  std::cout << " 5. lookup(robots, 1): " << CATEGORY(lookup(robots, 1)) << '\n';
  std::cout << " 6. make_route(n): " << CATEGORY(make_route(n)) << '\n';
  std::cout << " 7. std::move(robots[0]): " << CATEGORY(std::move(robots[0])) << '\n';
  std::cout << " 8. std::move(robots[0]).route: " << CATEGORY(std::move(robots[0]).route)
            << '\n';
  std::cout << " 9. make_robot(n).battery_pct: " << CATEGORY(make_robot(n).battery_pct)
            << '\n';
  std::cout << "10. ++n: " << CATEGORY(++n) << '\n';
}
}  // namespace exercise_1

// [Slide 33] rvalue References
namespace rvalue_references {
void run() {
  std::vector<Position> route{make_route(1000)};
  std::vector<Position>&& temp_ref{make_route(10)};  // OK: binds to an rvalue
  const std::vector<Position>& any_ref{route};       // const T& binds to both (Lecture 5)
  // Does not compile: cannot bind rvalue reference of type
  // 'std::vector<Position>&&' to lvalue of type 'std::vector<Position>'
  // std::vector<Position>&& lvalue_ref{route};
  std::cout << "temp_ref.size(): " << temp_ref.size() << ", any_ref.size(): " << any_ref.size()
            << '\n';  // 10 1000
}
}  // namespace rvalue_references

// [Slide 34] Named rvalue References
namespace named_rvalue_ref {
void run() {
  std::vector<Position>&& r{make_route(10)};
  std::cout << "decltype(r) is std::vector<Position>&&: " << std::boolalpha
            << std::is_same_v<decltype(r), std::vector<Position>&&> << std::noboolalpha
            << '\n';                              // true: its type
  std::cout << "r: " << CATEGORY(r) << '\n';      // lvalue: it has a name
}
}  // namespace named_rvalue_ref

// [Slide 35] Overloads for Copy and Move
namespace pick_copy_or_move {
std::vector<Position> robot_route;

void assign_route(const std::vector<Position>& route) {
  std::cout << "  const& overload: copy\n";
  robot_route = route;
}
void assign_route(std::vector<Position>&& route) {
  std::cout << "  && overload: move\n";
  robot_route = std::move(route);
}

void run() {
  std::vector<Position> route{make_route(1000)};
  std::vector<Position>&& r{make_route(10)};
  std::cout << "assign_route(route):\n";
  assign_route(route);               // lvalue: copy
  std::cout << "assign_route(make_route(5)):\n";
  assign_route(make_route(5));       // prvalue: move
  std::cout << "assign_route(r):\n";
  assign_route(r);                   // r is an lvalue: copy
  std::cout << "assign_route(std::move(route)):\n";
  assign_route(std::move(route));    // xvalue: move
}
}  // namespace pick_copy_or_move

// [Slide 36] std::move, a Cast
namespace move_moves_nothing {
void run() {
  std::vector<Position> route{make_route(1000)};
  // Warns: ignoring return value of 'constexpr typename std::remove_reference<_Tp>::type&&
  //        std::move(_Tp&&) [with _Tp = vector<Position>&; typename
  //        remove_reference<_Tp>::type = vector<Position>]', declared with attribute
  //        'nodiscard' [-Wunused-result]
  // std::move(route);
  static_cast<void>(std::move(route));  // the cast, with its result thrown away
  std::cout << "route.size(): " << route.size() << '\n';  // 1000: nothing moved
}
}  // namespace move_moves_nothing

// [Slide 37] std::move on a const Object
namespace move_const {
void run() {
  const std::vector<Position> route{make_route(1000)};
  std::vector<Position> robot_route{std::move(route)};  // compiles: a copy
  std::cout << std::boolalpha << "robot_route.data() == route.data(): "
            << (robot_route.data() == route.data()) << std::noboolalpha
            << ", route.size(): " << route.size() << '\n';  // false 1000
}
}  // namespace move_const

// [Slide 38] A Parameter the Function Moves From
// Core Guidelines F.18: when a function always moves from a parameter, take it
// by X&& and std::move it. The caller must then say std::move for an lvalue,
// so a copy never happens by accident.
namespace will_move_from {
struct Robot {
  RobotStatus status;
  std::vector<Position> route;
};

void set_route(Robot& robot, std::vector<Position>&& route) {
  robot.route = std::move(route);  // route has a name: it is an lvalue, so move it
}

void run() {
  Robot robot{make_robot(3), {}};
  std::vector<Position> route{make_route(1000)};
  const Position* where{route.data()};
  // Does not compile: cannot bind rvalue reference of type 'std::vector<Position>&&'
  //                   to lvalue of type 'std::vector<Position>'
  // set_route(robot, route);
  set_route(robot, std::move(route));  // the caller says: take it
  std::cout << std::boolalpha << "robot.route.data() == where: " << (robot.route.data() == where)
            << std::noboolalpha << ", route.size(): " << route.size() << '\n';  // true 0
  set_route(robot, make_route(5));     // a prvalue binds with no std::move
  std::cout << "robot.route.size(): " << robot.route.size() << '\n';  // 5
}
}  // namespace will_move_from

// [Slide 39] Return by Value, Again
namespace return_by_value {
std::vector<Position> plan_route(int n) {
  std::vector<Position> route{make_route(n)};
  std::cout << "  inside plan_route, route.data(): " << route.data() << '\n';
  return route;  // elided, or moved: never copied (Lecture 5)
  // Warns: moving a local object in a return statement prevents copy elision
  //        [-Wpessimizing-move]
  // return std::move(route);
}

void run() {
  std::vector<Position> path{plan_route(1000)};
  std::cout << "  in the caller, path.data(): " << path.data() << '\n';  // the same address
}
}  // namespace return_by_value

#undef CATEGORY

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {12, "Handing a Route to a Robot", handing_route::run},
      {13, "An Implicit Move", implicit_move::run},
      {16, "The Moved-from Object", moved_from::run},
      {19, "lvalues, Step 1: A Name for an Object", lvalue_name::run},
      {20, "lvalues, Step 2: The & Test", lvalue_test::run},
      {21, "lvalues, Step 3: Three More, with Why", lvalue_more::run},
      {22, "lvalues, Step 4: In the Fleet Manager", lvalues::run},
      {23, "prvalues, Step 1: A Value, Not an Object", prvalue_value::run},
      {24, "prvalues, Step 2: Three More, with Why", prvalue_more::run},
      {25, "prvalues, Step 3: In the Fleet Manager", prvalues::run},
      {26, "xvalues, Step 1: Marked for Taking", xvalue_marked::run},
      {27, "xvalues, Step 2: Three Ways to Mark", xvalue_ways::run},
      {28, "xvalues, Step 3: In the Fleet Manager", xvalues::run},
      {29, "Temporary Materialization", materialization::run},
      {31, "The decltype Test", testing_category::run},
      {32, "Exercise 1: Value Categories", exercise_1::run},
      {33, "rvalue References", rvalue_references::run},
      {34, "Named rvalue References", named_rvalue_ref::run},
      {35, "Overloads for Copy and Move", pick_copy_or_move::run},
      {36, "std::move, a Cast", move_moves_nothing::run},
      {37, "std::move on a const Object", move_const::run},
      {38, "A Parameter the Function Moves From", will_move_from::run},
      {39, "Return by Value, Again", return_by_value::run},
  };
}

}  // namespace moves
