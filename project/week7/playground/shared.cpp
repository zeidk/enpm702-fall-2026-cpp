/**
 * @file shared.cpp
 * @brief L7 Section 3, Smart Pointers, std::shared_ptr: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Part of week7_playground: main.cpp collects slides() from every
 * section file and runs them.
 *
 * @code
 * 702run week7_playground 56    # only [Slide 56]
 * @endcode
 *
 * Robot 3 is in the fleet and waits in the charging queue at the same time,
 * so both lists own it through std::shared_ptr. Code that does not compile is
 * commented out where its slide shows it: uncomment it, build, and you get
 * the slide's error.
 */
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "fleet.hpp"
#include "sections.hpp"

namespace shared {

using Fleet = std::vector<std::shared_ptr<RobotStatus>>;

// [Slide 53] A Robot in Two Lists
// The slide states the problem; its code is in the comments of the slide.

// [Slide 54] Creating One
namespace creating {
void run() {
  Fleet fleet;
  Fleet charging_queue;
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  fleet.push_back(robot);           // a copy: one more owner
  charging_queue.push_back(robot);  // a copy: one more owner
  std::cout << "robot.use_count(): " << robot.use_count() << '\n';  // 3
  std::cout << std::boolalpha << "fleet[0] == charging_queue[0]: "
            << (fleet[0] == charging_queue[0]) << std::noboolalpha << '\n';  // true: one robot
}
}  // namespace creating

// [Slide 55] The Control Block
namespace control_block {
void run() {
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  std::shared_ptr<RobotStatus> in_queue{robot};
  std::cout << "sizeof(robot): " << sizeof(robot) << ", sizeof(RobotStatus): "
            << sizeof(RobotStatus) << '\n';                              // 16 40
  std::cout << "robot.use_count(): " << robot.use_count() << '\n';     // 2
}
}  // namespace control_block

// [Slide 56] use_count, Step by Step
namespace use_count_steps {
void run() {
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  std::cout << "created: " << robot.use_count() << '\n';                // 1
  std::shared_ptr<RobotStatus> in_queue{robot};
  std::cout << "copied: " << robot.use_count() << '\n';                 // 2
  std::shared_ptr<RobotStatus> moved{std::move(in_queue)};
  std::cout << std::boolalpha << "moved: " << robot.use_count()
            << ", in_queue == nullptr: " << (in_queue == nullptr) << std::noboolalpha
            << '\n';                                                     // 2 true
  moved.reset();
  std::cout << "reset: " << robot.use_count() << '\n';                  // 1
}
}  // namespace use_count_steps

// [Slide 57] Three Ways to Let Go
// Three separate runs, each from step 2; start() makes steps 1 and 2 and prints
// their counts once, in the first run.
namespace let_go {
std::shared_ptr<RobotStatus> start(bool show) {
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  {
    std::shared_ptr<RobotStatus> in_queue{robot};
    if (show) {
      std::cout << "1: " << robot.use_count() << '\n';  // 2
    }
  }  // in_queue's scope ends
  if (show) {
    std::cout << "2: " << robot.use_count() << '\n';  // 1
  }
  return robot;
}

void run() {
  {
    auto robot{start(true)};
    robot = std::make_shared<RobotStatus>(4);  // robot 3 deleted; robot 4's other members take defaults
    std::cout << "3a: robot->id: " << robot->id << ", use_count: " << robot.use_count()
              << '\n';                                                           // 4 1
  }
  {
    auto robot{start(false)};
    robot.reset();  // robot 3 deleted
    std::cout << std::boolalpha << "3b: robot == nullptr: " << (robot == nullptr)
              << std::noboolalpha << '\n';  // true
  }
  {
    auto robot{start(false)};
  }  // 3c: robot's scope ends, robot 3 deleted
}
}  // namespace let_go

// [Slide 58] Three Ways to Let Go, in Memory
// The slide shows the figure of slide 57's code.

// [Slide 59] Deletion by the Last Owner
namespace last_owner {
void run() {
  Fleet fleet;
  Fleet charging_queue;
  fleet.push_back(std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false));
  charging_queue.push_back(fleet[0]);
  std::cout << "both lists: " << charging_queue[0].use_count() << '\n';  // 2
  fleet.clear();  // robot 3 leaves the fleet while it still waits in the queue
  std::cout << "after fleet.clear(): " << charging_queue[0].use_count()
            << ", robot " << charging_queue[0]->id << " still there\n";  // 1, robot 3
  charging_queue.clear();  // the last owner: the robot is deleted here
  std::cout << "after charging_queue.clear(): no owner left\n";
}  // LeakSanitizer reports nothing at exit: the robot was freed
}  // namespace last_owner

