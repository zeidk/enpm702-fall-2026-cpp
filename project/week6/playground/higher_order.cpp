/**
 * @file higher_order.cpp
 * @brief L6 Sections 4 and 6, Higher-Order Functions and Storing and Adapting
 *        Callables: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week6_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground       # every slide of every section, in order
 * 702run week6_playground 52    # only [Slide 52]
 * @endcode
 *
 * Each slide's code is in its own namespace, inside the namespace @c higher_order, so
 * two slides, or two section files, can both declare a @c RobotStatus without
 * a clash. The comment above a namespace names its slide, and slides() at the
 * bottom lists them.
 *
 * Code that does not compile is commented out where its slide shows it:
 * uncomment it, build, and you get the slide's error. [Slide 66] throws, so a
 * full run skips it.
 */
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "sections.hpp"

namespace higher_order {

// [Slide 52] A Condition instead of a Value
namespace condition {
bool is_low(double pct) { return pct < 40.0; }  // outside main

void run() {
  std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  std::cout << "count of 35.0: "
            << std::count(battery_pct.begin(), battery_pct.end(), 35.0) << '\n';  // 1
  std::cout << "count_if is_low: "
            << std::count_if(battery_pct.begin(), battery_pct.end(), is_low) << '\n';  // 2
}
}  // namespace condition

// [Slide 64] std::function
namespace std_function {
// to_fraction from the appendix frame Function Pointers.
double to_fraction(double pct) { return pct / 100.0; }

void run() {
  std::function<double(double)> convert{to_fraction};
  std::cout << "convert(64.0) with to_fraction: " << convert(64.0) << '\n';  // 0.64

  double scale{2.0};
  convert = [scale](double pct) { return scale * pct; };
  std::cout << "convert(64.0) with the lambda: " << convert(64.0) << '\n';  // 128
}
}  // namespace std_function

// [Slide 65] A Table of Commands
namespace command_table {
void run() {
  std::map<std::string, std::function<void(int)>> on_command;

  on_command["dock"] = [](int id) {
    std::cout << "robot " << id << ": go to dock\n";
  };
  on_command["pause"] = [](int id) {
    std::cout << "robot " << id << ": paused\n";
  };

  on_command["dock"](4);   // robot 4: go to dock
  on_command["pause"](2);  // robot 2: paused
}
}  // namespace command_table

// [Slide 66] An Empty std::function
// operator[] inserts an empty std::function for the missing key, and calling
// an empty std::function throws std::bad_function_call. Nothing catches it, so
// the program stops with exit status 134.
namespace empty_function {
void run() {
  std::map<std::string, std::function<void(int)>> on_command;

  // The fix: find adds nothing. Call the handler only if find found one.
  auto handler{on_command.find("reboot")};
  if (handler != on_command.end()) { handler->second(2); }
  std::cout << "after find, entries: " << on_command.size() << '\n';  // 0

  on_command["reboot"](2);  // no handler was ever stored: [] adds one, the call throws
}
}  // namespace empty_function

// [Slide 67] Choosing a Parameter Type
namespace choosing {
void run() {
  double (*convert_ptr)(double){std_function::to_fraction};
  std::function<double(double)> convert_function{std_function::to_fraction};
  std::cout << "sizeof(convert_ptr): " << sizeof(convert_ptr) << ", sizeof(convert_function): " << sizeof(convert_function) << '\n';  // 8 32
}
}  // namespace choosing

// [Slide 68] std::bind
namespace bind {
double charge_time_h(double missing_pct, double rate_pct_per_h) {
  return missing_pct / rate_pct_per_h;
}

void run() {
  using namespace std::placeholders;                         // _1, _2, ...
  auto at_fast_dock = std::bind(charge_time_h, _1, 40.0);  // (x, 40.0)
  auto to_half = std::bind(charge_time_h, 50.0, _1);       // (50.0, x)
  auto swapped = std::bind(charge_time_h, _2, _1);         // (y, x)

  std::cout << "at_fast_dock(60.0): " << at_fast_dock(60.0) << '\n';    // 1.5
  std::cout << "to_half(25.0): " << to_half(25.0) << '\n';               // 2
  std::cout << "swapped(20.0, 60.0): " << swapped(20.0, 60.0) << '\n';   // 3
}
}  // namespace bind

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
    {52, "A Condition instead of a Value", condition::run},
    {64, "std::function", std_function::run},
    {65, "A Table of Commands", command_table::run},
    {66, "An Empty std::function", empty_function::run, true},
    {67, "Choosing a Parameter Type", choosing::run},
    {68, "std::bind", bind::run},
  };
}

}  // namespace higher_order
