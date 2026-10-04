/**
 * @file results.cpp
 * @brief L6 Section 2, Several Results, or None: the code of every slide,
 *        runnable.
 *
 * @details Build target: @c week6_results. This file stands alone.
 *
 * @code
 * 702build week6_results
 * 702run week6_results        # every slide of the section, in order
 * 702run week6_results 21     # only [Slide 21]
 * @endcode
 *
 * The two structs and the demo fleet are declared once, at the top, because
 * every slide of this section uses them. Code that does not compile is in
 * @c ../diagnostics/, code that throws in @c ../throws/, and code with
 * undefined behavior in @c ../undefined/.
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

// [Slide 21] Returning Several Values
namespace returning_several {
std::pair<double, double> find_battery_range(
    const std::vector<RobotStatus>& fleet) {
  double lo{fleet.front().battery_pct};
  double hi{fleet.front().battery_pct};
  for (const auto& r : fleet) {
    if (r.battery_pct < lo) { lo = r.battery_pct; }
    if (r.battery_pct > hi) { hi = r.battery_pct; }
  }
  return {lo, hi};  // a braced list, as for a struct
}

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::pair<double, double> range{find_battery_range(fleet)};
  std::cout << range.first << ' ' << range.second << '\n';  // 18 82.5
}
}  // namespace returning_several

// [Slide 22] Pair, Tuple or Struct
namespace pair_tuple_struct {
std::tuple<double, double, int> summarize(const std::vector<RobotStatus>& fleet) {
  double lo{fleet.front().battery_pct};
  double hi{fleet.front().battery_pct};
  int busy_count{0};
  for (const auto& r : fleet) {
    if (r.battery_pct < lo) { lo = r.battery_pct; }
    if (r.battery_pct > hi) { hi = r.battery_pct; }
    if (r.busy) { ++busy_count; }
  }
  return {lo, hi, busy_count};
}

struct FleetSummary {
  double min_battery_pct;
  double max_battery_pct;
  int busy_count;
};

FleetSummary summarize_fleet(const std::vector<RobotStatus>& fleet) {
  FleetSummary s{fleet.front().battery_pct, fleet.front().battery_pct, 0};
  for (const auto& r : fleet) {
    if (r.battery_pct < s.min_battery_pct) { s.min_battery_pct = r.battery_pct; }
    if (r.battery_pct > s.max_battery_pct) { s.max_battery_pct = r.battery_pct; }
    if (r.busy) { ++s.busy_count; }
  }
  return s;
}

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::tuple<double, double, int> t{summarize(fleet)};
  std::cout << std::get<0>(t) << ' ' << std::get<2>(t) << '\n';  // 18 1

  FleetSummary s{summarize_fleet(fleet)};
  std::cout << s.min_battery_pct << ' ' << s.busy_count << '\n';  // 18 1
}
}  // namespace pair_tuple_struct

// [Slide 23] Structured Bindings
namespace structured_bindings {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  auto [lo, hi] = returning_several::find_battery_range(fleet);
  std::cout << lo << ' ' << hi << '\n';  // 18 82.5

  auto [min_pct, max_pct, busy_count] = pair_tuple_struct::summarize_fleet(fleet);
  std::cout << min_pct << ' ' << max_pct << ' ' << busy_count << '\n';  // 18 82.5 1
}
}  // namespace structured_bindings

// [Slide 24] By Value and by Reference
namespace by_value_reference {
void run() {
  RobotStatus r{3, 64.0, {2.0, 3.0}, false};

  auto [id, battery_pct, position, busy] = r;  // copies r
  battery_pct = 0.0;
  std::cout << r.battery_pct << '\n';  // 64

  auto& [rid, rbattery, rposition, rbusy] = r;  // refers to r
  rbattery = 0.0;
  std::cout << r.battery_pct << '\n';  // 0

  // Print the other names too, so the compiler does not warn that they are unused.
  std::cout << id << ' ' << position.x << ' ' << busy << ' ' << rid << ' ' << rposition.y
            << ' ' << rbusy << '\n';  // 3 2 0 3 3 0
}
}  // namespace by_value_reference

// [Slide 25] The Lecture 4 Map Loop
namespace map_loop {
void run() {
  std::map<int, std::string> zone_of{{1, "dock"}, {2, "aisle 4"}, {3, "aisle 7"}};
  for (const auto& [id, zone] : zone_of) {
    std::cout << "robot " << id << ": " << zone << '\n';
  }
}
}  // namespace map_loop

// [Slide 27] std::optional
namespace optional_def {
// Id of the first idle robot with at least min_battery_pct, if there is one.
std::optional<int> find_idle_robot(const std::vector<RobotStatus>& fleet,
                                   double min_battery_pct) {
  for (const auto& r : fleet) {
    if (!r.busy && r.battery_pct >= min_battery_pct) { return r.id; }
  }
  return std::nullopt;  // the empty value
}

void run() {
  std::cout << optional_def::find_idle_robot(make_fleet(), 50.0).value_or(-1) << '\n';  // 1
}
}  // namespace optional_def

// [Slide 28] Reading an Optional
namespace reading_optional {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};

  std::optional<int> idle{optional_def::find_idle_robot(fleet, 50.0)};
  if (idle) {                                     // or idle.has_value()
    std::cout << "robot " << *idle << '\n';       // robot 1
  }

  std::optional<int> none{optional_def::find_idle_robot(fleet, 90.0)};
  // prints 0 -1
  std::cout << none.has_value() << ' ' << none.value_or(-1) << '\n';
}
}  // namespace reading_optional

// [Slide 30] Optional, Pointer or Special Value
namespace optional_pointer {
void run() {
  std::cout << sizeof(std::optional<int>) << ' ' << sizeof(int) << '\n';  // 8 4
}
}  // namespace optional_pointer

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: Section 5, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  std::cout << "[Slide " << slide << "] " << title << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 21, "Returning Several Values", returning_several::run);
  show(only, 22, "Pair, Tuple or Struct", pair_tuple_struct::run);
  show(only, 23, "Structured Bindings", structured_bindings::run);
  show(only, 24, "By Value and by Reference", by_value_reference::run);
  show(only, 25, "The Lecture 4 Map Loop", map_loop::run);
  show(only, 27, "std::optional", optional_def::run);
  show(only, 28, "Reading an Optional", reading_optional::run);
  show(only, 30, "Optional, Pointer or Special Value", optional_pointer::run);
}
