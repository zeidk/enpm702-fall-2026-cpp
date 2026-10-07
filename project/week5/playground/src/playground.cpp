/**
 * @file playground.cpp
 * @brief L5, Functions: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week5_playground.
 *
 * @code
 * 702build week5_playground
 * 702run week5_playground        # every slide, in order
 * 702run week5_playground 12     # only [Slide 12]
 * @endcode
 *
 * Each slide's code is in its own namespace, with a @c run() that does what
 * the slide does in @c main(). Two slides can then both define a function such
 * as @c clamp_joint without a clash. @c "[Slide N]" is the number in the
 * top-left corner of the slide. The table in main() lists the slides, and
 * run_slides() in @c ../../common/slides.cpp runs them. The appendix frames
 * are a program of their own: @c ../../appendix/, target @c week5_appendix.
 *
 * Code that does not compile is commented out where its slide shows it:
 * uncomment it, build, and you get the slide's error. The same goes for a
 * line that only warns. Code with undefined behavior is in
 * @c ../../undefined/undefined.cpp, built with AddressSanitizer and
 * UndefinedBehaviorSanitizer. A full run skips those slides; ask for one by
 * its number to run it, as in @c "702run week5_playground 31".
 *
 * The arm program split into headers and source files is in
 * @c ../../arm_demo/ (target @c week5_arm_demo). The Header Files slides and
 * the Documenting Functions section use it.
 *
 * @note Every number in the comments was measured with g++ 13.3,
 *       @c -std=c++20, on 2026-10-06. Addresses differ on every run and on
 *       every machine; only which ones are equal is the point.
 */
#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <numbers>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "slides.hpp"
#include "undefined.hpp"

// =============================================================================
// Functions
// =============================================================================

// [Slide 7] Without Functions
namespace without_functions {
constexpr double max_deg{170.0};

void copy_the_logic() {
  double q2{-200.0};  // elbow
  if (q2 > max_deg) { q2 = max_deg; }
  if (q2 < -max_deg) { q2 = -max_deg; }
  double q3{-250.0};  // wrist, copied
  if (q3 > max_deg) { q3 = max_deg; }
  if (q3 < -max_deg) { q2 = -max_deg; }  // the bug: q2, not q3
  std::cout << "without a function, q2: " << q2 << ", q3: " << q3
            << '\n';  // -170 -250: the wrist is not clamped
}

// With a Function
double clamp_joint(double deg) {
  if (deg > max_deg) { return max_deg; }
  if (deg < -max_deg) { return -max_deg; }
  return deg;
}

void call_the_function() {
  double q2{clamp_joint(-200.0)};  // elbow
  double q3{clamp_joint(-250.0)};  // wrist
  std::cout << "with a function, q2: " << q2 << ", q3: " << q3 << '\n';  // -170 -170
}

void run() {
  copy_the_logic();
  call_the_function();
}
}  // namespace without_functions

// [Slide 8] Anatomy of a Function
namespace anatomy {
double convert_deg_to_rad(double deg) {
  return deg * std::numbers::pi / 180.0;  // C++20, <numbers>
}

void run() {
  std::cout << "convert_deg_to_rad(180.0): " << convert_deg_to_rad(180.0) << '\n';  // 3.14159
}
}  // namespace anatomy

// [Slide 9] Anatomy of a Function
namespace function_header {
double convert_deg_to_rad(double deg)  // the header
{
  return deg * std::numbers::pi / 180.0;  // the body
}

void run() {
  std::cout << "convert_deg_to_rad(90.0): " << convert_deg_to_rad(90.0) << '\n';  // 1.5708
}
}  // namespace function_header

// [Slide 10] Anatomy of a Function
namespace signature {
namespace robot {
constexpr double convert_deg_to_rad(double deg);
}
// signature: robot::convert_deg_to_rad(double)
// In this file robot sits inside the namespace signature, so the full
// signature is signature::robot::convert_deg_to_rad(double).

namespace robot {
constexpr double convert_deg_to_rad(double deg) { return deg * std::numbers::pi / 180.0; }
}  // namespace robot

void run() {
  std::cout << "robot::convert_deg_to_rad(90.0): " << robot::convert_deg_to_rad(90.0)
            << '\n';  // 1.5708
}
}  // namespace signature

