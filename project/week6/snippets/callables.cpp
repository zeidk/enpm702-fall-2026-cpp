/**
 * @file callables.cpp
 * @brief L6 Section 5, Other Callables: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_callables. This file stands alone.
 *
 * @code
 * 702build week6_callables
 * 702run week6_callables        # every slide of the section, in order
 * 702run week6_callables 65    # only [Slide 65]
 * @endcode
 *
 * Code that does not compile is commented out where its slide shows it:
 * uncomment it, build, and you get the slide's error. Calling an empty
 * std::function is in @c ../throws/function_empty.cpp.
 */
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <source_location>
#include <string>
#include <string_view>

// [Slide 65] Function Pointers
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

// [Slide 66] Passing a Function
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

// [Slide 67] std::function
namespace std_function {
void run() {
  std::function<double(double)> convert{function_pointers::to_fraction};
  std::cout << "convert(64.0) with to_fraction: " << convert(64.0) << '\n';  // 0.64

  double scale{2.0};
  convert = [scale](double pct) { return scale * pct; };
  std::cout << "convert(64.0) with the lambda: " << convert(64.0) << '\n';  // 128
}
}  // namespace std_function

// [Slide 68] A Table of Commands
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

// [Slide 70] Choosing a Parameter Type
namespace choosing {
void run() {
  double (*convert_ptr)(double){function_pointers::to_fraction};
  std::function<double(double)> convert_function{function_pointers::to_fraction};
  std::cout << "sizeof(convert_ptr): " << sizeof(convert_ptr) << ", sizeof(convert_function): " << sizeof(convert_function) << '\n';  // 8 32
}
}  // namespace choosing

// [Slide 71] std::bind
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

// [Slide 72] bind_front and Lambdas
namespace bind_front {
void run() {
  auto to_half = std::bind_front(bind::charge_time_h, 50.0);  // C++20
  std::cout << "to_half(25.0): " << to_half(25.0) << '\n';  // 2

  auto fast_l = [](double missing_pct) { return bind::charge_time_h(missing_pct, 40.0); };
  auto swap_l = [](double rate_pct_per_h, double missing_pct) {
    return bind::charge_time_h(missing_pct, rate_pct_per_h);
  };
  std::cout << "fast_l(60.0): " << fast_l(60.0) << ", swap_l(20.0, 60.0): " << swap_l(20.0, 60.0)
            << '\n';  // 1.5 3

  using namespace std::placeholders;
  auto at_fast_dock = std::bind(bind::charge_time_h, _1, 40.0);
  // compiles, prints 1.5: 99.0 is dropped
  std::cout << "at_fast_dock(60.0, 99.0): " << at_fast_dock(60.0, 99.0) << '\n';
}
}  // namespace bind_front

// [Slide 74] std::source_location
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

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function, as on the Function Pointers slide.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  const std::string header{"[Slide " + std::to_string(slide) + "] " + title};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 65, "Function Pointers", function_pointers::run);
  show(only, 66, "Passing a Function", passing_function::run);
  show(only, 67, "std::function", std_function::run);
  show(only, 68, "A Table of Commands", command_table::run);
  show(only, 70, "Choosing a Parameter Type", choosing::run);
  show(only, 71, "std::bind", bind::run);
  show(only, 72, "bind_front and Lambdas", bind_front::run);
  show(only, 74, "std::source_location", source_location_slide::run);
}
