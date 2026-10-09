/**
 * @file deduced.cpp
 * @brief L6 Section 3, Deduced Return Types: the code of every slide,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week6_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground       # every slide of every section, in order
 * 702run week6_playground 37    # only [Slide 37]
 * @endcode
 *
 * Each slide's code is in its own namespace, inside the namespace @c deduced.
 * The comment above a namespace names its slide, and slides() at the bottom
 * lists them. Code that does not compile is commented out where its slide
 * shows it: uncomment it, build, and you get the slide's error.
 */
#include <concepts>
#include <iostream>

#include "sections.hpp"

namespace deduced {

// [Slide 37] One Type for Every return
namespace one_type {
auto battery_fraction(double pct) {  // the return type is double
  return pct / 100.0;
}

// The deduced return type, checked while compiling.
static_assert(std::same_as<decltype(battery_fraction(64.0)), double>);

// Does not compile: two return statements, two types.
// error: inconsistent deduction for auto return type: 'int' and then 'double'
// auto battery_fraction_mixed(double pct) {
//   if (pct < 0.0) { return 0; }  // int
//   return pct / 100.0;           // double
// }

void run() {
  double fraction{battery_fraction(64.0)};
  std::cout << "battery_fraction(64.0): " << fraction << '\n';  // 0.64
}
}  // namespace one_type

// [Slide 38] The Body before the Call
// The slide splits the function over stats.hpp, stats.cpp and main.cpp. One
// file shows the same error: a call that comes before the body.
namespace body_first {
auto battery_fraction(double pct);  // the declaration only, as in stats.hpp

void run() {
  // Does not compile: the body is further down, so the return type is not
  // known yet.
  // error: use of 'auto deduced::body_first::battery_fraction(double)' before
  // deduction of 'auto'
  // double fraction{battery_fraction(64.0)};
  std::cout << "does not compile: uncomment the line in body_first::run()\n";
}

auto battery_fraction(double pct) {  // the body, as in stats.cpp
  return pct / 100.0;
}
}  // namespace body_first

// The slides of this section: number, title, the function that runs it.
std::vector<Slide> slides() {
  return {
    {37, "One Type for Every return", one_type::run},
    {38, "The Body before the Call", body_first::run},
  };
}

}  // namespace deduced