// [Slide 11] Parameters and Arguments
namespace parameters {
void print_velocities(double linear, double angular) {  // parameters
  std::cout << linear << ' ' << angular << '\n';
}

void run() {
  std::cout << "print_velocities(0.5, 0.1): ";
  print_velocities(0.5, 0.1);  // arguments: prints 0.5 0.1
}
}  // namespace parameters

// [Slide 12] Declaration and Definition
namespace declaration_definition {
constexpr double max_deg{170.0};

double clamp_joint(double deg);  // declaration (a prototype)

double clamp_joint(double deg) {  // definition
  return std::clamp(deg, -max_deg, max_deg);
}

void run() {
  std::cout << "clamp_joint(200.0): " << clamp_joint(200.0) << '\n';  // 170
}
}  // namespace declaration_definition

// [Slide 13] Declaration Order
namespace declaration_order {
namespace without_declaration {
// Does not compile: error: 'print_limits' was not declared in this scope
// void report_arm() {
//   std::cout << "arm: ";
//   print_limits();  // not seen yet
// }
// void print_limits() {
//   std::cout << "170 deg\n";
// }
}  // namespace without_declaration

void print_limits();  // the promise
void report_arm() {
  std::cout << "arm: ";
  print_limits();  // OK
}
void print_limits() {
  std::cout << "170 deg\n";
}

void run() {
  std::cout << "report_arm(): ";
  report_arm();  // arm: 170 deg
}
}  // namespace declaration_order

// [Slide 14] No Order Works
namespace no_order_works {
constexpr double max_deg{170.0};

double clamp_joint(double deg) { return std::clamp(deg, -max_deg, max_deg); }

namespace without_declaration {
// Does not compile: error: 'retry_move' was not declared in this scope; did you
//                   mean 'remove'?
// void move_joint(double deg) {
//   if (deg > max_deg) {
//     retry_move(deg);// not seen yet
//     return;
//   }
//   // drive the motor to deg
// }
// void retry_move(double deg) {
//   move_joint(clamp_joint(deg));
// }
}  // namespace without_declaration

// One Declaration
void retry_move(double deg);
void move_joint(double deg) {
  if (deg > max_deg) {
    retry_move(deg);  // OK
    return;
  }
  // drive the motor to deg
  std::cout << "driving to " << deg << '\n';
}
void retry_move(double deg) {
  move_joint(clamp_joint(deg));
}

void run() {
  std::cout << "move_joint(200.0): ";
  move_joint(200.0);  // driving to 170: clamped, then driven
}
}  // namespace no_order_works

// [Slide 15] Declaration and Definition
namespace every_function_declared {
constexpr double max_deg{170.0};

double clamp_joint(double deg);
void move_joint(double deg);
void retry_move(double deg);
void retry_move(double deg) {
  move_joint(clamp_joint(deg));
}
void move_joint(double deg) {
  if (deg > max_deg) {
    retry_move(deg);
    return;
  }
  std::cout << "driving to " << deg << '\n';  // not on the slide: shows the call ran
}
double clamp_joint(double deg) {
  return std::clamp(deg, -max_deg, max_deg);
}

void run() {
  std::cout << "move_joint(200.0): ";
  move_joint(200.0);  // driving to 170
}
}  // namespace every_function_declared

// [Slide 16] A Missing Definition
namespace missing_definition {
double clamp_joint(double deg);  // promised, never delivered

void run() {
  // Compiles, then does not link:
  //   undefined reference to `missing_definition::clamp_joint(double)'
  //   collect2: error: ld returned 1 exit status
  // std::cout << clamp_joint(200.0) << '\n';
  std::cout << "clamp_joint is declared and never defined: the call is commented out\n";
}
}  // namespace missing_definition

// Slides 20 to 27, Header Files: their code is split into headers and source
// files, so it is the arm program in ../../arm_demo/, not code in this file.

// [Slide 29] Calling and Returning
namespace calling_returning {
void print_limits() {
  std::cout << "170 deg\n";
}
void report_arm() {
  std::cout << "arm: ";
  print_limits();
}

void run() {
  report_arm();               // arm: 170 deg
  std::cout << "exit main\n";
}
}  // namespace calling_returning

