/**
 * @file lambdas.cpp
 * @brief L6 Section 5, Lambdas: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_lambdas. This file stands alone.
 *
 * @code
 * 702build week6_lambdas
 * 702run week6_lambdas        # every slide of the section, in order
 * 702run week6_lambdas 56     # only [Slide 56]
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

// [Appendix] std::transform: runs only with the whole program
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

// [Appendix] Projections (C++20): runs only with the whole program
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

// [Appendix] Projections with RobotStatus: runs only with the whole program
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

// [Appendix] mutable and Init-capture: runs only with the whole program
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

// [Appendix] The Return Type: runs only with the whole program
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

// [Appendix] Template Lambdas (C++20): runs only with the whole program
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
  show(only, 56, "Lambda Expressions", lambda_expression::run);
  show(only, 57, "Passing a Lambda", passing_lambda::run);
  show(only, 58, "std::find_if with a Lambda", find_if_lambda::run);
  show(only, 59, "std::sort with a Lambda", sort_lambda::run);
  show(only, 60, "Captures", no_capture::run);
  show(only, 61, "By Value and by Reference", by_value_reference::run);
  show(only, 63, "What the Compiler Writes", compiler_writes::run);
  show(only, 65, "Generic Lambdas", generic::run);
  show_appendix(only, "std::transform", transform_appendix::run);
  show_appendix(only, "Projections (C++20)", projections::run);
  show_appendix(only, "Projections with RobotStatus", projections_robot::run);
  show_appendix(only, "mutable and Init-capture", mutable_init::run);
  show_appendix(only, "The Return Type", return_type::run);
  show_appendix(only, "Template Lambdas (C++20)", template_lambda::run);
}
