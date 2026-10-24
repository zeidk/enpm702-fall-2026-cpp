/**
 * @file struct_or_class.cpp
 * @brief L8 Section 2, Struct or Class: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile is commented out
 * where its slide shows it: uncomment it, build, and you get the slide's error.
 */
#include <iostream>

#include "sections.hpp"

namespace struct_or_class {

// [Slide 16] Default Access
namespace default_access {
struct OpenSpeed { double target_speed{0.0}; };
class HiddenSpeed { double target_speed{0.0}; };

void run() {
  OpenSpeed open;
  open.target_speed = 12.0;    // OK: a struct's members are public
  HiddenSpeed hidden;
  // hidden.target_speed = 12.0;  // does not compile:
  // error: 'double struct_or_class::default_access::HiddenSpeed::target_speed'
  //        is private within this context
  // (the slide's code has no namespaces, so the slide's error names HiddenSpeed only)
  (void)hidden;
  std::cout << open.target_speed << '\n';  // 12
}
}  // namespace default_access

// [Slide 17] A Rule Nothing Checks
namespace nothing_checks {
struct CruiseData {
  double speed_limit{13.4};  // m/s
  double target_speed{0.0};  // m/s, must stay from 0 to speed_limit
};

void run() {
  CruiseData cruise;
  cruise.target_speed = 20.0;  // compiles: any code can break the rule
  std::cout << cruise.target_speed << " > "
            << cruise.speed_limit << '\n';  // 20 > 13.4
}
}  // namespace nothing_checks

std::vector<Slide> slides() {
  return {
      {16, "Default Access", default_access::run},
      {17, "A Rule Nothing Checks", nothing_checks::run},
  };
}

}  // namespace struct_or_class
