/**
 * @file templates.cpp
 * @brief L6 Section 3, Function Templates: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_templates. This file stands alone.
 *
 * @code
 * 702build week6_templates
 * 702run week6_templates        # every slide of the section, in order
 * 702run week6_templates 37     # only [Slide 37]
 * nm -C build/project/week6/week6_templates | grep 'clamp_value<'
 * @endcode
 *
 * The template clamp_value is declared once, at namespace scope, because most
 * slides of the section call it, and because the Instantiation slide lists its
 * instantiations with nm. Code that does not compile, or does not link, is
 * commented out where its slide shows it: uncomment it, build, and you get the
 * slide's error.
 */
#include <concepts>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// [Slide 37] One Body, Several Overloads
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
  std::cout << "clamp_value(130, 0, 100): " << clamp_value(130, 0, 100)
            << ", clamp_value(104.2, 0.0, 100.0): " << clamp_value(104.2, 0.0, 100.0)
            << '\n';  // 100 100
}
}  // namespace overloads

// [Slide 38] Declaring a Template
template <typename T>
T clamp_value(T value, T low, T high) {
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

namespace declaring_template {
void run() {
  std::cout << "clamp_value(130, 0, 100): " << clamp_value(130, 0, 100)
            << ", clamp_value(104.2, 0.0, 100.0): " << clamp_value(104.2, 0.0, 100.0)
            << '\n';  // 100 100
}
}  // namespace declaring_template

// [Slide 39] Instantiation
namespace instantiation {
void run() {
  int speed_pct{clamp_value(130, 0, 100)};              // 100
  double battery_pct{clamp_value(104.2, 0.0, 100.0)};   // 100
  int other_pct{clamp_value(50, 0, 100)};               // 50
  std::cout << "speed_pct: " << speed_pct << ", battery_pct: " << battery_pct
            << ", other_pct: " << other_pct << '\n';
}
}  // namespace instantiation

// [Slide 40] Templates Go in Headers
// The slide uses three files. One file shows the same linker error: a template
// that is declared and never defined.
namespace in_headers {
template <typename T> T clamp_value(T value, T low, T high);  // the declaration only

void run() {
  // Does not link: undefined reference to clamp_value<double>.
  // double pct{clamp_value(104.2, 0.0, 100.0)};
  std::cout << "does not link: uncomment the line in in_headers::run()\n";
}
}  // namespace in_headers

// [Slide 43] One T for Every Argument
namespace one_t {
void run() {
  // Does not compile: 104 says T is int, 0.0 says double.
  // double pct{clamp_value(104, 0.0, 100.0)};
  std::cout << "does not compile: uncomment the line in one_t::run()\n";
}
}  // namespace one_t

// [Slide 44] Explicit Template Arguments
namespace explicit_args {
template <typename T>
T make_zero() { return T{}; }

void run() {
  int speed_reading{104};  // from a sensor, an int
  double pct{clamp_value<double>(speed_reading, 0.0, 100.0)};  // 100
  double zero{make_zero<double>()};                            // 0
  std::cout << "pct: " << pct << ", zero: " << zero << '\n';
  // Does not compile: deduced conflicting types for parameter 'T'.
  // double without_explicit{clamp_value(speed_reading, 0.0, 100.0)};
  // Does not compile: couldn't deduce template parameter 'T'.
  // double no_argument{make_zero()};
}
}  // namespace explicit_args

// [Slide 45] Two Template Parameters
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
  std::cout << "add_offset(80, 15): " << add_offset(80, 15)
            << ", add_offset(80, 2.5): " << add_offset(80, 2.5)
            << ", add_offset(80.5f, 2): " << add_offset(80.5f, 2) << '\n';  // 95 82.5 82.5
}
}  // namespace two_parameters

// [Slide 46] Abbreviated Templates
namespace abbreviated {
void print_all(const auto& values) {
  for (const auto& value : values) {
    std::cout << value << ' ';
  }
  std::cout << '\n';
}

void run() {
  std::cout << "robot ids: ";
  print_all(std::vector<int>{1, 2, 3, 4});                 // robot ids
  std::cout << "battery levels: ";
  print_all(std::vector<double>{82.5, 35.0});              // battery levels
}
}  // namespace abbreviated

// [Slide 47] Concepts
namespace constrained {
template <std::floating_point T>
T average_of(const std::vector<T>& values) {
  T sum{0};
  for (const T& value : values) { sum += value; }
  return sum / static_cast<T>(values.size());
}

void run() {
  std::cout << "average_of(doubles): "
            << average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0}) << '\n';  // 49.875
}
}  // namespace constrained

