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
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "obstacle.hpp"
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

// A path of n poses, one meter apart along the road ([Appendix iv]).
const Pose* made_in_make_path{nullptr};  // the block the last make_path allocated

std::vector<Pose> make_path(int n) {
  std::vector<Pose> path;
  path.reserve(n);  // room for n poses: one allocation
  for (int i{0}; i < n; ++i) {
    path.push_back({static_cast<double>(i), 0.0, 0.0});
  }
  made_in_make_path = path.data();
  return path;
}

// An Obstacle by value ([Appendix xx]).
Obstacle make_obstacle(int id) {
  return Obstacle{id};  // the other members take their defaults
}

// [Slide 12] Inside a Copy
// reserve(1024) gives path 24 spare slots (libstdc++ allocates exactly 1024); the
// copy allocates for its 1000 elements only.
namespace inside_copy {
void run() {
  std::vector<Pose> path{make_path(1000)};
  path.reserve(1024);              // room for 24 more
  std::vector<Pose> copied{path};  // a copy
  std::cout << "path: size " << path.size() << ", capacity " << path.capacity() << ", "
            << path.capacity() * sizeof(Pose) << " bytes\n";                // 1000 1024 24576
  std::cout << "copied: size " << copied.size() << ", capacity " << copied.capacity() << ", "
            << copied.capacity() * sizeof(Pose) << " bytes\n";              // 1000 1000 24000
}
}  // namespace inside_copy

// [Slide 13] Inside a Move
namespace inside_move {
void run() {
  std::vector<Pose> path{make_path(1000)};
  const Pose* where{path.data()};            // where path's block is
  std::vector<Pose> moved{std::move(path)};  // std::move marks path
  std::cout << std::boolalpha << "moved.data() == where: " << (moved.data() == where)
            << ", path.size(): " << path.size() << ", path.data() == nullptr: "
            << (path.data() == nullptr) << std::noboolalpha << '\n';  // true 0 true
}
}  // namespace inside_move

// [Slide 14] Elision or a Move
// best is initialized by the prvalue make_scores() (C++20 [dcl.init] 17.6.1) and,
// with NRVO, is the local scores itself: the same two addresses. push_back takes
// a reference, so it gets a temporary and moves it into all[0]: a new vector, the
// same block. With -fno-elide-constructors (no NRVO), best gets a new address too.
namespace elision_or_move {
std::vector<int> make_scores() {
  std::vector<int> scores{90, 75, 82};
  std::cout << "  inside: " << &scores << ' ' << scores.data() << '\n';  // local, block
  return scores;  // no std::move
}

void run() {
  std::cout << "elision:\n";
  std::vector<int> best{make_scores()};  // elision: same two addresses
  std::cout << "  best:   " << &best << ' ' << best.data() << '\n';
  std::cout << "move:\n";
  std::vector<std::vector<int>> all;
  all.push_back(make_scores());  // a move: new vector, same block
  std::cout << "  all[0]: " << &all[0] << ' ' << all[0].data() << '\n';
}
}  // namespace elision_or_move

// [Slide 15] The Moved-from Object
namespace moved_from {
void run() {
  std::vector<Pose> path{make_path(1000)};
  std::vector<Pose> controller_path{std::move(path)};
  std::cout << "after the move, path.empty(): " << path.empty()
            << '\n';                  // 1 (true) with libstdc++; the standard says only
                                      // "valid but unspecified" [lib.types.movedfrom]
  path = make_path(5);                // assign before you use it again
  std::cout << "after assignment: " << path.size() << ' ' << controller_path.size()
            << '\n';                  // 5 1000
}
}  // namespace moved_from

// [Slide 16] Exercise 1: Copy and Move
// After line 4: two blocks on the heap, c holds the first and a holds b's copy;
// b is empty, and nothing was freed (libstdc++, measured).
namespace exercise_copy_move {
void show(const char* name, const std::vector<Pose>& v) {
  std::cout << name << " " << v.size() << "/" << v.capacity() << " at " << v.data();
}

void run() {
  std::vector<Pose> a{make_path(3)};
  std::cout << "1: ", show("a", a), std::cout << '\n';
  std::vector<Pose> b{a};
  std::cout << "2: ", show("a", a), std::cout << ", ", show("b", b), std::cout << '\n';
  std::vector<Pose> c{std::move(a)};
  std::cout << "3: ", show("a", a), std::cout << ", ", show("b", b), std::cout << ", ",
      show("c", c), std::cout << '\n';
  a = std::move(b);
  std::cout << "4: ", show("a", a), std::cout << ", ", show("b", b), std::cout << ", ",
      show("c", c), std::cout << '\n';
  std::cout << "(size/capacity at block; 0 means no block)\n";
}
}  // namespace exercise_copy_move

