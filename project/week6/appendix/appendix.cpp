/**
 * @file appendix.cpp
 * @brief L6, Functions, Advanced Topics: the code of every appendix frame,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_appendix.
 *
 * @code
 * 702build week6_appendix
 * 702run week6_appendix         # every appendix frame, in order
 * 702run week6_appendix xvii    # only [Appendix xvii]; 702run week6_appendix 17 works too
 * @endcode
 *
 * The appendix frames are numbered i, ii, iii, ... in their top-left corner,
 * and @c "[Appendix xvii]" is that number. Each frame's code is in its own
 * namespace, with a @c run() that does what the frame does in @c main(). The
 * table in main() lists them, and run_slides() in @c ../common/slides.cpp runs
 * them. Code that does not compile is commented out where its frame shows it:
 * uncomment it, build, and you get the frame's error.
 */
#include <algorithm>
#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <map>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

#include "slides.hpp"

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

// The demo fleet used on the lambda frames.
std::vector<RobotStatus> make_fleet() {
  return {{1, 82.5, {0.0, 0.0}, false},
          {2, 35.0, {4.0, 1.0}, true},
          {3, 64.0, {2.0, 3.0}, false},
          {4, 18.0, {6.0, 2.0}, false}};
}

void print_ids(const std::vector<RobotStatus>& fleet) {
  std::cout << "ids in order: ";
  for (const auto& robot : fleet) { std::cout << robot.id << ' '; }
  std::cout << '\n';
}

// [Appendix vi] The Lecture 4 Map Loop
namespace map_loop {
void run() {
  std::map<int, std::string> zone_of{{1, "dock"}, {2, "aisle 4"}, {3, "aisle 7"}};
  for (const auto& [id, zone] : zone_of) {
    std::cout << "robot " << id << ": " << zone << '\n';
  }
}
}  // namespace map_loop

// [Appendix vii] auto Returns a Copy
namespace auto_copy {
auto first_copy(const std::vector<std::string>& names) {
  return names[0];  // std::string: a copy
}
const auto& first_ref(const std::vector<std::string>& names) {
  return names[0];  // const std::string&: the element itself
}
decltype(auto) first_exact(const std::vector<std::string>& names) {
  return names[0];  // const std::string&: the exact type, & included
}

// The three return types, checked while compiling.
static_assert(std::same_as<decltype(first_copy(std::vector<std::string>{})), std::string>);
static_assert(std::same_as<decltype(first_ref(std::vector<std::string>{})), const std::string&>);
static_assert(std::same_as<decltype(first_exact(std::vector<std::string>{})), const std::string&>);

void run() {
  std::vector<std::string> sensors{"imu", "gps"};
  std::string copy{first_copy(sensors)};
  const std::string& ref{first_ref(sensors)};
  sensors[0] = "lidar";
  std::cout << "copy: " << copy << ", ref: " << ref << '\n';  // copy: imu, ref: lidar
}
}  // namespace auto_copy

// [Appendix ix] typename or class
namespace typename_or_class {
// Two declarations of the same template: class and typename mean the same here.
template <class T>    T clamp_value(T value, T low, T high);
template <typename T> T clamp_value(T value, T low, T high) {
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

void run() {
  std::cout << "clamp_value(130, 0, 100): " << clamp_value(130, 0, 100) << '\n';  // 100
}
}  // namespace typename_or_class

// [Appendix x] Dependent Name
namespace dependent_name {
template <typename Container>
void print_first(const Container& values) {
  typename Container::value_type first{values.front()};
  std::cout << first << '\n';
  // Does not compile without typename:
  // error: need 'typename' before 'Container::value_type' because 'Container'
  // is a dependent scope
  // Container::value_type second{values.back()};
}

void run() {
  std::vector<double> readings{12.5, 13.1};
  print_first(readings);  // 12.5
}
}  // namespace dependent_name

// [Appendix xi] Where the Body Ends Up
// The frame splits a template like a regular function: the declaration in
// stats.hpp, the body in stats.cpp. One file shows the same linker error: a
// template that is declared and never defined.
namespace template_link {
template <typename T> T clamp_value(T value, T low, T high);  // the declaration only

void run() {
  // Does not link: undefined reference to
  // `double template_link::clamp_value<double>(double, double, double)'
  // double pct{clamp_value(104.2, 0.0, 100.0)};
  std::cout << "does not link: uncomment the line in template_link::run()\n";
}
}  // namespace template_link

// [Appendix xvi] std::transform
namespace transform_appendix {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  std::vector<double> fraction(battery_pct.size());  // 4 elements, all 0
  std::transform(battery_pct.begin(), battery_pct.end(), fraction.begin(),
                 [](double pct) { return pct / 100.0; });
  std::cout << "fraction: ";
  for (double value : fraction) { std::cout << value << ' '; }
  std::cout << '\n';  // 0.825 0.35 0.64 0.18
}
}  // namespace transform_appendix

