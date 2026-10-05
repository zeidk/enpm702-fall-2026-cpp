/**
 * @file lambdas.cpp
 * @brief L6 Section 4, Lambdas: the code of every slide, runnable.
 *
 * @details Build target: @c week6_lambdas. This file stands alone.
 *
 * @code
 * 702build week6_lambdas
 * 702run week6_lambdas        # every slide of the section, in order
 * 702run week6_lambdas 54     # only [Slide 54]
 * @endcode
 *
 * The structs and the demo fleet are declared once, at the top. Code that does
 * not compile is commented out where its slide shows it: uncomment it, build,
 * and you get the slide's error. The dangling capture is in
 * @c ../undefined/dangling_capture.cpp, built with AddressSanitizer.
 */
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
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

void print_ids(const std::vector<RobotStatus>& fleet) {
  for (const auto& r : fleet) { std::cout << r.id << ' '; }
  std::cout << '\n';
}

// [Slide 51] A Condition instead of a Value
namespace condition {
bool is_low(double pct) { return pct < 40.0; }  // outside main

void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  std::cout << std::count(battery_pct.begin(), battery_pct.end(), 35.0) << '\n';      // 1
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';  // 2
}
}  // namespace condition

// [Slide 52] Lambda Expressions
namespace lambda_expression {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(),
                             [](double pct) { return pct < 40.0; })
            << '\n';  // 2
}
}  // namespace lambda_expression

// [Slide 53] Lambdas with Algorithms
namespace algorithms {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  // highest battery first: true when a must come before b
  std::sort(fleet.begin(), fleet.end(), [](const RobotStatus& a, const RobotStatus& b) {
    return a.battery_pct > b.battery_pct;
  });
  print_ids(fleet);  // 1 3 2 4

  auto it = std::find_if(fleet.begin(), fleet.end(),
                         [](const RobotStatus& r) { return r.busy; });
  std::cout << it->id << '\n';  // 2: the first busy robot

  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  std::vector<double> fraction(battery_pct.size());
  std::transform(battery_pct.begin(), battery_pct.end(), fraction.begin(),
                 [](double pct) { return pct / 100.0; });
  for (double f : fraction) { std::cout << f << ' '; }
  std::cout << '\n';  // 0.825 0.35 0.64 0.18
}
}  // namespace algorithms

// [Slide 54] Projections (C++20)
namespace projections {
void run() {
  std::vector<RobotStatus> fleet{make_fleet()};
  std::ranges::sort(fleet, {}, [](const RobotStatus& r) { return r.battery_pct; });
  print_ids(fleet);  // 4 2 3 1

  auto closest = std::ranges::min_element(fleet, {}, [](const RobotStatus& r) {
    return std::hypot(r.position.x - 5.0, r.position.y - 5.0);
  });
  std::cout << closest->id << '\n';  // 4: closest to the pickup at (5, 5)
}
}  // namespace projections

// [Slide 55] Captures
namespace no_capture {
void run() {
  // Does not compile: limit_pct is not captured.
  // double limit_pct{40.0};
  // auto is_low = [](double pct) { return pct < limit_pct; };
  std::cout << "does not compile: uncomment the lines in no_capture::run()\n";
}
}  // namespace no_capture

// [Slide 56] By Value and by Reference
namespace by_value_reference {
void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  double limit_pct{40.0};
  auto is_low = [limit_pct](double pct) { return pct < limit_pct; };
  auto is_low_ref = [&limit_pct](double pct) { return pct < limit_pct; };

  limit_pct = 70.0;
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';      // 2
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(), is_low_ref) << '\n';  // 3
  limit_pct = 20.0;
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(), is_low_ref) << '\n';  // 1
}
}  // namespace by_value_reference

// [Slide 58] mutable and Init-capture
namespace mutable_init {
void run() {
  // Does not compile: a copy capture is read-only without mutable.
  // int assigned{0};
  // auto assign = [assigned]() { ++assigned; };

  auto next_task_id = [id = 100]() mutable { return ++id; };
  // prints 101 102 103
  std::cout << next_task_id() << ' ' << next_task_id() << ' ' << next_task_id() << '\n';

  auto copy = next_task_id;  // copies the lambda, with id at 103
  std::cout << copy() << ' ' << next_task_id() << '\n';  // 104 104
}
}  // namespace mutable_init

// [Slide 59] What the Compiler Writes
namespace compiler_writes {
struct IsLow {
  double limit_pct;  // the capture
  bool operator()(double pct) const { return pct < limit_pct; }
};

void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  double limit_pct{40.0};
  auto a = [limit_pct](double pct) { return pct < limit_pct; };
  IsLow b{limit_pct};
  std::cout << std::count_if(battery_pct.begin(), battery_pct.end(), a) << ' '
            << std::count_if(battery_pct.begin(), battery_pct.end(), b) << '\n';  // 2 2
  std::cout << sizeof(a) << ' ' << sizeof(b) << '\n';  // 8 8

  auto none = [] { return 0; };
  auto two_refs = [&limit_pct, &battery_pct] { return limit_pct + battery_pct[0]; };
  std::cout << sizeof(none) << ' ' << sizeof(two_refs) << '\n';  // 1 16
}
}  // namespace compiler_writes

// [Slide 61] Generic Lambdas
namespace generic {
void run() {
  auto larger = [](const auto& a, const auto& b) { return a > b ? a : b; };
  std::cout << larger(3, 7) << ' ' << larger(82.5, 64.0) << ' '
            << larger(std::string{"dock"}, std::string{"aisle 4"}) << '\n';  // 7 82.5 dock
  std::cout << larger(3, 7.5) << '\n';  // 7.5
}
}  // namespace generic

// [Slide 62] The Return Type
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
  std::cout << speed_for(15.0) << ' ' << speed_for(80.0) << '\n';  // 0 0.8
}
}  // namespace return_type

// [Slide 63] Template Lambdas (C++20)
namespace template_lambda {
void run() {
  auto larger_same = []<typename T>(const T& a, const T& b) { return a > b ? a : b; };
  std::cout << larger_same(3, 7) << ' ' << larger_same(82.5, 64.0) << '\n';  // 7 82.5
  // Does not compile: 3 says T is int, 7.5 says double.
  // std::cout << larger_same(3, 7.5) << '\n';
}
}  // namespace template_lambda

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: Section 5, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  std::cout << "[Slide " << slide << "] " << title << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 51, "A Condition instead of a Value", condition::run);
  show(only, 52, "Lambda Expressions", lambda_expression::run);
  show(only, 53, "Lambdas with Algorithms", algorithms::run);
  show(only, 54, "Projections (C++20)", projections::run);
  show(only, 55, "Captures", no_capture::run);
  show(only, 56, "By Value and by Reference", by_value_reference::run);
  show(only, 58, "mutable and Init-capture", mutable_init::run);
  show(only, 59, "What the Compiler Writes", compiler_writes::run);
  show(only, 61, "Generic Lambdas", generic::run);
  show(only, 62, "The Return Type", return_type::run);
  show(only, 63, "Template Lambdas (C++20)", template_lambda::run);
}