// [Slide 48] A Call That Compiles and Is Wrong
namespace unconstrained {
template <typename T>
T average_of(const std::vector<T>& values) {
  T sum{0};
  for (const T& value : values) { sum += value; }
  return sum / static_cast<T>(values.size());
}

void run() {
  std::cout << "average_of(doubles): "
            << average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0}) << '\n';  // 49.875
  std::cout << "average_of(ints): "
            << average_of(std::vector<int>{80, 35, 64, 18}) << '\n';  // 49, not 49.25
  // Does not compile: the constrained version (Slide 46) rejects int.
  // std::cout << constrained::average_of(std::vector<int>{80, 35, 64, 18}) << '\n';
}
}  // namespace unconstrained

// [Slide 49] Form 1: In Place of typename
namespace form1 {
template <std::integral T>  // T must be an integral type
bool is_valid_id(T id) { return id > 0; }

void run() {
  std::cout << "is_valid_id(3): " << is_valid_id(3)
            << ", is_valid_id(-2L): " << is_valid_id(-2L)
            << ", is_valid_id(true): " << is_valid_id(true) << '\n';  // 1 0 1
  // Does not compile: constraints not satisfied (double is not integral).
  // is_valid_id(2.5);
}
}  // namespace form1

// [Slide 50] Form 2: A requires Clause
namespace form2 {
template <typename T>
  requires std::integral<T> && (!std::same_as<T, bool>)
bool is_valid_id(T id) { return id > 0; }

void run() {
  std::cout << "is_valid_id(3): " << is_valid_id(3)
            << ", is_valid_id(-2L): " << is_valid_id(-2L) << '\n';  // 1 0
  // Does not compile: constraints not satisfied (T is bool).
  // is_valid_id(true);
}
}  // namespace form2

// [Slide 51] Form 3: Before auto
namespace form3 {
// The slide shows the same function twice, so each version gets a namespace.
namespace with_form1 {
// Form 1 names the type T, so both parameters must be one type.
template <std::integral T>
bool same_id(T first, T second) {
  return first == second;
}
}  // namespace with_form1

namespace with_form3 {
// Form 3 has no name: each auto parameter is its own type.
bool same_id(
    std::integral auto first,
    std::integral auto second) {
  return first == second;
}
}  // namespace with_form3

void run() {
  std::cout << "form 1, same_id(3, 3): " << with_form1::same_id(3, 3)
            << ", form 3, same_id(3, 3): " << with_form3::same_id(3, 3)
            << ", form 3, same_id(3, 3L): " << with_form3::same_id(3, 3L) << '\n';  // 1 1 1
  // Does not compile: deduced conflicting types for parameter 'T' ('int' and 'long int').
  // with_form1::same_id(3, 3L);
}
}  // namespace form3

// [Slide 52] Which Form to Use
namespace which_form {
// 1. Form 3 by default: each parameter has its own simple requirement
namespace with_form3 {
bool same_id(std::integral auto first, std::integral auto second) {
  return first == second;
}
}  // namespace with_form3

// 2. Form 1 when two parameters must be one type
namespace with_form1 {
template <std::integral T>
bool same_id(T first, T second) {
  return first == second;
}
}  // namespace with_form1

// 3. Form 2 when the condition joins tests
template <typename T>
  requires std::integral<T> && (!std::same_as<T, bool>)
bool is_valid_id(T id) {
  return id > 0;
}

void run() {
  std::cout << std::boolalpha << "form 3, same_id(3, 3L): " << with_form3::same_id(3, 3L)
            << ", form 1, same_id(3, 3): " << with_form1::same_id(3, 3)
            << ", is_valid_id(3): " << is_valid_id(3) << std::noboolalpha
            << '\n';  // true true true
}
}  // namespace which_form

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: see the appendix, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  const std::string header{"[Slide " + std::to_string(slide) + "] " + title};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 37, "One Body, Several Overloads", overloads::run);
  show(only, 38, "Declaring a Template", declaring_template::run);
  show(only, 39, "Instantiation", instantiation::run);
  show(only, 40, "Templates Go in Headers", in_headers::run);
  show(only, 43, "One T for Every Argument", one_t::run);
  show(only, 44, "Explicit Template Arguments", explicit_args::run);
  show(only, 45, "Two Template Parameters", two_parameters::run);
  show(only, 46, "Abbreviated Templates", abbreviated::run);
  show(only, 47, "Concepts", constrained::run);
  show(only, 48, "A Call That Compiles and Is Wrong", unconstrained::run);
  show(only, 49, "Form 1: In Place of typename", form1::run);
  show(only, 50, "Form 2: A requires Clause", form2::run);
  show(only, 51, "Form 3: Before auto", form3::run);
  show(only, 52, "Which Form to Use", which_form::run);
}