// [Slide 30] In a void Function
namespace return_statement {
void print_range(double m) {
  if (m < 0.0) {
    std::cout << "invalid\n";
    return;  // leave early
  }
  std::cout << m << " m\n";
}  // returns here otherwise

// With a Value
int calculate_sum(int a, int b) {
  int result{a + b};
  return result;
}

void run() {
  std::cout << "print_range(-1.0): ";
  print_range(-1.0);  // invalid
  std::cout << "print_range(2.5): ";
  print_range(2.5);   // 2.5 m

  int sum{calculate_sum(5, 3)};  // 8
  std::cout << "sum: " << sum << '\n';
}
}  // namespace return_statement

// [Slide 31] Missing Returns
// Undefined behavior: the code is undefined::missing_return in
// ../../undefined/undefined.cpp, built with the sanitizers.

// [Slide 32] Conversion on Return
namespace conversion_on_return {
// Silent under the course flags. Add -Wconversion and g++ prints:
//   warning: conversion from 'double' to 'int' may change value [-Wfloat-conversion]
int truncate_value() {
  double value{99.99};
  return value;  // converted to int: 99
}

void run() {
  std::cout << "truncate_value(): " << truncate_value() << '\n';  // 99
}
}  // namespace conversion_on_return

// [Slide 33] Calling and Returning
namespace nodiscard_attribute {
constexpr double max_deg{170.0};

[[nodiscard]] double clamp_joint(double deg) {
  return std::clamp(deg, -max_deg, max_deg);
}

void run() {
  // Warns: ignoring return value of 'double nodiscard_attribute::clamp_joint(double)',
  //        declared with attribute 'nodiscard' [-Wunused-result]
  // clamp_joint(200.0);  // result thrown away
  std::cout << "clamp_joint(200.0): " << clamp_joint(200.0) << '\n';  // 170

  std::vector<double> v{1.0};
  // Warns: ignoring return value of 'constexpr bool std::vector<_Tp, _Alloc>::empty() const
  //        [with _Tp = double; _Alloc = std::allocator<double>]', declared with attribute
  //        'nodiscard' [-Wunused-result]
  // v.empty();
  std::cout << std::boolalpha << "v.empty(): " << v.empty() << ", v.size(): " << v.size()
            << std::noboolalpha << '\n';  // false 1: empty() only asks
}
}  // namespace nodiscard_attribute

// =============================================================================
// Passing Arguments
// =============================================================================

// [Slide 36] Pass by Value
namespace pass_by_value {
void nudge_joint(double deg) {  // double deg{q2};
  deg += 10.0;                  // changes the copy
}

void run() {
  double q2{5.0};
  nudge_joint(q2);
  std::cout << "q2: " << q2 << '\n';  // 5
}
}  // namespace pass_by_value

// [Slide 37] The Cost of a Copy
namespace cost_of_copy {
double average_angle(std::vector<double> angles) {  // a copy
  double sum{0.0};
  for (double a : angles) { sum += a; }
  return sum / angles.size();
}

void run() {
  std::vector<double> path(1'000'000);
  std::cout << "average_angle(path): " << average_angle(path)
            << '\n';  // 0, after 8 MB allocated and copied
  std::cout << "bytes copied: " << path.size() * sizeof(double) << '\n';  // 8000000
}
}  // namespace cost_of_copy

// [Slide 38] Pass by Reference
namespace pass_by_reference {
void nudge_joint(double& deg) {  // double& deg{q2};
  deg += 10.0;                   // changes q2
}

void run() {
  double q2{5.0};
  nudge_joint(q2);
  std::cout << "q2: " << q2 << '\n';  // 15

  // Does not compile: error: cannot bind non-const lvalue reference of type
  //                   'double&' to an rvalue of type 'double'
  // nudge_joint(5.0);
}
}  // namespace pass_by_reference

// [Slide 39] A Swap Function
namespace swap_function {
namespace by_value {
void swap_deg(double a, double b) {
  double tmp{a};
  a = b;
  b = tmp;
}  // swapped two copies
}  // namespace by_value

namespace by_reference {
void swap_deg(double& a, double& b) {
  double tmp{a};
  a = b;
  b = tmp;
}  // swapped q2 and q3
}  // namespace by_reference

void run() {
  {
    using by_value::swap_deg;
    double q2{1.0};
    double q3{2.0};
    swap_deg(q2, q3);  // q2 1, q3 2
    std::cout << "by value, q2: " << q2 << ", q3: " << q3 << '\n';
  }
  {
    using by_reference::swap_deg;
    double q2{1.0};
    double q3{2.0};
    swap_deg(q2, q3);  // q2 2, q3 1
    std::cout << "by reference, q2: " << q2 << ", q3: " << q3 << '\n';
  }
}
}  // namespace swap_function

