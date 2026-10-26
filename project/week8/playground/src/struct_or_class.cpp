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

// [Slide 18] Default Access
namespace default_access {
struct OpenSpeed { double speed{0.0}; };
class HiddenSpeed { double speed{0.0}; };

void run() {
  OpenSpeed open;
  open.speed = 12.0;    // OK: a struct's members are public
  HiddenSpeed hidden;
  // hidden.speed = 12.0;  // does not compile:
  // error: 'double struct_or_class::default_access::HiddenSpeed::speed'
  //        is private within this context
  // (the slide's code has no namespaces, so the slide's error names HiddenSpeed only)
  (void)hidden;
  std::cout << open.speed << '\n';  // 12
}
}  // namespace default_access

// [Slide 19] A Rule Nothing Checks
namespace nothing_checks {
struct VehicleData {
  double max_speed{13.4};  // m/s
  double speed{0.0};       // m/s, must stay from 0 to max_speed
};

void run() {
  VehicleData data;
  data.speed = 20.0;  // compiles: any code can break the rule
  std::cout << data.speed << " > "
            << data.max_speed << '\n';  // 20 > 13.4
}
}  // namespace nothing_checks

std::vector<Slide> slides() {
  return {
      {18, "Default Access", default_access::run},
      {19, "A Rule Nothing Checks", nothing_checks::run},
  };
}

}  // namespace struct_or_class
