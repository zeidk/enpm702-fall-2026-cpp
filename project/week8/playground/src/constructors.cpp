/**
 * @file constructors.cpp
 * @brief L8, Constructors: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week8_playground: main.cpp collects slides() from every
 * section file and runs them. The appendix frames of this section are listed
 * in appendix_slides(), which week8_appendix runs. Code that does not compile,
 * or that has undefined behavior, is commented out where its slide shows it,
 * with what the compiler says. Each commented-out class is in a namespace of
 * its own, so uncommenting one leaves the rest of the file as it was.
 *
 * [Slide 61] shows a first version of the constructor, written in the class
 * (declared and defined in one place), whose body assigns each member; the
 * Vehicle of param_ctor below is that version. [Slide 63] shows it split into
 * a header and a source file; the course defines every constructor in the
 * class. From [Slide 64] on, the slides use the full class, Vehicle in
 * ../../common/include/vehicle.hpp, whose constructors, defined in the class,
 * use the member initializer list of [Slide 70].
 *
 * The Live Coding slide (Step 5) has no code here: we write that code together
 * in ../../live/src/main.cpp (week8_live). The discussion Spot the Bug is in
 * discussions.cpp.
 */
#include <iostream>
#include <stdexcept>
#include <string>

#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"
#include "sensor.hpp"
#include "vehicle.hpp"

namespace constructors {

// [Slide 58] Default Constructor: the first version of Vehicle declares no
// constructor, so the compiler writes the default constructor. It sets each
// data member from its default member initializer.
namespace compiler_default {
class Vehicle {  // the first version: no constructor declared
 public:
  double get_max_speed() const { return max_speed_; }
  const std::string& get_color() const { return color_; }

 private:
  double max_speed_{13.4};  // default member initializer
  double speed_{0.0};       // default member initializer
  std::string color_{"white"};  // default member initializer
  double mileage_{0.0};     // default member initializer
};

void run() {
  Vehicle car_a;  // the compiler's default constructor runs
  std::cout << car_a.get_max_speed() << ' ' << car_a.get_color() << '\n';  // 13.4 white
}
}  // namespace compiler_default

// [Slide 59] A User-Defined Default Constructor
namespace user_default_ctor {
class Vehicle {
 public:
  Vehicle() {  // a user-defined default constructor
    std::cout << "a new Vehicle: " << color_ << ", "
              << max_speed_ << " m/s\n";
  }

 private:
  double max_speed_{13.4};
  double speed_{0.0};
  std::string color_{"white"};
  double mileage_{0.0};
};

void run() {
  Vehicle car_a;  // prints a new Vehicle: white, 13.4 m/s
}
}  // namespace user_default_ctor

// [Slide 60] A Member with No Initializer
namespace no_initializer {
class Vehicle {
 public:
  double get_speed() const { return speed_; }

 private:
  double max_speed_{13.4};
  double speed_;  // no { }
  std::string color_{"white"};
  double mileage_{0.0};
};

void run() {
  // Vehicle a;  // a.speed_ has an indeterminate value
  // std::cout << a.get_speed() << '\n';  // undefined behavior
  // g++ 13.3 gives no warning with -O0, the course's Debug build. With -O1 or
  // -O2 it warns here, in a function other than main():
  // warning: 'a.constructors::no_initializer::Vehicle::speed_' is used uninitialized [-Wuninitialized]
  // (the slide's code has no namespaces, so the slide's warning names
  // 'a.Vehicle::speed_' only)
  Vehicle b{};  // the { } sets every member to zero first
  std::cout << b.get_speed() << '\n';  // 0
}
}  // namespace no_initializer

// [Slide 61] Constructor with Parameters: a first version, written in the
// class (declared and defined in one place), where the body assigns each
// member. ../../common/include/vehicle.hpp uses the member initializer list of
// [Slide 70] instead.
namespace param_ctor {
class Vehicle {
 public:
  Vehicle(double max_speed, const std::string& color = "white") {
    max_speed_ = max_speed;  // assignment
    color_ = color;
    if (max_speed_ <= 0.0) {
      throw std::invalid_argument{
          "maximum speed must be greater than 0"};
    }
  }
  double get_max_speed() const { return max_speed_; }
  double get_speed() const { return speed_; }
  const std::string& get_color() const { return color_; }
  double get_mileage() const { return mileage_; }