// [Slide 40] Pass by const Reference
namespace const_reference {
double average_angle(const std::vector<double>& angles) {
  double sum{0.0};
  for (double a : angles) { sum += a; }
  // Does not compile: error: passing 'const std::vector<double>' as 'this' argument
  //                   discards qualifiers [-fpermissive]
  // angles.push_back(0.0);
  return sum / angles.size();
}

void run() {
  std::vector<double> path(1'000'000, 1.0);
  std::cout << "average_angle(path): " << average_angle(path) << '\n';  // 1, no copy
  std::cout << "average_angle({1.0, 2.0}): " << average_angle({1.0, 2.0})
            << '\n';  // 1.5: a temporary is accepted too
}
}  // namespace const_reference

// [Slide 41] String Parameters
namespace string_parameters {
void log_joint(std::string_view name);  // C++17, <string_view>

void log_joint(std::string_view name) { std::cout << name << '\n'; }

void run() {
  std::string joint{"elbow"};
  std::cout << "log_joint(joint): ";
  log_joint(joint);    // views the string: no copy
  std::cout << "log_joint(\"elbow\"): ";
  log_joint("elbow");  // views the literal: no std::string is built
}
}  // namespace string_parameters

// [Slide 42] Pass by const Reference
namespace string_view_memory {
// Prints what the view holds: where its characters are, and how many.
void log_joint(std::string_view name) {
  std::cout << "  name.data(): " << static_cast<const void*>(name.data())
            << ", name.size(): " << name.size() << ", sizeof(name): " << sizeof(name) << '\n';
}

void run() {
  std::string joint{"elbow"};
  std::cout << "joint.data(): " << static_cast<const void*>(joint.data()) << '\n';
  std::cout << "log_joint(joint):\n";
  log_joint(joint);    // views the string: no copy
  std::cout << "log_joint(\"elbow\"):\n";
  log_joint("elbow");  // views the literal: no std::string is built
  // The first data() is joint.data(), on the stack (0x7ff...). The second is
  // the literal in .rodata (0x55...). The size is 5 and sizeof is 16 both times.
}
}  // namespace string_view_memory

// [Slide 43] Pass by const Reference
namespace span_parameter {
double average_angle(std::span<const double> angles) {
  double sum{0.0};
  for (double a : angles) { sum += a; }
  return sum / angles.size();
}

void run() {
  std::vector<double> angles{10.0, 20.0, 60.0};
  std::cout << "average_angle(angles): " << average_angle(angles) << '\n';  // 30
}
}  // namespace span_parameter

// [Slide 44] One Parameter, Three Sequences
namespace three_sequences {
using span_parameter::average_angle;

void run() {
  double c_array[]{1.0, 2.0, 3.0};
  std::array<double, 2> arr{4.0, 6.0};
  std::vector<double> vec{1.5, 2.5, 3.5, 4.5};
  std::cout << "average_angle(c_array): " << average_angle(c_array) << '\n';  // 2
  std::cout << "average_angle(arr): " << average_angle(arr) << '\n';          // 5
  std::cout << "average_angle(vec): " << average_angle(vec) << '\n';          // 3
}
}  // namespace three_sequences

// [Slide 45] A Span in Memory
namespace span_memory {
using span_parameter::average_angle;

void run() {
  std::array<double, 2> arr{4.0, 6.0};
  std::cout << "average_angle(arr): " << average_angle(arr) << '\n';  // 5: angles is built from arr

  std::span<const double> angles{arr};  // the span the call builds
  std::cout << "arr.data(): " << arr.data() << ", angles.data(): " << angles.data()
            << '\n';  // the same address
  std::cout << "angles.size(): " << angles.size() << ", sizeof(angles): " << sizeof(angles)
            << '\n';  // 2 16
}
}  // namespace span_memory

// [Slide 46] Pass by Pointer
namespace pass_by_pointer {
void nudge_joint(double* p) {  // double* p{&q2};
  if (p != nullptr) {
    *p += 10.0;                // changes q2
  }
}

void run() {
  double q2{5.0};
  nudge_joint(&q2);
  std::cout << "q2: " << q2 << '\n';  // 15
  nudge_joint(nullptr);  // does nothing
  std::cout << "after nudge_joint(nullptr), q2: " << q2 << '\n';  // 15
}
}  // namespace pass_by_pointer