// [Appendix xvii] Projections (C++20)
namespace projections {
void run() {
  std::vector<std::string> zones{"charging bay", "dock", "aisle 4"};
  // the projection turns each zone into its length
  std::ranges::sort(zones, {}, [](const std::string& zone) { return zone.size(); });
  std::cout << "zones by length: ";
  for (const auto& zone : zones) { std::cout << '"' << zone << "\" "; }
  std::cout << '\n';  // "dock" "aisle 4" "charging bay"
}
}  // namespace projections

// [Appendix xviii] Projections with RobotStatus
namespace projections_robot {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};  // the four robots of the slide
  std::ranges::sort(fleet, {}, [](const RobotStatus& robot) { return robot.battery_pct; });
  print_ids(fleet);  // 4 2 3 1

  auto closest = std::ranges::min_element(fleet, {}, [](const RobotStatus& robot) {
    return std::hypot(robot.position.x - 5.0, robot.position.y - 5.0);
  });
  std::cout << "closest to the task at (5, 5): " << closest->id << '\n';  // 4
}
}  // namespace projections_robot

// [Appendix xix] mutable and Init-capture
namespace mutable_init {
void run() {
  // Does not compile: a copy capture is read-only without mutable.
  // int assigned{0};
  // auto assign = [assigned]() { ++assigned; };

  auto next_task_id = [id = 100]() mutable { return ++id; };
  // prints 101 102 103
  std::cout << "next_task_id() three times: " << next_task_id() << ' ' << next_task_id() << ' '
            << next_task_id() << '\n';

  auto copy = next_task_id;  // copies the lambda, with id at 103
  std::cout << "copy(): " << copy() << ", next_task_id(): " << next_task_id()
            << '\n';  // 104 104
}
}  // namespace mutable_init

// [Appendix xx] The Return Type
namespace return_type {
void run() {
  // Does not compile: one return gives int, the other double.
  // {
  //   auto speed_for = [](double battery_pct) {
  //     if (battery_pct < 20.0) { return 0; }
  //     return 0.01 * battery_pct;
  //   };
  // }

  auto speed_for = [](double battery_pct) -> double {
    if (battery_pct < 20.0) { return 0; }  // 0 converts to 0.0
    return 0.01 * battery_pct;             // m/s
  };
  std::cout << "speed_for(15.0): " << speed_for(15.0) << ", speed_for(80.0): " << speed_for(80.0)
            << '\n';  // 0 0.8
}
}  // namespace return_type

// [Appendix xxi] Template Lambdas (C++20)
namespace template_lambda {
void run() {
  auto larger = [](const auto& left, const auto& right) {
    return left > right ? left : right;
  };
  auto larger_same = []<typename T>(const T& left, const T& right) {
    return left > right ? left : right;
  };
  std::cout << "larger(3, 7.5): " << larger(3, 7.5)
            << ", larger_same(3, 7): " << larger_same(3, 7) << '\n';  // 7.5 7
  // Does not compile: 3 says T is int, 7.5 says double.
  // larger_same(3, 7.5);
}
}  // namespace template_lambda

// [Appendix xxii] Function Pointers
namespace function_pointers {
double to_fraction(double pct) {
  return pct / 100.0;
}
double to_pct(double fraction) { return fraction * 100.0; }  // the inverse

void run() {
  double (*convert)(double){to_fraction};
  std::cout << "convert(82.5) with to_fraction: " << convert(82.5) << '\n';  // 0.825
  convert = to_pct;
  std::cout << "convert(0.35) with to_pct: " << convert(0.35) << '\n';  // 35
}
}  // namespace function_pointers