// [Slide 20] A Named Object
namespace lvalue_name {
void run() {
  int count{3};        // makes an int object that holds 3
  count = 4;           // the same object now holds 4
  int* where{&count};  // where holds that object's address
  std::cout << std::boolalpha << "count: " << count << ", where == &count: " << (where == &count)
            << std::noboolalpha << '\n';  // 4 true
}
}  // namespace lvalue_name

// [Slide 21] Copied, Not Taken
namespace lvalue_copied {
void run() {
  std::vector<int> scores{90, 75, 82};
  std::vector<int> copied{scores};           // scores is an lvalue: copied from
  std::cout << "after line 2: scores.size(): " << scores.size()
            << ", copied.size(): " << copied.size() << '\n';  // 3, 3
  std::vector<int> best{std::move(scores)};  // std::move(scores) marks scores: taken from
  std::cout << "after line 3: scores.size(): " << scores.size()
            << ", best.size(): " << best.size() << '\n';  // 0, 3 (libstdc++)
}
}  // namespace lvalue_copied

// [Slide 22] Checking with &
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

// [Slide 24] A Computed Value
namespace prvalue_value {
void run() {
  int count{3};
  int next{count + 1};  // the 4 initializes next
  std::cout << "next: " << next << ", count: " << count << '\n';  // 4 3
  std::cout << "count + 1: " << CATEGORY(count + 1) << '\n';    // prvalue
  std::vector<Pose> path;
  path = make_path(5);  // path takes the block
  std::cout << "make_path(5): " << CATEGORY(make_path(5)) << '\n';  // prvalue
  std::cout << std::boolalpha << "path took make_path's block: " << (path.data() == made_in_make_path)
            << std::noboolalpha << '\n';  // true
}
}  // namespace prvalue_value

// [Slide 26] Marked by std::move
namespace xvalue_marked {
void run() {
  std::vector<int> scores{90, 75, 82};
  std::vector<int> best{std::move(scores)};
  std::cout << "best.size(): " << best.size() << ", scores.size(): " << scores.size()
            << '\n';  // 3, 0 (libstdc++)
}
}  // namespace xvalue_marked

// [Slide 28] Temporary Materialization
namespace materialization {
void run() {
  std::cout << "make_obstacle(3): " << CATEGORY(make_obstacle(3)) << '\n';        // prvalue
  std::cout << "make_obstacle(3).id: " << CATEGORY(make_obstacle(3).id) << '\n';  // xvalue
  int id{make_obstacle(3).id};  // the Obstacle is created, read, and destroyed
  std::cout << "id: " << id << '\n';  // 3
}
}  // namespace materialization

// [Slide 30] std::is_same_v
namespace same_type {
static_assert(std::is_same_v<int, int>);         // the same type
static_assert(!std::is_same_v<int, int&>);       // int& is another type
static_assert(!std::is_same_v<int, const int>);  // const int is too

void run() {
  std::cout << "the three static_asserts compiled\n";
}
}  // namespace same_type

// [Slide 31] The decltype Test
namespace testing_category {
std::vector<Pose> path{make_path(3)};
static_assert(std::is_same_v<decltype((path)), std::vector<Pose>&>);              // lvalue
static_assert(std::is_same_v<decltype((std::move(path))), std::vector<Pose>&&>);  // xvalue
static_assert(std::is_same_v<decltype((make_path(3))), std::vector<Pose>>);       // prvalue

void run() {
  std::cout << "the three static_asserts compiled\n";
}
}  // namespace testing_category

// [Slide 33] Exercise 2: Value Categories
namespace exercise_categories {
// The Obstacle with this id, or the first one when no id matches.
Obstacle& lookup(std::vector<Obstacle>& seen, int id) {
  for (Obstacle& obstacle : seen) {
    if (obstacle.id == id) {
      return obstacle;
    }
  }
  return seen[0];
}

void run() {
  std::vector<Obstacle> seen{make_obstacle(1), make_obstacle(2)};
  Obstacle* p{&seen[0]};
  int n{4};
  // Write your answer for each line before you run it.
  std::cout << " 1. seen: " << CATEGORY(seen) << '\n';
  std::cout << " 2. seen[1].pose: " << CATEGORY(seen[1].pose) << '\n';
  std::cout << " 3. p->pose.x: " << CATEGORY(p->pose.x) << '\n';
  std::cout << " 4. n * 2: " << CATEGORY(n * 2) << '\n';
  std::cout << " 5. lookup(seen, 1): " << CATEGORY(lookup(seen, 1)) << '\n';
  std::cout << " 6. make_path(n): " << CATEGORY(make_path(n)) << '\n';
  std::cout << " 7. std::move(seen[0]): " << CATEGORY(std::move(seen[0])) << '\n';
  std::cout << " 8. std::move(seen[0]).pose: " << CATEGORY(std::move(seen[0]).pose) << '\n';
  std::cout << " 9. make_obstacle(n).speed: " << CATEGORY(make_obstacle(n).speed) << '\n';
  std::cout << "10. ++n: " << CATEGORY(++n) << '\n';
}
}  // namespace exercise_categories

