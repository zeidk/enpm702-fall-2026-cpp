/**
 * @file results.cpp
 * @brief L6 Section 2, Multiple and Optional Results: the code of every slide,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week6_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground       # every slide of every section, in order
 * 702run week6_playground 24    # only [Slide 24]
 * @endcode
 *
 * Each slide's code is in its own namespace, inside the namespace @c results, so
 * two slides, or two section files, can both declare a @c RobotStatus without
 * a clash. The comment above a namespace names its slide, and slides() at the
 * bottom lists them.
 *
 * The two structs and the demo fleet are declared once, at the top, because
 * every slide of this section uses them. Code that does not compile is
 * commented out where its slide shows it: uncomment it, build, and you get the
 * slide's error. [Slide 35] throws, and calls the code with undefined behavior
 * in @c ../undefined/undefined.cpp, so a full run skips it.
 */
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "sections.hpp"
#include "undefined.hpp"

namespace results {

struct Position {
  double x{0.0};
  double y{0.0};
};

struct RobotStatus {
  int id{0};
  double battery_pct{100.0};
  Position position{};
  bool busy{false};
};

// The demo fleet used on every slide.
std::vector<RobotStatus> make_fleet() {
  return {{1, 82.5, {0.0, 0.0}, false},
          {2, 35.0, {4.0, 1.0}, true},
          {3, 64.0, {2.0, 3.0}, false},
          {4, 18.0, {6.0, 2.0}, false}};
}

// [Slide 24] Returning a std::pair
namespace returning_several {
// How many full boxes, and how many parts are left over.
std::pair<int, int> pack(int parts, int per_box) {
  return {parts / per_box, parts % per_box};  // a braced list, as for a struct
}

void run() {
  std::pair<int, int> packed{pack(17, 5)};
  std::cout << "packed.first: " << packed.first << ", packed.second: " << packed.second
            << '\n';  // 3 2
}
}  // namespace returning_several

// [Slide 25] Returning a std::tuple
namespace returning_tuple {
// full boxes, parts left over, boxes needed
std::tuple<int, int, int> pack_as_tuple(int parts, int per_box) {
  int full_boxes{parts / per_box};
  int left_over{parts % per_box};
  int boxes_needed{full_boxes};
  if (left_over > 0) { ++boxes_needed; }  // one more box for the rest
  return {full_boxes, left_over, boxes_needed};
}

void run() {
  std::tuple<int, int, int> packed{pack_as_tuple(17, 5)};
  std::cout << "std::get<0>(packed): " << std::get<0>(packed)
            << ", std::get<1>(packed): " << std::get<1>(packed)
            << ", std::get<2>(packed): " << std::get<2>(packed) << '\n';  // 3 2 4
}
}  // namespace returning_tuple

// [Slide 26] Returning a struct
namespace returning_struct {
struct Packing {
  int full_boxes;
  int left_over;
  int boxes_needed;
};

Packing pack_as_struct(int parts, int per_box) {
  int full_boxes{parts / per_box};
  int left_over{parts % per_box};
  int boxes_needed{full_boxes};
  if (left_over > 0) { ++boxes_needed; }
  return {full_boxes, left_over, boxes_needed};
}

void run() {
  Packing packing{pack_as_struct(17, 5)};
  std::cout << "packing.boxes_needed: " << packing.boxes_needed << '\n';  // 4
}
}  // namespace returning_struct

// [Slide 28] Structured Bindings
namespace structured_bindings {
using returning_several::pack;
using returning_struct::pack_as_struct;

void run() {
  auto [boxes, left_over] = pack(17, 5);
  std::cout << "boxes: " << boxes << ", left_over: " << left_over << '\n';  // 3 2

  auto [full, left, needed] = pack_as_struct(17, 5);
  std::cout << "full: " << full << ", left: " << left << ", needed: " << needed
            << '\n';  // 3 2 4
}
}  // namespace structured_bindings

// [Slide 29] By Value and by Reference
namespace by_value_reference {
void run() {
  std::pair<int, int> packed{3, 2};  // full boxes, parts left over

  auto [boxes, left_over] = packed;  // copies packed
  left_over = 0;
  std::cout << "packed.second: " << packed.second << '\n';  // 2: unchanged

  auto& [ref_boxes, ref_left_over] = packed;  // refers to packed
  ref_left_over = 0;
  std::cout << "packed.second: " << packed.second << '\n';  // 0

  // Print the other names too, so the compiler does not warn that they are unused.
  std::cout << "boxes: " << boxes << ", ref_boxes: " << ref_boxes << '\n';  // 3 3
}
}  // namespace by_value_reference

// [Slide 30] One Name per Member
namespace binding_count {
void run() {
  returning_struct::Packing packing{3, 2, 4};
  // Does not compile: Packing decomposes into 3 elements.
  // auto [full, left] = packing;
  auto [full, left, needed] = packing;  // three names
  std::cout << "full: " << full << ", left: " << left << ", needed: " << needed
            << '\n';  // 3 2 4
}
}  // namespace binding_count

// [Slide 31] std::optional
namespace optional_age {
namespace old_way {
// Age of a person, or -1 when the name is unknown.
int find_age(const std::string& name) {
  if (name == "Ana") { return 31; }
  if (name == "Ben") { return 24; }
  return -1;  // -1 means "no age"
}
}  // namespace old_way

std::optional<int> find_age(const std::string& name) {
  if (name == "Ana") { return 31; }
  if (name == "Ben") { return 24; }
  return std::nullopt;  // no age
}

void run() {
  std::cout << "old_way::find_age(\"Cy\"): " << old_way::find_age("Cy")
            << ", plus one: " << old_way::find_age("Cy") + 1 << '\n';  // -1 0
  std::cout << std::boolalpha << "find_age(\"Ana\").has_value(): " << find_age("Ana").has_value()
            << ", find_age(\"Cy\").has_value(): " << find_age("Cy").has_value()
            << std::noboolalpha << '\n';  // true false
}
}  // namespace optional_age

// [Slide 32] Finding an Idle Robot
namespace optional_def {
// Id of the first idle robot with at least min_battery_pct, if there is one.
std::optional<int> find_idle_robot(const std::vector<RobotStatus>& fleet,
                                   double min_battery_pct) {
  for (const auto& robot : fleet) {
    if (!robot.busy && robot.battery_pct >= min_battery_pct) { return robot.id; }
  }
  return std::nullopt;  // the empty value
}

void run() {
  std::optional<int> idle{find_idle_robot(make_fleet(), 50.0)};
  if (idle) {
    std::cout << "find_idle_robot(fleet, 50.0): robot " << *idle << '\n';  // robot 1
  }
}
}  // namespace optional_def

// [Slide 33] Reading an Optional
namespace reading_optional {
using optional_def::find_idle_robot;

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::optional<int> idle{find_idle_robot(fleet, 50.0)};
  if (idle) {                                     // or idle.has_value()
    std::cout << "robot " << *idle << '\n';       // robot 1
  }
}
}  // namespace reading_optional