 private:
  double max_speed_;     // set by the constructor
  double speed_{0.0};    // default member initializer
  std::string color_;    // set by the constructor
  double mileage_{0.0};  // default member initializer
};

void run() {
  Vehicle car_a{13.4};  // white: the default argument
  std::cout << car_a.get_max_speed() << ' ' << car_a.get_color() << '\n';  // 13.4 white
}
}  // namespace param_ctor

// [Slide 62] Two Kinds of Default Values: the Vehicle of [Slide 61]. The
// default argument gives color its value when the caller leaves it out; the
// default member initializers give speed_ and mileage_ theirs.
namespace default_values {
void run() {
  param_ctor::Vehicle car_a{13.4};          // white: the default argument
  param_ctor::Vehicle car_b{13.4, "blue"};  // blue
  std::cout << car_a.get_color() << ' ' << car_b.get_color() << '\n';  // white blue
  std::cout << car_a.get_speed() << ' ' << car_a.get_mileage() << '\n';  // 0 0
}
}  // namespace default_values

// [Slide 63] The Default Argument in the Header: the constructor of
// [Slide 61], split into a declaration (vehicle.hpp) and a definition
// (vehicle.cpp). The default argument is written once, in the declaration.
// Written again in the definition, it does not compile:
// namespace default_twice {
// class Vehicle {  // vehicle.hpp
//  public:
//   Vehicle(double max_speed, const std::string& color = "white");
//  private:
//   double max_speed_;
//   double speed_{0.0};
//   std::string color_;
//   double mileage_{0.0};
// };
// // vehicle.cpp
// Vehicle::Vehicle(double max_speed, const std::string& color = "white") {
//   max_speed_ = max_speed;
//   color_ = color;
// }
// }  // namespace default_twice
// error: default argument given for parameter 2 of 'constructors::default_twice::Vehicle::Vehicle(double, const std::string&)'
// note: previous specification in 'constructors::default_twice::Vehicle::Vehicle(double, const std::string&)' here
// (the slide's code has no namespaces, so the slide's messages name Vehicle only)

// [Slide 64] Discussion: Which Lines Compile? The answers: car_a and car_b
// compile, parked_car does not. The class declares a constructor, so the
// compiler writes no default constructor.
namespace no_default {
void run() {
  Vehicle car_a{13.4};          // compiles: maximum speed 13.4, white
  Vehicle car_b{13.4, "blue"};  // compiles: blue
  // Vehicle parked_car;        // does not compile:
  // error: no matching function for call to 'Vehicle::Vehicle()'
  std::cout << car_a.get_max_speed() << ' '
            << car_a.get_speed() << ' '
            << car_a.get_color() << ' '
            << car_b.get_color() << '\n';  // 13.4 0 white blue
}
}  // namespace no_default

// [Slide 65] Braces or Parentheses
namespace braces_parens {
void run() {
  Vehicle car_b{13.4, "blue"};  // braces: it runs the constructor
  Vehicle car_c(13.4, "blue");  // parentheses: the same constructor
  std::cout << car_b.get_color() << ' ' << car_c.get_color() << '\n';  // blue blue

  // Vehicle parked_car{};  // asks for the default constructor; Vehicle has none:
  // error: no matching function for call to 'Vehicle::Vehicle(<brace-enclosed initializer list>)'
  // Vehicle parked_car();  // declares a function that returns a Vehicle:
  // warning: empty parentheses were disambiguated as a function declaration [-Wvexing-parse]
  // Using it as an object then fails, for example parked_car.get_color():
  // error: request for member 'get_color' in 'parked_car', which is of non-class type 'Vehicle()'
}
}  // namespace braces_parens

// [Slide 66] Narrowing and Member Defaults
namespace narrowing {
class MemberDefault {  // a member's default value, in a class
 public:
  [[nodiscard]] double get_speed() const noexcept { return speed_; }