// [Slide 48] Exercise 2: Four Calls
namespace exercise_2 {
// Write your answer before you run it. Without the two pragma lines, g++
// prints a warning that gives one answer away:
//   warning: parameter 'x' set but not used [-Wunused-but-set-parameter]
//   warning: parameter 'p' set but not used [-Wunused-but-set-parameter]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
void f1(int x) { x = 99; }
void f2(int& x) { x = 99; }
void f3(int* p) { p = nullptr; }
void f4(int* p) { *p = 99; }
#pragma GCC diagnostic pop

void run() {
  int a{1};
  int b{1};
  int c{1};
  int d{1};
  f1(a);  f2(b);  f3(&c);  f4(&d);
  std::cout << "a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << '\n';
}
}  // namespace exercise_2

// =============================================================================
// Returning Values
// =============================================================================

// [Slide 50] Return by Value
namespace return_by_value {
constexpr double max_deg{170.0};

double clamp_joint(double deg) {
  return std::clamp(deg, -max_deg, max_deg);
}

void run() {
  double q2{clamp_joint(200.0)};  // 170
  std::cout << "q2: " << q2 << '\n';
}
}  // namespace return_by_value

// [Slide 51] A Large Result
namespace large_result {
std::vector<double> plan_path() {
  std::vector<double> angles(1'000'000);
  // ... fill it, one step per millisecond ...
  // Sweep the joint from 0 to 90 degrees over 1000 seconds.
  const double last{static_cast<double>(angles.size() - 1)};
  for (std::size_t i{0}; i < angles.size(); ++i) {
    angles[i] = 90.0 * static_cast<double>(i) / last;
  }
  return angles;  // copy a million doubles?
}

void run() {
  std::vector<double> path{plan_path()};
  std::cout << "path.size(): " << path.size() << '\n';  // 1000000
  std::cout << "path.front(): " << path.front() << ", middle: " << path[path.size() / 2]
            << ", path.back(): " << path.back() << '\n';  // 0 45 90
}
}  // namespace large_result

// [Slide 53] Measured Elision
namespace measured_elision {
std::vector<double> make_path() {
  std::vector<double> v(1000);
  std::cout << "  inside make_path, &v: " << &v << ", v.data(): " << v.data() << '\n';
  return v;  // a named local
}

void run() {
  std::vector<double> path{make_path()};
  std::cout << "  in the caller, &path: " << &path << ", path.data(): " << path.data() << '\n';
  // The same two addresses on both lines: elided. Build with
  // -fno-elide-constructors and &path differs while data() does not: moved.
}
}  // namespace measured_elision

// [Slide 54] Return by Reference
namespace return_by_reference {
double& get_joint(std::vector<double>& q, std::size_t i) {
  return q.at(i);
}

void run() {
  std::vector<double> q{0.0, 0.5, 1.0};
  get_joint(q, 1) = 0.7;  // q is now 0 0.7 1
  std::cout << "q: " << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';
}
}  // namespace return_by_reference

// [Slide 55] Returning a Local
// Undefined behavior: the code is undefined::returning_local in
// ../../undefined/undefined.cpp, built with the sanitizers.

// [Slide 56] Returning a Parameter
// Undefined behavior: the code is undefined::returning_parameter in
// ../../undefined/undefined.cpp, built with the sanitizers.

// [Slide 57] Return by Pointer
namespace return_by_pointer {
double* find_value(std::vector<double>& v, double target) {
  for (double& x : v) {
    if (x == target) { return &x; }
  }
  return nullptr;  // not found
}

void run() {
  std::vector<double> q{0.0, 0.5, 1.0};
  double* p{find_value(q, 0.5)};
  if (p != nullptr) { *p = 0.7; }  // q is now 0 0.7 1
  std::cout << "q: " << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';
  std::cout << std::boolalpha << "find_value(q, 9.9) == nullptr: "
            << (find_value(q, 9.9) == nullptr) << std::noboolalpha << '\n';  // true
}
}  // namespace return_by_pointer

// [Slide 58] Returning the Address of a Local
// Undefined behavior: the code is undefined::returning_address in
// ../../undefined/undefined.cpp, built with the sanitizers.

// =============================================================================
// Function Overloading
// =============================================================================