// [Appendix xxiii] Passing a Function
namespace passing_function {
void convert_all(double* values, int count, double (*convert)(double)) {
  for (int i{0}; i < count; ++i) { values[i] = convert(values[i]); }
}

void run() {
  double battery[]{82.5, 35.0};
  convert_all(battery, 2, [](double pct) { return pct / 100.0; });  // OK: no capture
  // Does not compile: a lambda that captures is not a function pointer.
  // double scale{2.0};
  // convert_all(battery, 2, [scale](double pct) { return scale * pct; });
  std::cout << "battery[0]: " << battery[0] << ", battery[1]: " << battery[1]
            << '\n';  // 0.825 0.35
  convert_all(battery, 2, function_pointers::to_pct);            // a function name
  std::cout << "battery[0]: " << battery[0] << ", battery[1]: " << battery[1]
            << '\n';  // 82.5 35
}
}  // namespace passing_function

// [Appendix xxv] std::source_location
// At namespace scope, not in a namespace of its own, so function_name() prints
// the plain names the slide shows.
void log_message(
  std::string_view text,
  std::source_location at = std::source_location::current()) {
  std::string_view file{at.file_name()};
  file.remove_prefix(file.rfind('/') + 1);  // the name only
  std::cout << file << ':' << at.line() << ' '
            << at.function_name() << ": " << text << '\n';
}

void assign_task(int task_id, int robot_id) {
  log_message("task " + std::to_string(task_id) + " to robot " + std::to_string(robot_id));
}

void end_shift() {
  log_message("shift over");
}

namespace source_location_slide {
void run() {
  assign_task(17, 3);
  end_shift();
}
}  // namespace source_location_slide

// [Appendix xxvi] bind_front and Lambdas
namespace bind_front {
// charge_time_h from [Slide 71] std::bind, in week6_playground.
double charge_time_h(double missing_pct, double rate_pct_per_h) {
  return missing_pct / rate_pct_per_h;
}

void run() {
  auto to_half = std::bind_front(charge_time_h, 50.0);  // C++20
  std::cout << "to_half(25.0): " << to_half(25.0) << '\n';  // 2

  auto fast_l = [](double missing_pct) {
    return charge_time_h(missing_pct, 40.0);
  };
  auto swap_l = [](double rate_pct_per_h, double missing_pct) {
    return charge_time_h(missing_pct, rate_pct_per_h);
  };
  std::cout << "fast_l(60.0): " << fast_l(60.0) << ", swap_l(20.0, 60.0): " << swap_l(20.0, 60.0)
            << '\n';  // 1.5 3

  using namespace std::placeholders;
  auto at_fast_dock = std::bind(charge_time_h, _1, 40.0);
  // compiles, prints 1.5: 99.0 is dropped
  std::cout << "at_fast_dock(60.0, 99.0): " << at_fast_dock(60.0, 99.0) << '\n';
}
}  // namespace bind_front

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (17 is xvii), its
  // title, and the function that runs it.
  const std::vector<Slide> slides{
      {6, "The Lecture 4 Map Loop", map_loop::run},
      {7, "auto Returns a Copy", auto_copy::run},
      {9, "typename or class", typename_or_class::run},
      {10, "Dependent Name", dependent_name::run},
      {11, "Where the Body Ends Up", template_link::run},
      {16, "std::transform", transform_appendix::run},
      {17, "Projections (C++20)", projections::run},
      {18, "Projections with RobotStatus", projections_robot::run},
      {19, "mutable and Init-capture", mutable_init::run},
      {20, "The Return Type", return_type::run},
      {21, "Template Lambdas (C++20)", template_lambda::run},
      {22, "Function Pointers", function_pointers::run},
      {23, "Passing a Function", passing_function::run},
      {25, "std::source_location", source_location_slide::run},
      {26, "bind_front and Lambdas", bind_front::run},
  };
  return run_slides(slides, argc, argv, Part::appendix);
}
