/**
 * @file results.cpp
 * @brief L6 Section 2, Multiple and Optional Results: the code of every slide,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_results. This file stands alone.
 *
 * @code
 * 702build week6_results
 * 702run week6_results        # every slide of the section, in order
 * 702run week6_results 24     # only [Slide 24]
 * @endcode
 *
 * The two structs and the demo fleet are declared once, at the top, because
 * every slide of this section uses them. Code that does not compile is
 * commented out where its slide shows it: uncomment it, build, and you get the
 * slide's error. Code that throws is in @c ../throws/, and code with undefined
 * behavior in @c ../undefined/.
 */
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

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

// [Appendix] The Lecture 4 Map Loop: runs only with the whole program
namespace map_loop {
void run() {
  std::map<int, std::string> zone_of{{1, "dock"}, {2, "aisle 4"}, {3, "aisle 7"}};
  for (const auto& [id, zone] : zone_of) {
    std::cout << "robot " << id << ": " << zone << '\n';
  }
}
}  // namespace map_loop

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

// [Slide 37] std::optional, Pointer, or Special Value
namespace optional_pointer {
void run() {
  std::cout << "sizeof(std::optional<int>): " << sizeof(std::optional<int>)
            << ", sizeof(int): " << sizeof(int) << '\n';  // 8 4
}
}  // namespace optional_pointer

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: see the appendix, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  const std::string header{"[Slide " + std::to_string(slide) + "] " + title};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
  run();
}

// Runs one appendix frame's code, only when the whole program runs: appendix
// frames have no slide number to ask for.
void show_appendix(int only, const char* title, void (*run)()) {
  if (only != 0) { return; }
  const std::string header{std::string{"[Appendix] "} + title};
  const std::string rule(header.size(), '-');
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 24, "Returning a std::pair", returning_several::run);
  show(only, 25, "Returning a std::tuple", returning_tuple::run);
  show(only, 26, "Returning a struct", returning_struct::run);
  show(only, 28, "Structured Bindings", structured_bindings::run);
  show(only, 29, "By Value and by Reference", by_value_reference::run);
  show(only, 30, "One Name per Member", binding_count::run);
  show(only, 31, "std::optional", optional_age::run);
  show(only, 32, "Finding an Idle Robot", optional_def::run);
  show(only, 33, "Reading an Optional", reading_optional::run);
  show(only, 34, "A Fallback Value", fallback_value::run);
  show(only, 37, "std::optional, Pointer, or Special Value", optional_pointer::run);
  show_appendix(only, "The Lecture 4 Map Loop", map_loop::run);
}
