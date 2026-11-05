/**
 * @file undefined.cpp
 * @brief L9: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with -fsanitize=address,undefined (see
 * ../../CMakeLists.txt). The reports on the slides come from these runs.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "automation_level.hpp"
#include "l8_vehicle.hpp"

namespace undefined {

using fleet::AutomationLevel;

// [Slide 64] A Base Class Smart Pointer to a Derived Object. RoboTaxi and Taxi
// on L8's Vehicle (l8::Vehicle), as in inheritance.cpp:
// ~Vehicle is not virtual.
class RoboTaxi : public l8::Vehicle {
 public:
  explicit RoboTaxi(double max_speed, std::string operating_zone = "College Park",
                    AutomationLevel automation_level = AutomationLevel::level_4)
      : Vehicle{max_speed},
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {}
  void change_zone(std::string operating_zone) { operating_zone_ = std::move(operating_zone); }

 private:
  std::string operating_zone_;        // the area it may drive in
  AutomationLevel automation_level_;  // SAE J3016 level of driving automation
};

class Taxi : public l8::Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;  // empty: no driver
};

// shared, made by std::make_shared<Taxi>, remembers the Taxi: ~Taxi runs, then
// ~Vehicle. owner deletes the RoboTaxi through a Vehicle*: undefined behavior
// ([expr.delete] 7.6.2.8 paragraph 3), and AddressSanitizer stops the program
// with new-delete-type-mismatch when owner goes out of scope.
namespace smart_pointer {
void run() {
  std::unique_ptr<l8::Vehicle> owner{std::make_unique<RoboTaxi>(13.4)};
  owner->set_speed(10.0);  // a Vehicle method
  // owner->change_zone("Silver Spring");  // does not compile
  std::shared_ptr<l8::Vehicle> shared{std::make_shared<Taxi>(13.4, "Ana")};
  shared->set_speed(5.0);
  std::cout << owner->get_speed() << ' ' << shared->get_speed() << '\n';  // 10 5
}  // owner deletes a RoboTaxi through a Vehicle*: AddressSanitizer stops here
// Uncommented, the line gives:
// error: 'class l8::Vehicle' has no member named 'change_zone'
}  // namespace smart_pointer

std::vector<Slide> slides() {
  return {
      {64, "A Base Class Smart Pointer to a Derived Object", smart_pointer::run, true},
  };
}

}  // namespace undefined