 private:
  double speed_{0.0};  // sets the default
  // double speed_(0.0);  // does not compile:
  // error: expected identifier before numeric constant
  // error: expected ',' or '...' before numeric constant
};

void run() {
  // Sensor lidar_a{1.5};  // does not compile: the id is an int
  // error: narrowing conversion of '1.5e+0' from 'double' to 'int' [-Wnarrowing]
  Sensor lidar_b(1.5);  // compiles with no warning: the .5 is dropped
  std::cout << "lidar_b: id " << lidar_b.get_id() << '\n';  // lidar_b: id 1

  const MemberDefault member_default;
  std::cout << "speed_: " << member_default.get_speed() << '\n';  // speed_: 0
}
}  // namespace narrowing

// [Slide 67] Assignment in the Body: the Code, and [Slide 68] Assignment
// Builds a Member Twice, its output
namespace builds_twice {
class PaperTray {
 public:
  PaperTray() { std::cout << "PaperTray(): letter\n"; }
  PaperTray(const std::string& size) {
    std::cout << "PaperTray(size): " << size << '\n';
  }
};

class Printer {
 public:
  Printer(const std::string& paper_size) {
    std::cout << "Printer constructor body\n";
    tray_ = PaperTray{paper_size};
  }
 private:
  PaperTray tray_;
};

void run() {
  // main.cpp
  Printer office_printer{"A4"};
  // PaperTray(): letter
  // Printer constructor body
  // PaperTray(size): A4
}
}  // namespace builds_twice

// [Slide 69] Assignment Fails for a const Member
// namespace const_member {
// class Vehicle {
//  public:
//   Vehicle(double max_speed, const std::string& color = "white") {
//     max_speed_ = max_speed;  // assignment: does not compile
//     color_ = color;
//   }
//  private:
//   const double max_speed_;  // fixed when the object is made
//   double speed_{0.0};
//   std::string color_;  // from the constructor
//   double mileage_{0.0};
// };
// }  // namespace const_member
// error: uninitialized const member in 'const double' [-fpermissive]
// error: assignment of read-only member 'constructors::const_member::Vehicle::max_speed_'
// (the slide's code has no namespaces, so the slide's error names Vehicle only)

// [Slide 70] Member Initializer List: the constructor of [Slide 61], defined
// in the class with the list. The run uses the full class,
// ../../common/include/vehicle.hpp, whose list also sets id_ from the static
// counter of the appendix's static Members.
namespace init_list {
void run() {
  Vehicle car_a{13.4};
  std::cout << car_a.get_max_speed() << ' ' << car_a.get_color() << '\n';  // 13.4 white
}
}  // namespace init_list

// [Slide 71] The List Fixes Both Problems: the Printer of [Slide 67] and the
// Vehicle of [Slide 69], with the member initializer list.
namespace list_fixes {
class Vehicle {
 public:
  Vehicle(double max_speed, const std::string& color = "white")
      : max_speed_{max_speed}, color_{color} {}
 private:
  const double max_speed_;  // fixed when the object is made
  double speed_{0.0};
  std::string color_;  // from the constructor
  double mileage_{0.0};
};

class Printer {
 public:
  Printer(const std::string& paper_size) : tray_{paper_size} {
    std::cout << "Printer constructor body\n";
  }
 private:
  builds_twice::PaperTray tray_;  // the PaperTray of [Slide 67]
};

void run() {
  Vehicle car_a{13.4};  // compiles: the list gives max_speed_ its value
  Printer office_printer{"A4"};
  // PaperTray(size): A4
  // Printer constructor body
}
}  // namespace list_fixes

// [Slide 72] The Range Check: Sensor's constructor, of ../../common/include/sensor.hpp,
// sets every member in the list, then checks the range.
namespace range_check {
void run() {
  try {
    Sensor radar{3, SensorType::radar, -5.0};
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }  // caught: range must be at least 0
}
}  // namespace range_check

// [Slide 73] Declaration Order
// namespace declaration_order {
// class Vehicle {
//  public:
//   Vehicle(double max_speed, double speed)
//       : speed_{speed}, max_speed_{max_speed}, color_{"white"} {}
//  private:
//   double max_speed_;     // created first
//   double speed_;         // created second
//   std::string color_;    // created third
//   double mileage_{0.0};  // created fourth
// };
// }  // namespace declaration_order
// warning: 'constructors::declaration_order::Vehicle::speed_' will be initialized after [-Wreorder]
// warning:   'double constructors::declaration_order::Vehicle::max_speed_' [-Wreorder]
// warning:   when initialized here [-Wreorder]
// (the slide's code has no namespaces, so the slide's warnings name Vehicle only)

// [Slide 75] explicit and [Slide 76] A Conversion Nobody Asked For: the
// constructor of Vehicle in ../../common/include/vehicle.hpp is explicit.
namespace conversion {
void show(const Vehicle& vehicle) {
  std::cout << vehicle.get_max_speed() << ' '
            << vehicle.get_speed() << '\n';
}

void run() {
  show(Vehicle{13.4});  // names the class: prints 13.4 0
  // show(13.4);  // a double, not a Vehicle. With explicit:
  // error: invalid initialization of reference of type 'const Vehicle&'
  //        from expression of type 'double'
  // Without explicit (remove it from vehicle.hpp), it compiles and
  // prints 13.4 0.
}
}  // namespace conversion

// [Slide 77] Delegating Constructor
namespace delegating {
void run() {
  Vehicle car_a{13.4, 12.0};
  std::cout << car_a.get_speed() << '\n';  // 12
  try {
    Vehicle bad_car{13.4, 20.0};
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
}
}  // namespace delegating

void run_commented_out() { print_commented_out("playground/src/constructors.cpp"); }

std::vector<Slide> slides() {
  return {
      {58, "Default Constructor", compiler_default::run},
      {59, "A User-Defined Default Constructor", user_default_ctor::run},
      {60, "A Member with No Initializer", no_initializer::run},
      {61, "Constructor with Parameters", param_ctor::run},
      {62, "Two Kinds of Default Values", default_values::run},
      {63, "The Default Argument in the Header", run_commented_out},
      {64, "Discussion: Which Lines Compile?", no_default::run},
      {65, "Braces or Parentheses", braces_parens::run},
      {66, "Narrowing and Member Defaults", narrowing::run},
      {67, "Assignment in the Body: the Code", builds_twice::run},
      {68, "Assignment Builds a Member Twice", builds_twice::run},
      {69, "Assignment Fails for a const Member", run_commented_out},
      {70, "Member Initializer List", init_list::run},
      {71, "The List Fixes Both Problems", list_fixes::run},
      {72, "The Range Check", range_check::run},
      {73, "Declaration Order", run_commented_out},
      {75, "explicit", conversion::run},
      {76, "A Conversion Nobody Asked For", conversion::run},
      {77, "Delegating Constructor", delegating::run},
  };
}

std::vector<Slide> appendix_slides() {
  return {
  };
}

}  // namespace constructors