// [Slide 35] Binding to rvalues
namespace rvalue_references {
void run() {
  std::vector<Pose> path{make_path(1000)};
  std::vector<Pose>&& temp_ref{make_path(10)};  // OK: binds to an rvalue
  const std::vector<Pose>& any_ref{path};       // const T& binds to both (Lecture 5)
  // Does not compile: cannot bind rvalue reference of type
  // 'std::vector<Pose>&&' to lvalue of type 'std::vector<Pose>'
  // std::vector<Pose>&& lvalue_ref{path};
  std::cout << "temp_ref.size(): " << temp_ref.size() << ", any_ref.size(): " << any_ref.size()
            << '\n';  // 10 1000
}
}  // namespace rvalue_references

// [Slide 36] What Each Reference Refers To
// The slide is the figure of [Slide 35]'s code: temp_ref, any_ref and
// lvalue_ref, and what each one refers to. Its code runs in [Slide 35].

// [Slide 37] Overloads for Copy and Move
namespace pick_copy_or_move {
std::vector<Pose> controller_path;

void assign_path(const std::vector<Pose>& path) {
  std::cout << "  const& overload: copy\n";
  controller_path = path;
}
void assign_path(std::vector<Pose>&& path) {
  std::cout << "  && overload: move\n";
  controller_path = std::move(path);
}

void run() {
  std::vector<Pose> path{make_path(1000)};
  std::vector<Pose>&& r{make_path(10)};
  std::cout << "assign_path(path):\n";
  assign_path(path);               // lvalue: copy
  std::cout << "assign_path(make_path(5)):\n";
  assign_path(make_path(5));       // prvalue: move
  std::cout << "assign_path(r):\n";
  assign_path(r);                  // r is an lvalue: copy
  std::cout << "assign_path(std::move(path)):\n";
  assign_path(std::move(path));    // xvalue: move
}
}  // namespace pick_copy_or_move

// [Slide 38] What std::move Does
namespace move_moves_nothing {
void run() {
  std::vector<Pose> path{make_path(1000)};
  // Warns: ignoring return value of 'constexpr typename std::remove_reference<_Tp>::type&&
  //        std::move(_Tp&&) [with _Tp = vector<Pose>&; typename
  //        remove_reference<_Tp>::type = vector<Pose>]', declared with attribute
  //        'nodiscard' [-Wunused-result]
  // std::move(path);
  static_cast<void>(std::move(path));  // the cast, with its result thrown away
  std::cout << "path.size(): " << path.size() << '\n';  // 1000: nothing moved
}
}  // namespace move_moves_nothing

// [Slide 39] A Parameter the Function Moves From
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

// [Slide 40] Returning a Local
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

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {12, "Inside a Copy", inside_copy::run},
      {13, "Inside a Move", inside_move::run},
      {14, "Elision or a Move", elision_or_move::run},
      {15, "The Moved-from Object", moved_from::run},
      {16, "Exercise 1: Copy and Move", exercise_copy_move::run},
      {20, "A Named Object", lvalue_name::run},
      {21, "Copied, Not Taken", lvalue_copied::run},
      {22, "Checking with &", lvalue_test::run},
      {24, "A Computed Value", prvalue_value::run},
      {26, "Marked by std::move", xvalue_marked::run},
      {28, "Temporary Materialization", materialization::run},
      {30, "std::is_same_v", same_type::run},
      {31, "The decltype Test", testing_category::run},
      {33, "Exercise 2: Value Categories", exercise_categories::run},
      {35, "Binding to rvalues", rvalue_references::run},
      {37, "Overloads for Copy and Move", pick_copy_or_move::run},
      {38, "What std::move Does", move_moves_nothing::run},
      {39, "A Parameter the Function Moves From", will_move_from::run},
      {40, "Returning a Local", return_by_value::run},
  };
}

}  // namespace moves
