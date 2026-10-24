/**
 * @file constructors.cpp
 * @brief L8 Section 5, Constructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. Code that does not compile, or that has undefined
 * behavior, is commented out where its slide shows it, with what the compiler
 * says.
 *
 * From [Slide 40] on, the slides use the full class, CruiseControl in
 * ../../common/include/cruise_control.hpp and ../../common/src/cruise_control.cpp.
 */
#include <iostream>
#include <stdexcept>

#include "cruise_control.hpp"
#include "sections.hpp"

namespace constructors {

// [Slide 39] A Member with No Initializer
namespace no_initializer {
class CruiseControl {
 public:
  double get_target_speed() const { return target_speed_; }

 private:
  double speed_limit_{13.4};
  double target_speed_;  // no { }
};

void run() {
  // CruiseControl a;  // a.target_speed_ has an indeterminate value
  // std::cout << a.get_target_speed() << '\n';  // undefined behavior
  // With -O0 (the Debug build) g++ says nothing. With -O2:
  // warning: 'a.CruiseControl::target_speed_' is used uninitialized [-Wuninitialized]
  CruiseControl b{};  // the { } sets every member to zero first
  std::cout << b.get_target_speed() << '\n';  // 0
}
}  // namespace no_initializer

// [Slide 41] No Default Constructor Now
namespace no_default {
void run() {
  CruiseControl ego_cruise{13.4};  // OK: speed limit 13.4, target speed 0
  // CruiseControl test_cruise;    // does not compile:
  // error: no matching function for call to 'CruiseControl::CruiseControl()'
  std::cout << ego_cruise.get_speed_limit() << ' '
            << ego_cruise.get_target_speed() << '\n';  // 13.4 0
}
}  // namespace no_default

// [Slide 42] A Constructor That Throws
namespace ctor_throws {
void run() {
  try {
    CruiseControl bad_cruise{0.0};  // breaks the invariant
    std::cout << "not reached\n";
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
}
}  // namespace ctor_throws

// [Slide 44] A const Member
// class CruiseControl {
//  public:
//   CruiseControl(double speed_limit) {
//     speed_limit_ = speed_limit;  // assignment: does not compile
//   }
//  private:
//   const double speed_limit_;  // a road's limit never changes
//   double target_speed_{0.0};
// };
// error: uninitialized const member in 'const double' [-fpermissive]
// error: assignment of read-only member 'CruiseControl::speed_limit_'

// [Slide 45] Declaration Order
// class CruiseControl {
//  public:
//   CruiseControl(double speed_limit, double target_speed)
//       : target_speed_{target_speed}, speed_limit_{speed_limit} {}
//  private:
//   double speed_limit_;   // created first
//   double target_speed_;  // created second
// };
// warning: 'CruiseControl::target_speed_' will be initialized after [-Wreorder]
// warning:   'double CruiseControl::speed_limit_' [-Wreorder]
// warning:   when initialized here [-Wreorder]

// [Slide 48] A Conversion Nobody Asked For
namespace conversion {
void show(const CruiseControl& cruise) {
  std::cout << cruise.get_speed_limit() << ' '
            << cruise.get_target_speed() << '\n';
}

void run() {
  show(CruiseControl{13.4});  // names the class: prints 13.4 0
  // show(13.4);  // a double, not a CruiseControl. With explicit:
  // error: invalid initialization of reference of type 'const CruiseControl&'
  //        from expression of type 'double'
  // Without explicit (remove it from cruise_control.hpp), it compiles and
  // prints 13.4 0.
}
}  // namespace conversion

// [Slide 49] Delegating Constructor
namespace delegating {
void run() {
  CruiseControl ego_cruise{13.4, 12.0};
  std::cout << ego_cruise.get_target_speed() << '\n';  // 12
  try {
    CruiseControl bad_cruise{13.4, 20.0};
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
}
}  // namespace delegating

std::vector<Slide> slides() {
  return {
      {39, "A Member with No Initializer", no_initializer::run},
      {41, "No Default Constructor Now", no_default::run},
      {42, "A Constructor That Throws", ctor_throws::run},
      {48, "A Conversion Nobody Asked For", conversion::run},
      {49, "Delegating Constructor", delegating::run},
  };
}

}  // namespace constructors