// [Slide 60] One Name, Three Functions
namespace overloading {
void print_pose(double x, double y);              // position
void print_pose(double x, double y, double deg);  // with heading
void print_pose(std::string_view name,
                double x, double y);              // labeled

void print_pose(double x, double y) { std::cout << x << ' ' << y << '\n'; }
void print_pose(double x, double y, double deg) {
  std::cout << x << ' ' << y << ' ' << deg << " deg\n";
}
void print_pose(std::string_view name, double x, double y) {
  std::cout << name << ": " << x << ' ' << y << '\n';
}

void run() {
  print_pose(0.42, 1.17);           // the two-value version: 0.42 1.17
  print_pose(0.42, 1.17, 30.0);     // the three-value version: 0.42 1.17 30 deg
  print_pose("wrist", 0.42, 1.17);  // the labeled version: wrist: 0.42 1.17
}
}  // namespace overloading

// [Slide 61] Valid Overloads
namespace valid_overloads {
void move_joint(int id);
void move_joint(int id, int deg);
void move_joint(double deg);
void move_joint(int id, double deg);
void move_joint(double deg, int id);

void move_joint(int id) { std::cout << "move_joint(int): id " << id << '\n'; }
void move_joint(int id, int deg) {
  std::cout << "move_joint(int, int): id " << id << ", deg " << deg << '\n';
}
void move_joint(double deg) { std::cout << "move_joint(double): deg " << deg << '\n'; }
void move_joint(int id, double deg) {
  std::cout << "move_joint(int, double): id " << id << ", deg " << deg << '\n';
}
void move_joint(double deg, int id) {
  std::cout << "move_joint(double, int): deg " << deg << ", id " << id << '\n';
}

namespace return_type_only {
// Does not compile: error: ambiguating new declaration of
//                   'double valid_overloads::return_type_only::joint_count()'
// int joint_count() { return 3; }
// double joint_count() { return 3.0; }
}  // namespace return_type_only

void run() {
  move_joint(2);        // move_joint(int)
  move_joint(2, 30);    // move_joint(int, int)
  move_joint(30.0);     // move_joint(double)
  move_joint(2, 30.0);  // move_joint(int, double)
  move_joint(30.0, 2);  // move_joint(double, int)
}
}  // namespace valid_overloads

// [Slide 63] Exercise 3: Overload Resolution
namespace exercise_3 {
int add(int a, int b) { return a + b; }
int add(int a, float b) { return a + b; }
int add(int a, double b) { return a + b; }

void run() {
  float f{3.5};
  long n{3};
  unsigned int u{3};
  // For each line, name the version that is called and what it prints, or
  // say why it does not compile. 'h' is 104.
  std::cout << "add(2, 3): " << add(2, 3) << '\n';              // line 1
  std::cout << "add(2, f): " << add(2, f) << '\n';              // line 2
  std::cout << "add(2.5, 3): " << add(2.5, 3) << '\n';          // line 3
  std::cout << "add('h', false): " << add('h', false) << '\n';  // line 4
  // Uncomment to check your answer for line 5:
  // std::cout << add(2, n) << '\n';        // line 5
  // Uncomment to check your answer for line 6:
  // std::cout << add(2, u) << '\n';        // line 6
  std::cout << "n: " << n << ", u: " << u << '\n';  // 3 3: n and u are used above only in comments
}
}  // namespace exercise_3

// =============================================================================
// Default Arguments
// =============================================================================

// [Slide 65] Filling from the Right
namespace filling_from_right {
void print_pose(double x, double y, int precision = 3,
                std::string_view label = "tool");

void print_pose(double x, double y, int precision, std::string_view label) {
  std::cout << label << ' ' << x << ' ' << y << " (" << precision << " dp)\n";
}

void run() {
  print_pose(0.42, 1.17, 2, "wrist");  // 2 dp, wrist
  print_pose(0.42, 1.17, 2);           // 2 dp, tool
  print_pose(0.42, 1.17);              // 3 dp, tool
  // Does not compile: error: too few arguments to function 'void
  //                   filling_from_right::print_pose(double, double, int, std::string_view)'
  // print_pose(0.42);
}
}  // namespace filling_from_right

