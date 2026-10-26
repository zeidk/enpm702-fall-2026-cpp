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
 * From [Slide 42] on, the slides use the full class, Vehicle in
 * ../../common/include/vehicle.hpp and ../../common/src/vehicle.cpp.
 */
#include <iostream>
#include <stdexcept>

#include "position.hpp"
#include "route.hpp"
#include "sections.hpp"
#include "vehicle.hpp"

namespace constructors {

// [Slide 41] A Member with No Initializer
namespace no_initializer {
class Vehicle {
 public:
  double get_speed() const { return speed_; }

 private:
  double max_speed_{13.4};
  double speed_;  // no { }
};

void run() {
  // Vehicle a;  // a.speed_ has an indeterminate value
  // std::cout << a.get_speed() << '\n';  // undefined behavior
  // With -O0 (the Debug build) g++ says nothing. With -O2:
  // warning: 'a.Vehicle::speed_' is used uninitialized [-Wuninitialized]
  Vehicle b{};  // the { } sets every member to zero first
  std::cout << b.get_speed() << '\n';  // 0
}
}  // namespace no_initializer

// [Slide 43] No Default Constructor Now
namespace no_default {
void run() {
  Vehicle downtown_car{13.4};  // OK: maximum speed 13.4, speed 0
  // Vehicle airport_car;      // does not compile:
  // error: no matching function for call to 'Vehicle::Vehicle()'
  std::cout << downtown_car.get_max_speed() << ' '
            << downtown_car.get_speed() << '\n';  // 13.4 0
}
}  // namespace no_default

// [Slide 44] A Constructor That Throws
namespace ctor_throws {
void run() {
  try {
    Vehicle bad_car{0.0};  // breaks the invariant
    std::cout << "not reached\n";
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
}
}  // namespace ctor_throws

// [Slide 46] A const Member
// class Vehicle {
//  public:
//   Vehicle(double max_speed) {
//     max_speed_ = max_speed;  // assignment: does not compile
//   }
//  private:
//   const double max_speed_;  // a vehicle's maximum never changes
//   double speed_{0.0};
// };
// error: uninitialized const member in 'const double' [-fpermissive]
// error: assignment of read-only member 'Vehicle::max_speed_'

// [Slide 47] Declaration Order
// class Vehicle {
//  public:
//   Vehicle(double max_speed, double speed)
//       : speed_{speed}, max_speed_{max_speed} {}
//  private:
//   double max_speed_;  // created first
//   double speed_;      // created second
// };
// warning: 'Vehicle::speed_' will be initialized after [-Wreorder]
// warning:   'double Vehicle::max_speed_' [-Wreorder]
// warning:   when initialized here [-Wreorder]

// [Slide 50] A Conversion Nobody Asked For
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

// [Slide 51] Delegating Constructor
namespace delegating {
void run() {
  Vehicle downtown_car{13.4, 12.0};
  std::cout << downtown_car.get_speed() << '\n';  // 12
  try {
    Vehicle bad_car{13.4, 20.0};
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << '\n';
  }
}
}  // namespace delegating

// [Slide 52] The Class So Far. The full class in vehicle.hpp also has drive(),
// which takes a Route and prints the trip.
namespace class_so_far {
void run() {
  Vehicle downtown_car{13.4, 12.0};
  Route route;
  route.add(Position{0.0, 0.0});
  route.add(Position{300.0, 0.0});
  route.add(Position{300.0, 400.0});
  downtown_car.drive(route);  // 700 m at 12 m/s: 58.3333 s
}
}  // namespace class_so_far

std::vector<Slide> slides() {
  return {
      {41, "A Member with No Initializer", no_initializer::run},
      {43, "No Default Constructor Now", no_default::run},
      {44, "A Constructor That Throws", ctor_throws::run},
      {50, "A Conversion Nobody Asked For", conversion::run},
      {51, "Delegating Constructor", delegating::run},
      {52, "The Class So Far", class_so_far::run},
  };
}

}  // namespace constructors