// [Slide 34] A Fallback Value
namespace fallback_value {
using optional_def::find_idle_robot;

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::optional<int> none{find_idle_robot(fleet, 90.0)};
  // prints 0 -1
  std::cout << "none.has_value(): " << none.has_value()
            << ", none.value_or(-1): " << none.value_or(-1) << '\n';
}
}  // namespace fallback_value

// [Slide 35] Three Ways to Read
// The three reads of the table, on an empty optional. *idle is undefined
// behavior, so that line is in ../undefined/undefined.cpp, built with
// AddressSanitizer. value() throws std::bad_optional_access and nothing catches
// it, so the program stops with exit status 134. It comes last for that
// reason. How to catch it is in the exceptions reading.
namespace three_ways {
void run() {
  std::optional<int> idle;  // empty
  std::cout << "idle.value_or(-1): " << idle.value_or(-1) << '\n';  // -1
  undefined::optional_star::run();                                  // *idle: undefined behavior
  std::cout << "idle.value(): " << idle.value() << '\n';           // throws
}
}  // namespace three_ways

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
    {24, "Returning a std::pair", returning_several::run},
    {25, "Returning a std::tuple", returning_tuple::run},
    {26, "Returning a struct", returning_struct::run},
    {28, "Structured Bindings", structured_bindings::run},
    {29, "By Value and by Reference", by_value_reference::run},
    {30, "One Name per Member", binding_count::run},
    {31, "std::optional", optional_age::run},
    {32, "Finding an Idle Robot", optional_def::run},
    {33, "Reading an Optional", reading_optional::run},
    {34, "A Fallback Value", fallback_value::run},
    {35, "Three Ways to Read", three_ways::run, true},
  };
}

}  // namespace results
