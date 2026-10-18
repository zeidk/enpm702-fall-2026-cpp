/**
 * @file undefined.cpp
 * @brief L7: the code of every slide that leaks or has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with AddressSanitizer and
 * UndefinedBehaviorSanitizer (set_source_files_properties in CMakeLists.txt).
 *
 * @warning Every slide here is a bug on purpose. Without the sanitizers a
 *          leak is silent, and a double delete or a dangling read may crash,
 *          print garbage, or appear to work.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "fleet.hpp"

namespace undefined {

namespace {
RobotStatus* make_robot(int id, double battery_pct) {
  return new RobotStatus{id, battery_pct, {0.0, 0.0}, false};
}
}  // namespace

// [Slide 9] The Leak, Measured
namespace early_return_leak {
bool needs_charge(int id) {
  RobotStatus* robot{make_robot(id, 64.0)};
  if (robot->battery_pct > 20.0) {
    return false;  // leaves without delete: the robot leaks
  }
  delete robot;
  return true;
}

void run() {
  std::cout << std::boolalpha << "needs_charge(3): " << needs_charge(3) << std::noboolalpha
            << '\n';  // false, and LeakSanitizer reports at exit
}
}  // namespace early_return_leak

// [Appendix ii] Double Delete, Measured
namespace double_delete {
void run() {
  RobotStatus* robot{make_robot(3, 64.0)};
  std::vector<RobotStatus*> fleet{robot};
  std::vector<RobotStatus*> idle{robot};  // the same robot, in a second list
  for (RobotStatus* r : fleet) { delete r; }
  for (RobotStatus* r : idle) { delete r; }  // the second delete of one robot
}
}  // namespace double_delete

// [Appendix iii] Dangling Read, Measured
namespace dangling_read {
void run() {
  RobotStatus* robot{make_robot(3, 64.0)};
  std::vector<RobotStatus*> fleet{robot};
  std::vector<RobotStatus*> idle{robot};
  for (RobotStatus* r : fleet) { delete r; }
  std::cout << "idle[0]->battery_pct: " << idle[0]->battery_pct << '\n';  // freed memory
}
}  // namespace dangling_read

// [Slide 66] The Cycle, Measured
namespace shared_cycle {
struct Robot {
  RobotStatus status;
  std::shared_ptr<Robot> partner;  // the robot it works with
};

void run() {
  auto a{std::make_shared<Robot>(RobotStatus{1, 82.5, {0.0, 0.0}, false})};
  auto b{std::make_shared<Robot>(RobotStatus{2, 35.0, {4.0, 1.0}, false})};
  a->partner = b;  // a owns b
  b->partner = a;  // b owns a
  std::cout << "a.use_count(): " << a.use_count() << ", b.use_count(): " << b.use_count()
            << '\n';  // 2 2
}  // a and b go out of scope: each count drops to 1, never 0, so neither is deleted
}  // namespace shared_cycle

}  // namespace undefined