// [Slide 60] Passing: Borrow
namespace borrow {
void print_status(const RobotStatus& robot) {  // only reads the robot
  std::cout << "  robot " << robot.id << ": " << robot.battery_pct << " %\n";
}

void run() {
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  print_status(*robot);
  std::cout << "robot.use_count(): " << robot.use_count() << '\n';  // 1: no owner added
}
}  // namespace borrow

// [Slide 61] Passing: Share
namespace share {
void add_to_queue(Fleet& queue, std::shared_ptr<RobotStatus> robot) {  // keeps a copy
  queue.push_back(std::move(robot));
}

void run() {
  Fleet charging_queue;
  auto robot{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  add_to_queue(charging_queue, robot);
  std::cout << "robot.use_count(): " << robot.use_count() << '\n';  // 2: the queue owns it too
}
}  // namespace share

// [Slide 62] Passing: const shared_ptr&
namespace maybe_share {
void queue_if_low(Fleet& queue, const std::shared_ptr<RobotStatus>& robot) {
  if (robot->battery_pct < 20.0) {
    queue.push_back(robot);  // copies only when it keeps it
  }
}

void run() {
  Fleet charging_queue;
  auto low{std::make_shared<RobotStatus>(3, 14.0, Position{2.0, 3.0}, false)};
  auto full{std::make_shared<RobotStatus>(4, 90.0, Position{6.0, 2.0}, false)};
  queue_if_low(charging_queue, low);
  queue_if_low(charging_queue, full);
  std::cout << "low.use_count(): " << low.use_count() << ", full.use_count(): "
            << full.use_count() << ", charging_queue.size(): " << charging_queue.size()
            << '\n';  // 2 1 1
}
}  // namespace maybe_share

// [Slide 63] Returning
namespace returning {
std::unique_ptr<RobotStatus> make_robot(int id) {
  return std::make_unique<RobotStatus>(id, 100.0, Position{0.0, 0.0}, false);
}

void run() {
  std::shared_ptr<RobotStatus> robot{make_robot(5)};  // unique_ptr to shared_ptr: OK
  std::cout << "robot->id: " << robot->id << ", robot.use_count(): " << robot.use_count()
            << '\n';  // 5 1
  // Does not compile: no matching function for call to
  //                   'std::unique_ptr<RobotStatus>::unique_ptr(<brace-enclosed initializer list>)'
  // std::unique_ptr<RobotStatus> back{robot};
}
}  // namespace returning

// The slides of this section: number, title, the function that runs it, and
// true when a full run must skip it.
std::vector<Slide> slides() {
  return {
      {54, "Creating One", creating::run},
      {55, "The Control Block", control_block::run},
      {56, "use_count, Step by Step", use_count_steps::run},
      {57, "Three Ways to Let Go", let_go::run},
      {59, "Deletion by the Last Owner", last_owner::run},
      {60, "Passing: Borrow", borrow::run},
      {61, "Passing: Share", share::run},
      {62, "Passing: const shared_ptr&", maybe_share::run},
      {63, "Returning", returning::run},
  };
}

}  // namespace shared
