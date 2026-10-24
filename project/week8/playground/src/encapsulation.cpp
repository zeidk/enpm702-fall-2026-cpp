/**
 * @file encapsulation.cpp
 * @brief L8 Section 3, Encapsulation: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile or link is commented
 * out where its slide shows it: uncomment it, build, and you get the slide's
 * error.
 */
#include <iostream>

#include "sections.hpp"

namespace encapsulation {

// [Slide 25] The Source File
// The class of [Slide 21], and its methods defined outside it with
// CruiseControl::, as cruise_control.hpp and cruise_control.cpp would hold them.
namespace source_file {
class CruiseControl {
 public:  // any code may call these
  void set_target_speed(double speed);
  double get_target_speed() const;

 private:  // only CruiseControl's own methods may use these
  double speed_limit_{13.4};  // m/s
  double target_speed_{0.0};  // m/s
};

void CruiseControl::set_target_speed(double speed) {
  target_speed_ = speed;
}

double CruiseControl::get_target_speed() const {
  return target_speed_;
}

// [Slide 26] Without CruiseControl::
// void set_target_speed(double speed) {
//   target_speed_ = speed;
// }
// error: 'target_speed_' was not declared in this scope;
//        did you mean 'set_target_speed'?

void run() {
  CruiseControl ego_cruise;
  ego_cruise.set_target_speed(12.0);
  std::cout << ego_cruise.get_target_speed() << '\n';  // 12
}
}  // namespace source_file

// [Slide 27] Changing the Inside
// The data members hold km/h; the methods still take and give m/s. The calls
// are those of [Slide 10], unchanged.
namespace changing_inside {
class CruiseControl {
 public:
  void set_target_speed(double speed) {
    target_speed_kmh_ = speed * 3.6;
  }
  double get_target_speed() const {
    return target_speed_kmh_ / 3.6;
  }

 private:
  double speed_limit_kmh_{48.24};  // 13.4 * 3.6
  double target_speed_kmh_{0.0};
};

void run() {
  CruiseControl ego_cruise;
  CruiseControl test_cruise;
  ego_cruise.set_target_speed(12.0);
  test_cruise.set_target_speed(10.0);
  std::cout << ego_cruise.get_target_speed() << ' '
            << test_cruise.get_target_speed() << '\n';  // 12 10
}
}  // namespace changing_inside

std::vector<Slide> slides() {
  return {
      {25, "The Source File", source_file::run},
      {27, "Changing the Inside", changing_inside::run},
  };
}

}  // namespace encapsulation
