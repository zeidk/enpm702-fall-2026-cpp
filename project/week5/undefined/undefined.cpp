/**
 * @file undefined.cpp
 * @brief L5: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer (set_source_files_properties in CMakeLists.txt),
 * so each run prints a report that names the bug and the line. The comment
 * above each namespace has the measured report.
 *
 * @warning Every slide here is undefined behavior on purpose. Without the
 *          sanitizers it may crash, print garbage, or appear to work.
 */
#include "undefined.hpp"

#include <iostream>
#include <string>

namespace undefined {

// [Slide 31] Missing Returns
// get_sign(0) falls off the end of a function that returns int. g++ warns
// while it compiles, and UndefinedBehaviorSanitizer stops the run at that call.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week5_playground 31:
//   undefined.cpp:39:1: warning: control reaches end of non-void function [-Wreturn-type]
//   get_sign(5): 1
//   get_sign(-5): -1
//   get_sign(0): undefined.cpp:33:5: runtime error: execution reached the end
//     of a value-returning function without returning a value
// The exit status is 1, and get_sign(0) prints no value.
namespace missing_return {
int get_sign(int number) {
  if (number > 0) {
    return 1;
  } else if (number < 0) {
    return -1;
  }
}  // number == 0 falls off the end

void run() {
  std::cout << "get_sign(5): " << get_sign(5) << '\n';    // 1
  std::cout << "get_sign(-5): " << get_sign(-5) << '\n';  // -1
  std::cout << "get_sign(0): " << get_sign(0) << '\n';    // undefined behavior
}
}  // namespace missing_return

// [Slide 55] Returning a Local
// tool_x returns a reference to local_x, which is destroyed when tool_x
// returns. g++ warns while it compiles, and puts a null pointer in place of the
// dead address, so the read crashes.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week5_playground 55:
//   undefined.cpp:65:10: warning: reference to local variable 'local_x' returned [-Wreturn-local-addr]
//   undefined.cpp:65:10: runtime error: reference binding to null pointer of type 'double'
//   r: undefined.cpp:70:25: runtime error: load of null pointer of type 'double'
//   ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
//   The signal is caused by a READ memory access.
//   Hint: address points to the zero page.
//     #0 in undefined::returning_local::run() undefined.cpp:70
// The exit status is 1.
namespace returning_local {
double& tool_x() {
  double local_x{0.42};
  return local_x;
}  // local_x is destroyed here

void run() {
  double& r{tool_x()};
  std::cout << "r: " << r << '\n';  // undefined behavior
}
}  // namespace returning_local

// [Slide 56] Returning a Parameter
// "camera" is not a std::string, so the call builds a temporary one, and the
// temporary dies at the end of the line that declares r. r is left naming it.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week5_playground 56:
//   undefined.cpp:96:22: warning: possibly dangling reference to a temporary [-Wdangling-reference]
//   r: ERROR: AddressSanitizer: stack-use-after-scope on address 0x...
//   READ of size 6 at 0x... thread T0
//     #0 in fwrite
//     #1 in std::__ostream_insert<char, std::char_traits<char> >(...)
//     #2 in undefined::returning_parameter::run() undefined.cpp:97
//   Address 0x... is located in stack of thread T0 at offset 144 in frame
//     #0 in undefined::returning_parameter::run() undefined.cpp:95
// READ of size 6 is "camera", read through r after the temporary is gone.
// The exit status is 1.
namespace returning_parameter {
const std::string& pick_longer(const std::string& a,
                               const std::string& b) {
  return a.size() >= b.size() ? a : b;
}

void run() {
  const std::string& r{pick_longer("lidar", "camera")};
  std::cout << "r: " << r << '\n';  // undefined behavior
}
}  // namespace returning_parameter

// [Slide 58] Returning the Address of a Local
// tool_x returns the address of local_x, which is destroyed when tool_x
// returns. g++ warns while it compiles, and returns a null pointer in place of
// the dead address, so the read crashes with SIGSEGV.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week5_playground 58:
//   undefined.cpp:117:10: warning: address of local variable 'local_x' returned [-Wreturn-local-addr]
//   *p: undefined.cpp:122:27: runtime error: load of null pointer of type 'double'
//   ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
//   The signal is caused by a READ memory access.
//   Hint: address points to the zero page.
//     #0 in undefined::returning_address::run() undefined.cpp:122
// The exit status is 1.
namespace returning_address {
double* tool_x() {
  double local_x{0.42};
  return &local_x;
}  // local_x is destroyed here

void run() {
  double* p{tool_x()};
  std::cout << "*p: " << *p << '\n';  // undefined behavior
}
}  // namespace returning_address

// [Appendix xix] Stack Overflow
// The frame's two limits, in this order because the second one stops the run.
// First, 21! does not fit in a long long, and signed overflow is undefined
// behavior (Lecture 2): UndefinedBehaviorSanitizer reports it, then the run
// goes on with a wrong, negative value. Second, dig() has no base case, so
// every call pushes one more frame until the stack runs out. g++ warns about
// dig() while it compiles.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week5_appendix xix:
//   undefined.cpp:156:6: warning: infinite recursion detected [-Winfinite-recursion]
//   compute_factorial(20): 2432902008176640000
//   compute_factorial(21): undefined.cpp:152:37: runtime error: signed integer
//     overflow: 21 * 2432902008176640000 cannot be represented in type 'long long int'
//   -4249290049419214848
//   ERROR: AddressSanitizer: stack-overflow on address 0x7ff...
//     #0 in undefined::stack_overflow::dig() undefined.cpp:158
//     #1 in undefined::stack_overflow::dig() undefined.cpp:158
//     ... and so on, one line per frame
//   SUMMARY: AddressSanitizer: stack-overflow undefined.cpp:158 in undefined::stack_overflow::dig()
// #0 is where the last call ran out of stack: line 158 in two of three runs,
// 156 in the third. The exit status is 1, and depth is never printed.
namespace stack_overflow {
long long compute_factorial(int n) {
  if (n <= 1) {        // base case
    return 1;
  }
  return n * compute_factorial(n - 1);
}

int depth{0};
void dig() {
  ++depth;
  dig();  // no base case
}

void run() {
  std::cout << "compute_factorial(20): " << compute_factorial(20) << '\n';  // 2432902008176640000
  std::cout << "compute_factorial(21): " << compute_factorial(21) << '\n';  // undefined behavior
  dig();
  std::cout << "depth: " << depth << '\n';  // never reached
}
}  // namespace stack_overflow

}  // namespace undefined