// [Slide 67] Defaults in the Declaration
namespace defaults_declaration {
// kinematics.hpp
void print_pose(double x, double y,
                int precision = 3);
// kinematics.cpp
void print_pose(double x, double y,
                int precision) {
  // ...
  std::cout << x << ' ' << y << " (" << precision << " dp)\n";
}

namespace repeated {
// #include "kinematics.hpp" brings in this declaration:
void print_pose(double x, double y, int precision = 3);
// Does not compile: error: default argument given for parameter 3 of 'void
//                   defaults_declaration::repeated::print_pose(double, double, int)'
//                   [-fpermissive]
// void print_pose(double x, double y,
//                 int precision = 3) {
//   // ...
// }
}  // namespace repeated

void run() {
  print_pose(0.42, 1.17);  // 0.42 1.17 (3 dp): the default from the declaration
}
}  // namespace defaults_declaration

// [Slide 68] Two Functions (Overloads)
namespace overloads_or_default {
namespace two_functions {
void print_pose(double x, double y);
void print_pose(double x, double y,
                int precision);

void print_pose(double x, double y) { print_pose(x, y, 3); }
void print_pose(double x, double y, int precision) {
  std::cout << x << ' ' << y << " (" << precision << " dp)\n";
}
}  // namespace two_functions

// One Function (Default Argument)
namespace one_function {
void print_pose(double x, double y,
                int precision = 3);

void print_pose(double x, double y, int precision) {
  std::cout << x << ' ' << y << " (" << precision << " dp)\n";
}
}  // namespace one_function

void run() {
  std::cout << "two functions: ";
  two_functions::print_pose(0.42, 1.17);  // 0.42 1.17 (3 dp)
  std::cout << "one function: ";
  one_function::print_pose(0.42, 1.17);   // 0.42 1.17 (3 dp)
}
}  // namespace overloads_or_default

// =============================================================================
// Static Local Variables
// =============================================================================

// [Slide 70] Lifetime and Scope
namespace static_lifetime {
int next_move_id() {
  static int id{0};
  return ++id;
}

void run() {
  std::cout << "next_move_id(): " << next_move_id() << '\n';  // 1
  std::cout << "next_move_id(): " << next_move_id() << '\n';  // 2
  std::cout << "next_move_id(): " << next_move_id() << '\n';  // 3
}
}  // namespace static_lifetime

// [Slide 71] One-time Initialization
namespace one_time_init {
int count_calls() {
  static int calls{0};  // once
  ++calls;
  return calls;  // 1, 2, 3
}
int count_calls_wrong() {
  static int calls;
  calls = 0;  // every call
  ++calls;
  return calls;  // always 1
}

void run() {
  std::cout << "count_calls() three times: " << count_calls() << ' ' << count_calls() << ' '
            << count_calls() << '\n';  // 1 2 3
  std::cout << "count_calls_wrong() three times: " << count_calls_wrong() << ' '
            << count_calls_wrong() << ' ' << count_calls_wrong() << '\n';  // 1 1 1
}
}  // namespace one_time_init

// =============================================================================
// The Call Stack
// =============================================================================

// [Slide 73] Stack Frames
namespace stack_frames {
void C() { }
void B() { C(); }
void A() { B(); }

void run() {
  A();
  std::cout << "A() called B(), B() called C(), and all three returned\n";
}
}  // namespace stack_frames

// [Slide 74]
namespace exercise_4 {
// The frame has no title. It shows this program with line numbers, for the
// call stack exercise: put a breakpoint on "x += (y + *z) * (first + second);"
// and read the CALL STACK panel. scale and calls are in no frame.
constexpr int scale{2};
int e() {
  static int calls{0};
  ++calls;
  return calls;
}
void f(int &x, int y, int *z) {
  int first{e()};
  int second{e()};
  x += (y + *z) * (first + second);
}
int g(int a, int b) {
  int result{};
  result = a + b;
  f(result, a, &b);
  return result * scale;
}

void run() {
  int x{10};
  int y{20};
  int z{};
  z = g(x, y);
  std::cout << "z: " << z << '\n';  // 240
}
}  // namespace exercise_4

// =============================================================================
// The main Function
// =============================================================================

// [Slide 76] Two Forms of main
// A program has one main, so this file cannot show both forms. The main at the
// bottom of this file is the second form.

// [Slide 77] Command-line Arguments
namespace command_line {
// This program reads its first argument as a slide number, so the slide's
// program is on its own: arguments/command_line.cpp, target week5_arguments.
void run() {
  std::cout << "a program of its own: 702run week5_arguments 30 -45 60\n";
}
}  // namespace command_line

// =============================================================================
// Documenting Functions
// =============================================================================

