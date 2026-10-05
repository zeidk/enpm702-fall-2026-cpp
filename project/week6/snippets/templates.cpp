/**
 * @file templates.cpp
 * @brief L6 Section 3, Function Templates: the code of every slide, runnable.
 *
 * @details Build target: @c week6_templates. This file stands alone.
 *
 * @code
 * 702build week6_templates
 * 702run week6_templates        # every slide of the section, in order
 * 702run week6_templates 35     # only [Slide 35]
 * nm -C build/project/week6/week6_templates | grep 'clamp_value<'
 * @endcode
 *
 * The template clamp_value is declared once, at namespace scope, because most
 * slides of the section call it, and because the Instantiation slide lists its
 * instantiations with nm. Code that does not compile is left out; its slide
 * shows the error.
 */
#include <concepts>
#include <cstdlib>
#include <iostream>
#include <vector>

// [Slide 35] One Body, Several Overloads
namespace overloads {
int clamp_value(int value, int low, int high) {  // a speed command, in percent
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

double clamp_value(double value, double low, double high) {  // a battery reading
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

void run() {
  std::cout << clamp_value(130, 0, 100) << ' ' << clamp_value(104.2, 0.0, 100.0)
            << '\n';  // 100 100
}
}  // namespace overloads

// [Slide 36] Declaring a Template
template <typename T>
T clamp_value(T value, T low, T high) {
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

namespace declaring_template {
void run() {
  std::cout << clamp_value(130, 0, 100) << ' ' << clamp_value(104.2, 0.0, 100.0)
            << '\n';  // 100 100
}
}  // namespace declaring_template

// [Slide 37] Instantiation
namespace instantiation {
void run() {
  int speed_pct{clamp_value(130, 0, 100)};              // 100
  double battery_pct{clamp_value(104.2, 0.0, 100.0)};   // 100
  int other_pct{clamp_value(50, 0, 100)};               // 50
  std::cout << speed_pct << ' ' << battery_pct << ' ' << other_pct << '\n';
}
}  // namespace instantiation

// [Slide 41] Explicit Template Arguments
namespace explicit_args {
void run() {
  double pct{clamp_value<double>(104, 0.0, 100.0)};  // 100
  std::cout << pct << '\n';
}
}  // namespace explicit_args

// [Slide 43] Two Template Parameters
namespace two_parameters {
template <typename T, typename U>
auto add_offset(T value, U offset) {
  return value + offset;
}

// The three return types, checked while compiling.
static_assert(std::same_as<decltype(add_offset(80, 15)), int>);
static_assert(std::same_as<decltype(add_offset(80, 2.5)), double>);
static_assert(std::same_as<decltype(add_offset(80.5f, 2)), float>);

void run() {
  std::cout << add_offset(80, 15) << ' ' << add_offset(80, 2.5) << ' ' << add_offset(80.5f, 2)
            << '\n';  // 95 82.5 82.5
}
}  // namespace two_parameters

// [Slide 44] Abbreviated Templates
namespace abbreviated {
void print_all(const auto& values) {
  for (const auto& v : values) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
}

void run() {
  print_all(std::vector<int>{1, 2, 3, 4});                 // robot ids
  print_all(std::vector<double>{82.5, 35.0});              // battery levels
}
}  // namespace abbreviated

// [Slide 45] Concepts
namespace constrained {
template <std::floating_point T>
T average_of(const std::vector<T>& values) {
  T sum{0};
  for (const T& v : values) { sum += v; }
  return sum / static_cast<T>(values.size());
}

void run() {
  std::cout << average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0}) << '\n';  // 49.875
}
}  // namespace constrained

// [Slide 46] A Call That Compiles and Is Wrong
// The constrained version rejects the int call, as the slide shows.
namespace unconstrained {
template <typename T>
T average_of(const std::vector<T>& values) {
  T sum{0};
  for (const T& v : values) { sum += v; }
  return sum / static_cast<T>(values.size());
}

void run() {
  std::cout << average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0}) << '\n';  // 49.875
  std::cout << average_of(std::vector<int>{80, 35, 64, 18}) << '\n';  // 49, not 49.25
}
}  // namespace unconstrained

// [Slide 47] Three Ways to Write a Constraint
// The three forms accept the same calls, so each sits in its own namespace.
namespace form1 {
template <std::integral T>  // 1. in place of typename
bool is_valid_id(T id) { return id > 0; }
}  // namespace form1

namespace form2 {
template <typename T>
  requires std::integral<T>  // 2. a requires clause
bool is_valid_id(T id) { return id > 0; }
}  // namespace form2

namespace form3 {
// 3. before auto
bool is_valid_id(std::integral auto id) { return id > 0; }
}  // namespace form3

namespace constraint_forms {
void run() {
  std::cout << form1::is_valid_id(3) << ' ' << form2::is_valid_id(0) << ' '
            << form3::is_valid_id(-2L) << '\n';  // 1 0 0
}
}  // namespace constraint_forms

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: Section 5, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  std::cout << "[Slide " << slide << "] " << title << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 35, "One Body, Several Overloads", overloads::run);
  show(only, 36, "Declaring a Template", declaring_template::run);
  show(only, 37, "Instantiation", instantiation::run);
  show(only, 41, "Explicit Template Arguments", explicit_args::run);
  show(only, 43, "Two Template Parameters", two_parameters::run);
  show(only, 44, "Abbreviated Templates", abbreviated::run);
  show(only, 45, "Concepts", constrained::run);
  show(only, 46, "A Call That Compiles and Is Wrong", unconstrained::run);
  show(only, 47, "Three Ways to Write a Constraint", constraint_forms::run);
}
