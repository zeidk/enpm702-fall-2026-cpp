/**
 * @file templates.cpp
 * @brief L6 Section 4, Function Templates: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week6_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground       # every slide of every section, in order
 * 702run week6_playground 40    # only [Slide 40]
 * @endcode
 *
 * Each slide's code is in its own namespace, inside the namespace @c templates, so
 * two slides, or two section files, can both declare a @c RobotStatus without
 * a clash. The comment above a namespace names its slide, and slides() at the
 * bottom lists them.
 *
 * The template clamp_value is declared once, at the top of the namespace
 * @c templates, because most slides of the section call it, and because the
 * Instantiation slide lists its instantiations with nm:
 * @code
 * nm -C build/project/week6/week6_playground | grep 'clamp_value<'
 * @endcode
 * Code that does not compile, or does not link, is commented out where its
 * slide shows it: uncomment it, build, and you get the slide's error.
 */
#include <concepts>
#include <iostream>
#include <string>
#include <vector>

#include "sections.hpp"

namespace templates {

// [Slide 40] One Body, Several Overloads
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

// [Slide 41] Declaring a Template
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

// [Slide 42] Instantiation
namespace instantiation {
void run() {
  int speed_pct{clamp_value(130, 0, 100)};              // T = int: 100
  double battery_pct{clamp_value(104.2, 0.0, 100.0)};   // T = double: 100
  int other_pct{clamp_value(50, 0, 100)};               // T = int: 50
  std::cout << "speed_pct: " << speed_pct << ", battery_pct: " << battery_pct
            << ", other_pct: " << other_pct << '\n';
}
}  // namespace instantiation

// [Slide 43] Template Argument Deduction
namespace deduction {
void run() {
  std::cout << "clamp_value(130, 0, 100): " << clamp_value(130, 0, 100)
            << '\n';  // three ints: T is int, 100
  // Does not compile: 104 says T is int, 0.0 says double.
  // note: deduced conflicting types for parameter 'T' ('int' and 'double')
  // clamp_value(104, 0.0, 100.0);
}
}  // namespace deduction

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

// [Slide 46] Templates Go in Headers
// On the slide, stats.hpp holds the whole template, body included, and
// main.cpp includes it; fleet/include/stats.hpp does the same. In this one
// file, clamp_value at the top of the namespace templates plays the part of
// that header: its body is above every call. The split that does not link is
// in week6_appendix, [Appendix xi] Where the Body Ends Up.
namespace in_headers {
void run() {
  double pct{clamp_value(104.2, 0.0, 100.0)};  // 100
  std::cout << "pct: " << pct << '\n';
}
}  // namespace in_headers

// [Slide 47] Abbreviated Templates
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

// [Slide 48] Concepts
namespace constrained {
template <std::floating_point T>
T half(T value) {
  return value / 2;
}

void run() {
  std::cout << "half(35.0): " << half(35.0) << '\n';  // 17.5
}
}  // namespace constrained

// [Slide 49] A Call That Compiles and Is Wrong
namespace unconstrained {
template <typename T>
T half(T value) {
  return value / 2;
}

void run() {
  std::cout << "half(35.0): " << half(35.0) << '\n';  // 17.5
  std::cout << "half(35): " << half(35) << '\n';      // 17, not 17.5
  // Does not compile: the constrained version (Slide 45) rejects int.
  // std::cout << constrained::half(35) << '\n';
}
}  // namespace unconstrained

// [Slide 50] Form 1: In Place of typename
namespace form1 {
template <std::integral T>  // T must be an integral type
bool is_valid_id(T id) { return id > 0; }

void run() {
  std::cout << "is_valid_id(3): " << is_valid_id(3)
            << ", is_valid_id(true): " << is_valid_id(true) << '\n';  // 1 1
  // Does not compile: constraints not satisfied (double is not integral).
  // is_valid_id(2.5);
}
}  // namespace form1

// [Slide 51] Form 2: A requires Clause
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

// [Slide 52] Form 3: Before auto
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

// [Slide 53] Which Form to Use
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

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
    {40, "One Body, Several Overloads", overloads::run},
    {41, "Declaring a Template", declaring_template::run},
    {42, "Instantiation", instantiation::run},
    {43, "Template Argument Deduction", deduction::run},
    {44, "Explicit Template Arguments", explicit_args::run},
    {45, "Two Template Parameters", two_parameters::run},
    {46, "Templates Go in Headers", in_headers::run},
    {47, "Abbreviated Templates", abbreviated::run},
    {48, "Concepts", constrained::run},
    {49, "A Call That Compiles and Is Wrong", unconstrained::run},
    {50, "Form 1: In Place of typename", form1::run},
    {51, "Form 2: A requires Clause", form2::run},
    {52, "Form 3: Before auto", form3::run},
    {53, "Which Form to Use", which_form::run},
  };
}

}  // namespace templates
