/**
 * @file lambdas.cpp
 * @brief L6 Section 5, Lambdas: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week6_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground       # every slide of every section, in order
 * 702run week6_playground 56    # only [Slide 56]
 * @endcode
 *
 * Each slide's code is in its own namespace, inside the namespace @c lambdas, so
 * two slides, or two section files, can both declare a @c RobotStatus without
 * a clash. The comment above a namespace names its slide, and slides() at the
 * bottom lists them.
 *
 * The structs and the demo fleet are declared once, at the top. Code that does
 * not compile is commented out where its slide shows it: uncomment it, build,
 * and you get the slide's error. The dangling capture, [Slide 64], is in
 * @c ../undefined/undefined.cpp, built with AddressSanitizer; a full run
 * skips it.
 */
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "sections.hpp"
#include "undefined.hpp"

namespace lambdas {

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

void print_ids(const std::vector<RobotStatus>& fleet) {
  std::cout << "ids in order: ";
  for (const auto& robot : fleet) { std::cout << robot.id << ' '; }
  std::cout << '\n';
}

// [Slide 56] Lambda Expressions
namespace lambda_expression {
void run() {
  // create it, call it at once
  std::cout << std::boolalpha << "called at once with 35.0: "
            << [](double pct) { return pct < 40.0; }(35.0) << std::noboolalpha << '\n';  // true

  auto is_low = [](double pct) { return pct < 40.0; };  // create it, store it
  std::cout << std::boolalpha << "is_low(35.0): " << is_low(35.0)
            << ", is_low(64.0): " << is_low(64.0) << std::noboolalpha << '\n';  // true false
}
}  // namespace lambda_expression

// [Slide 57] Passing a Lambda
namespace passing_lambda {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  auto is_low = [](double pct) { return pct < 40.0; };
  std::cout << "count_if with is_low: "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';  // 2
  std::cout << "count_if with the lambda in the call: "
            << std::count_if(battery_pct.begin(), battery_pct.end(),
                             [](double pct) { return pct < 40.0; })
            << '\n';  // 2
}
}  // namespace passing_lambda

// [Slide 58] std::find_if with a Lambda
namespace find_if_lambda {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};  // robots 1, 2, 3 and 4; only robot 2 is busy
  auto is_busy = [](const RobotStatus& robot) { return robot.busy; };
  auto first_busy = std::find_if(fleet.begin(), fleet.end(), is_busy);
  std::cout << "first_busy->id: " << first_busy->id << '\n';  // 2
}
}  // namespace find_if_lambda

// [Slide 59] std::sort with a Lambda
namespace sort_lambda {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  // true when left must come before right: more battery first
  auto higher_battery = [](const RobotStatus& left, const RobotStatus& right) {
    return left.battery_pct > right.battery_pct;
  };
  std::sort(fleet.begin(), fleet.end(), higher_battery);
  print_ids(fleet);  // 1 3 2 4
}
}  // namespace sort_lambda

// [Slide 60] Captures
namespace no_capture {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  double limit_pct{40.0};  // a local variable of run
  auto is_low = [limit_pct](double pct) { return pct < limit_pct; };
  std::cout << "count_if with [limit_pct]: "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';  // 2
}
}  // namespace no_capture

// [Slide 61] By Value and by Reference
namespace by_value_reference {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  double limit_pct{40.0};
  auto is_low = [limit_pct](double pct) { return pct < limit_pct; };
  auto is_low_ref = [&limit_pct](double pct) { return pct < limit_pct; };

  limit_pct = 70.0;
  std::cout << "limit 70, is_low (copy of 40): "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';  // 2
  std::cout << "limit 70, is_low_ref: "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low_ref) << '\n';  // 3
  limit_pct = 20.0;
  std::cout << "limit 20, is_low_ref: "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low_ref) << '\n';  // 1
}
}  // namespace by_value_reference

// [Slide 63] What the Compiler Writes
namespace compiler_writes {
struct IsLow {
  double limit_pct;  // the capture
  bool operator()(double pct) const { return pct < limit_pct; }
};

void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  double limit_pct{40.0};
  auto is_low = [limit_pct](double pct) { return pct < limit_pct; };
  IsLow is_low_struct{limit_pct};
  std::cout << "count_if with is_low: " << std::count_if(battery_pct.begin(), battery_pct.end(), is_low)
            << ", with is_low_struct: " << std::count_if(battery_pct.begin(), battery_pct.end(), is_low_struct)
            << '\n';  // 2 2
  std::cout << "sizeof(is_low): " << sizeof(is_low) << ", sizeof(is_low_struct): " << sizeof(is_low_struct) << '\n';  // 8 8

  auto none = [] { return 0; };
  auto two_refs = [&limit_pct, &battery_pct] { return limit_pct + battery_pct[0]; };
  std::cout << "sizeof(none): " << sizeof(none) << ", sizeof(two_refs): " << sizeof(two_refs)
            << '\n';  // 1 16
}
}  // namespace compiler_writes

// [Slide 65] Generic Lambdas
namespace generic {
void run() {
  auto larger = [](const auto& left, const auto& right) { return left > right ? left : right; };
  std::cout << "larger(3, 7): " << larger(3, 7) << ", larger(82.5, 64.0): " << larger(82.5, 64.0)
            << ", larger(\"dock\", \"aisle 4\"): "
            << larger(std::string{"dock"}, std::string{"aisle 4"}) << '\n';  // 7 82.5 dock
  std::cout << "larger(3, 7.5): " << larger(3, 7.5) << '\n';  // 7.5
}
}  // namespace generic

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
    {56, "Lambda Expressions", lambda_expression::run},
    {57, "Passing a Lambda", passing_lambda::run},
    {58, "std::find_if with a Lambda", find_if_lambda::run},
    {59, "std::sort with a Lambda", sort_lambda::run},
    {60, "Captures", no_capture::run},
    {61, "By Value and by Reference", by_value_reference::run},
    {63, "What the Compiler Writes", compiler_writes::run},
    {64, "A Dangling Capture", undefined::dangling_capture::run, true},
    {65, "Generic Lambdas", generic::run},
  };
}

}  // namespace lambdas