// [Slide 81] Doxygen Comments
namespace doxygen_comment {
/**
 * @brief Convert an angle from degrees to radians.
 * @param deg The angle in degrees. Any finite value.
 * @return The same angle in radians.
 */
double convert_deg_to_rad(double deg);

double convert_deg_to_rad(double deg) { return deg * std::numbers::pi / 180.0; }

void run() {
  std::cout << "convert_deg_to_rad(180.0): " << convert_deg_to_rad(180.0) << '\n';  // 3.14159
}
}  // namespace doxygen_comment

// [Slide 82] The File Comment
// A @file comment belongs at the top of a header, so this file cannot show it
// in place. See ../../arm_demo/include/kinematics.hpp.

// [Slide 83] The VS Code Extension
namespace doxygen_skeleton {
// The skeleton the extension types. Replace "@return double" with what the
// value is, and write the @brief.
/**
 * @brief
 *
 * @param deg
 * @return double
 */
double convert_deg_to_rad(double deg);

double convert_deg_to_rad(double deg) { return deg * std::numbers::pi / 180.0; }

void run() {
  std::cout << "convert_deg_to_rad(90.0): " << convert_deg_to_rad(90.0) << '\n';  // 1.5708
}
}  // namespace doxygen_skeleton

int main(int argc, char* argv[]) {
  // One entry per slide that has code: its number, its title, and the function
  // that runs it. true at the end marks undefined behavior: a full run skips
  // that slide.
  const std::vector<Slide> slides{
      {7, "Without Functions", without_functions::run},
      {8, "Anatomy of a Function", anatomy::run},
      {9, "Anatomy of a Function", function_header::run},
      {10, "Anatomy of a Function", signature::run},
      {11, "Parameters and Arguments", parameters::run},
      {12, "Declaration and Definition", declaration_definition::run},
      {13, "Declaration Order", declaration_order::run},
      {14, "No Order Works", no_order_works::run},
      {15, "Declaration and Definition", every_function_declared::run},
      {16, "A Missing Definition", missing_definition::run},
      {29, "Calling and Returning", calling_returning::run},
      {30, "In a void Function", return_statement::run},
      {31, "Missing Returns", undefined::missing_return::run, true},
      {32, "Conversion on Return", conversion_on_return::run},
      {33, "Calling and Returning", nodiscard_attribute::run},
      {36, "Pass by Value", pass_by_value::run},
      {37, "The Cost of a Copy", cost_of_copy::run},
      {38, "Pass by Reference", pass_by_reference::run},
      {39, "A Swap Function", swap_function::run},
      {40, "Pass by const Reference", const_reference::run},
      {41, "String Parameters", string_parameters::run},
      {42, "Pass by const Reference", string_view_memory::run},
      {43, "Pass by const Reference", span_parameter::run},
      {44, "One Parameter, Three Sequences", three_sequences::run},
      {45, "A Span in Memory", span_memory::run},
      {46, "Pass by Pointer", pass_by_pointer::run},
      {48, "Exercise 2: Four Calls", exercise_2::run},
      {50, "Return by Value", return_by_value::run},
      {51, "A Large Result", large_result::run},
      {53, "Measured Elision", measured_elision::run},
      {54, "Return by Reference", return_by_reference::run},
      {55, "Returning a Local", undefined::returning_local::run, true},
      {56, "Returning a Parameter", undefined::returning_parameter::run, true},
      {57, "Return by Pointer", return_by_pointer::run},
      {58, "Returning the Address of a Local", undefined::returning_address::run, true},
      {60, "One Name, Three Functions", overloading::run},
      {61, "Valid Overloads", valid_overloads::run},
      {63, "Exercise 3: Overload Resolution", exercise_3::run},
      {65, "Filling from the Right", filling_from_right::run},
      {67, "Defaults in the Declaration", defaults_declaration::run},
      {68, "Two Functions (Overloads)", overloads_or_default::run},
      {70, "Lifetime and Scope", static_lifetime::run},
      {71, "One-time Initialization", one_time_init::run},
      {73, "Stack Frames", stack_frames::run},
      {74, "", exercise_4::run},
      {77, "Command-line Arguments", command_line::run},
      {81, "Doxygen Comments", doxygen_comment::run},
      {83, "The VS Code Extension", doxygen_skeleton::run},
  };
  return run_slides(slides, argc, argv, Part::lecture);
}
