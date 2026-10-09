/**
 * @file undefined.cpp
 * @brief L6: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with AddressSanitizer (set_source_files_properties
 * in CMakeLists.txt). The comment above each namespace has the measured result.
 *
 * @warning Every slide here is undefined behavior on purpose. Without the
 *          sanitizer it may crash, print garbage, or appear to work.
 */
#include "undefined.hpp"

#include <iostream>
#include <optional>

namespace undefined {

// [Slide 35] Three Ways to Read
// * does not check whether the optional holds a value. Whatever it prints
// means nothing, and may change with the next build. AddressSanitizer does
// not catch it: the bytes it reads belong to idle.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week6_playground 35:
//   *idle: 0
namespace optional_star {
void run() {
  std::optional<int> idle;
  std::cout << "*idle: " << *idle << '\n';  // undefined behavior
}
}  // namespace optional_star

// [Slide 61] A Dangling Capture
// limit_pct is a parameter: it dies when make_filter returns, and the lambda
// keeps a reference to it. AddressSanitizer stops the run with a report that
// names the bug and the line.
//
// Measured on 2026-10-06, g++ 13.3, -std=c++20 -g, 702run week6_playground 61:
//   is_low(30.0): ERROR: AddressSanitizer: stack-use-after-return on address 0x...
//   READ of size 8 at 0x... thread T0
//     #0 in operator() undefined.cpp:49
//     #1 in undefined::dangling_capture::run() undefined.cpp:54
//   Address 0x... is located in stack of thread T0 at offset 32 in frame
//     #0 in undefined::dangling_capture::make_filter(double) undefined.cpp:48
//     [32, 40) 'limit_pct' (line 48) <== Memory access at offset 32 is inside this variable
// The exit status is 1. Built without the sanitizer, it printed 0 in 3 of 3 runs.
namespace dangling_capture {
auto make_filter(double limit_pct) {
  return [&limit_pct](double pct) { return pct < limit_pct; };
}

void run() {
  auto is_low = make_filter(40.0);
  std::cout << "is_low(30.0): " << is_low(30.0) << '\n';  // should be 1
}
}  // namespace dangling_capture

}  // namespace undefined
