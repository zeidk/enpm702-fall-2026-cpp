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
 * 702run week6_results 25     # only [Slide 25]
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

// [Slide 25] Returning a std::pair
namespace returning_several {
std::pair<double, double> find_battery_range(
    const std::vector<RobotStatus>& fleet) {
  double min_pct{fleet.front().battery_pct};
  double max_pct{fleet.front().battery_pct};
  for (const auto& robot : fleet) {
    if (robot.battery_pct < min_pct) { min_pct = robot.battery_pct; }
    if (robot.battery_pct > max_pct) { max_pct = robot.battery_pct; }
  }
  return {min_pct, max_pct};  // a braced list, as for a struct
}

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::pair<double, double> range{find_battery_range(fleet)};
  std::cout << "range.first: " << range.first << ", range.second: " << range.second
            << '\n';  // 18 82.5
}
}  // namespace returning_several

// [Slide 26] std::pair, std::tuple, or struct
namespace pair_tuple_struct {
std::tuple<double, double, int> summarize(const std::vector<RobotStatus>& fleet) {
  double min_pct{fleet.front().battery_pct};
  double max_pct{fleet.front().battery_pct};
  int busy_count{0};
  for (const auto& robot : fleet) {
    if (robot.battery_pct < min_pct) { min_pct = robot.battery_pct; }
    if (robot.battery_pct > max_pct) { max_pct = robot.battery_pct; }
    if (robot.busy) { ++busy_count; }
  }
  return {min_pct, max_pct, busy_count};
}

struct FleetSummary {
  double min_battery_pct;
  double max_battery_pct;
  int busy_count;
};

FleetSummary summarize_fleet(const std::vector<RobotStatus>& fleet) {
  FleetSummary summary{fleet.front().battery_pct, fleet.front().battery_pct, 0};
  for (const auto& robot : fleet) {
    if (robot.battery_pct < summary.min_battery_pct) { summary.min_battery_pct = robot.battery_pct; }
    if (robot.battery_pct > summary.max_battery_pct) { summary.max_battery_pct = robot.battery_pct; }
    if (robot.busy) { ++summary.busy_count; }
  }
  return summary;
}

void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::tuple<double, double, int> summary_tuple{summarize(fleet)};
  std::cout << "std::get<2>(summary_tuple): " << std::get<2>(summary_tuple)
            << '\n';  // 1: the busy count

  FleetSummary summary{summarize_fleet(fleet)};
  std::cout << "summary.busy_count: " << summary.busy_count << '\n';  // 1
}
}  // namespace pair_tuple_struct

// [Slide 27] Structured Bindings
namespace structured_bindings {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  auto [low_pct, high_pct] = returning_several::find_battery_range(fleet);
  std::cout << "low_pct: " << low_pct << ", high_pct: " << high_pct << '\n';  // 18 82.5

  auto [min_pct, max_pct, busy_count] = pair_tuple_struct::summarize_fleet(fleet);
  std::cout << "min_pct: " << min_pct << ", max_pct: " << max_pct
            << ", busy_count: " << busy_count << '\n';  // 18 82.5 1
}
}  // namespace structured_bindings

// [Slide 28] By Value and by Reference
namespace by_value_reference {
void run() {
  RobotStatus robot_status{3, 64.0, {2.0, 3.0}, false};

  auto [id, battery_pct, position, busy] = robot_status;  // copies robot_status
  battery_pct = 0.0;
  std::cout << "robot_status.battery_pct: " << robot_status.battery_pct << '\n';  // 64

  auto& [ref_id, ref_battery_pct, ref_position, ref_busy] = robot_status;  // refers to robot_status
  ref_battery_pct = 0.0;
  std::cout << "robot_status.battery_pct: " << robot_status.battery_pct << '\n';  // 0

  // Print the other names too, so the compiler does not warn that they are unused.
  std::cout << "id: " << id << ", position.x: " << position.x << ", busy: " << busy
            << ", ref_id: " << ref_id << ", ref_position.y: " << ref_position.y << ", ref_busy: " << ref_busy
            << '\n';  // 3 2 0 3 3 0
}
}  // namespace by_value_reference

// [Slide 29] The Lecture 4 Map Loop
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
  RobotStatus robot_status{3, 64.0};
  // Does not compile: RobotStatus decomposes into 4 elements.
  // auto [id, battery_pct] = robot_status;
  auto [robot_id, pct, where, busy] = robot_status;  // four names
  std::cout << "robot_id: " << robot_id << ", pct: " << pct << ", where.x: " << where.x
            << ", busy: " << busy << '\n';  // 3 64 0 0
}
}  // namespace binding_count

// [Slide 31] std::optional
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
  std::cout << "find_idle_robot(fleet, 50.0).value_or(-1): "
            << optional_def::find_idle_robot(make_fleet(), 50.0).value_or(-1) << '\n';  // 1
}
}  // namespace optional_def

// [Slide 32] Reading an Optional
namespace reading_optional {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};

  std::optional<int> idle{optional_def::find_idle_robot(fleet, 50.0)};
  if (idle) {                                     // or idle.has_value()
    std::cout << "robot " << *idle << '\n';       // robot 1
  }

  std::optional<int> none{optional_def::find_idle_robot(fleet, 90.0)};
  // prints 0 -1
  std::cout << "none.has_value(): " << none.has_value()
            << ", none.value_or(-1): " << none.value_or(-1) << '\n';
}
}  // namespace reading_optional

// [Slide 34] std::optional, Pointer, or Special Value
namespace optional_pointer {
void run() {
  std::cout << "sizeof(std::optional<int>): " << sizeof(std::optional<int>)
            << ", sizeof(int): " << sizeof(int) << '\n';  // 8 4
}
}  // namespace optional_pointer

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: Section 5, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  const std::string header{"[Slide " + std::to_string(slide) + "] " + title};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 25, "Returning a std::pair", returning_several::run);
  show(only, 26, "std::pair, std::tuple, or struct", pair_tuple_struct::run);
  show(only, 27, "Structured Bindings", structured_bindings::run);
  show(only, 28, "By Value and by Reference", by_value_reference::run);
  show(only, 29, "The Lecture 4 Map Loop", map_loop::run);
  show(only, 30, "One Name per Member", binding_count::run);
  show(only, 31, "std::optional", optional_def::run);
  show(only, 32, "Reading an Optional", reading_optional::run);
  show(only, 34, "std::optional, Pointer, or Special Value", optional_pointer::run);
}
