/**
 * @file types.cpp
 * @brief L6 Section 1, Types You Write: the code of every slide, runnable.
 *
 * @details Build target: @c week6_types. This file stands alone: it includes
 * no project header.
 *
 * @par How to use it
 * @code
 * 702build week6_types
 * 702run week6_types        # every slide of the section, in order
 * 702run week6_types 12     # only [Slide 12]
 * @endcode
 * The number is the frame number in the slide's top-left corner. Each slide's
 * code sits in its own namespace, so two slides can both declare a
 * @c RobotStatus without a clash. The comment above a namespace names its
 * slide, and @c main() at the bottom runs them.
 *
 * Code that does NOT compile, or that warns, is in @c ../diagnostics/, one
 * file per slide. The expected output of this program is in
 * @c ../tests/expected/types.txt, and @c 702test checks it.
 */
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

// [Slide 11] Three Values, One Status
namespace three_values {
// Without a struct: three reference parameters carry the answer back.
void get_status(int id, double& battery_pct, double& x, double& y) {
  if (id == 3) {
    battery_pct = 64.0;
    x = 2.0;
    y = 3.0;
  } else {
    battery_pct = 0.0;
    x = 0.0;
    y = 0.0;
  }
}

// With a struct: one return value carries all three.
struct RobotStatus {
  double battery_pct;
  double x;
  double y;
};

RobotStatus get_status(int id) {
  if (id == 3) { return {64.0, 2.0, 3.0}; }
  return {0.0, 0.0, 0.0};
}

void run() {
  double battery_pct{};
  double x{};
  double y{};
  get_status(3, battery_pct, x, y);
  std::cout << battery_pct << ' ' << x << ' ' << y << '\n';  // 64 2 3

  RobotStatus s{get_status(3)};
  std::cout << s.battery_pct << ' ' << s.x << ' ' << s.y << '\n';  // 64 2 3
}
}  // namespace three_values

// [Slide 12] Declaring a Struct
namespace declaring {
struct Position {
  double x;  // metres, warehouse frame
  double y;
};

struct RobotStatus {
  int id;
  double battery_pct;
  Position position;  // a struct inside a struct
  bool busy;
};

void run() {
  RobotStatus r{3, 64.0, {2.0, 3.0}, false};
  std::cout << r.id << ' ' << r.battery_pct << '\n';  // 3 64
}
}  // namespace declaring

// [Slide 13] Member Access
namespace member_access {
struct Position {
  double x;
  double y;
};

struct RobotStatus {
  int id;
  double battery_pct;
  Position position;
  bool busy;
};

void run() {
  RobotStatus r{3, 64.0, {2.0, 3.0}, false};
  std::cout << r.id << ' ' << r.position.x << '\n';  // 3 2

  RobotStatus* rp{&r};
  rp->battery_pct -= 10.0;  // the same as (*rp).battery_pct -= 10.0;
  std::cout << r.battery_pct << '\n';  // 54
}
}  // namespace member_access

// [Slide 15] Initializing a Struct
// RobotStatus b{3}; warns under -Wextra: see ../diagnostics/missing_initializer.cpp.
// RobotStatus d; is not run here: reading d is undefined behavior.
namespace aggregate_init {
struct Position {
  double x;
  double y;
};

struct RobotStatus {
  int id;
  double battery_pct;
  Position position;
  bool busy;
};

void print(const RobotStatus& r) {
  std::cout << r.id << ' ' << r.battery_pct << " (" << r.position.x << ", " << r.position.y
            << ") " << (r.busy ? "busy" : "idle") << '\n';
}

void run() {
  RobotStatus a{3, 64.0, {2.0, 3.0}, false};
  RobotStatus c{};
  print(a);  // 3 64 (2, 3) idle
  print(c);  // 0 0 (0, 0) idle
}
}  // namespace aggregate_init

// [Slide 16] Default Member Initializers
namespace default_members {
struct Position {
  double x{0.0};
  double y{0.0};
};

struct RobotStatus {
  int id{0};
  double battery_pct{100.0};  // a new robot starts charged
  Position position{};
  bool busy{false};
};

void print(const RobotStatus& r) {
  std::cout << r.id << ' ' << r.battery_pct << " (" << r.position.x << ", " << r.position.y
            << ") " << (r.busy ? "busy" : "idle") << '\n';
}

void run() {
  RobotStatus r1{};
  RobotStatus r2{4, 18.0};
  RobotStatus r3{2, 35.0, {4.0, 1.0}, true};
  RobotStatus r4;  // no braces: the defaults still apply
  print(r1);  // 0 100 (0, 0) idle
  print(r2);  // 4 18 (0, 0) idle
  print(r3);  // 2 35 (4, 1) busy
  print(r4);  // 0 100 (0, 0) idle
}
}  // namespace default_members

// [Slide 17] Designated Initializers (C++20)
// The line in the wrong order is in ../diagnostics/designated_order.cpp.
namespace designated {
struct Position {
  double x{0.0};
  double y{0.0};
};

struct RobotStatus {
  int id{0};
  double battery_pct{100.0};
  Position position{};
  bool busy{false};
};

void print(const RobotStatus& r) {
  std::cout << r.id << ' ' << r.battery_pct << " (" << r.position.x << ", " << r.position.y
            << ") " << (r.busy ? "busy" : "idle") << '\n';
}

void run() {
  RobotStatus a{.id = 1, .battery_pct = 82.5};  // idle at (0, 0)
  RobotStatus b{.id = 2, .busy = true};         // battery 100
  print(a);  // 1 82.5 (0, 0) idle
  print(b);  // 2 100 (0, 0) busy
}
}  // namespace designated

// [Slide 18] Structs in Memory
namespace padding {
struct Position {
  double x;
  double y;
};

struct RobotStatus {
  int id;              // 4 bytes
  double battery_pct;  // 8
  Position position;   // 16: two doubles
  bool busy;           // 1
};

void run() {
  std::cout << sizeof(RobotStatus) << '\n';  // 40
  std::cout << alignof(double) << '\n';      // 8
  std::cout << offsetof(RobotStatus, id) << ' ' << offsetof(RobotStatus, battery_pct) << ' '
            << offsetof(RobotStatus, position) << ' ' << offsetof(RobotStatus, busy)
            << '\n';  // 0 8 16 32
}
}  // namespace padding

// [Slide 19] Member Order
namespace member_order {
struct Position {
  double x;
  double y;
};

struct RobotStatusSorted {
  double battery_pct;
  Position position;
  int id;
  bool busy;
};

void run() {
  std::cout << sizeof(RobotStatusSorted) << '\n';  // 32
  std::cout << offsetof(RobotStatusSorted, battery_pct) << ' '
            << offsetof(RobotStatusSorted, position) << ' ' << offsetof(RobotStatusSorted, id)
            << ' ' << offsetof(RobotStatusSorted, busy) << '\n';  // 0 8 24 28
}
}  // namespace member_order

// [Slide 20] Structs and Functions
namespace struct_functions {
struct Position {
  double x{0.0};
  double y{0.0};
};

struct RobotStatus {
  int id{0};
  double battery_pct{100.0};
  Position position{};
  bool busy{false};
};

double get_battery(const RobotStatus& r) {  // copies nothing
  return r.battery_pct;
}

RobotStatus make_new_robot(int id) {  // returns by value
  return {id, 100.0, {0.0, 0.0}, false};
}

void run() {
  std::cout << sizeof(RobotStatus) << '\n';                 // 40
  std::cout << get_battery(make_new_robot(5)) << '\n';      // 100
}
}  // namespace struct_functions

// [Slide 21] A Vector of Structs
namespace vector_of_structs {
struct Position {
  double x{0.0};
  double y{0.0};
};

struct RobotStatus {
  int id{0};
  double battery_pct{100.0};
  Position position{};
  bool busy{false};
};

void run() {
  std::vector<RobotStatus> fleet{{1, 82.5, {0.0, 0.0}, false},
                                 {2, 35.0, {4.0, 1.0}, true},
                                 {3, 64.0, {2.0, 3.0}, false},
                                 {4, 18.0, {6.0, 2.0}, false}};
  for (const auto& r : fleet) {
    std::cout << "robot " << r.id << ": " << r.battery_pct << " %"
              << (r.busy ? ", busy\n" : ", idle\n");
  }
}
}  // namespace vector_of_structs

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: Section 5, Function Pointers.
void show(int only, int slide, const char* title, void (*run)()) {
  if (only != 0 && only != slide) { return; }
  std::cout << "[Slide " << slide << "] " << title << '\n';
  run();
}

int main(int argc, char* argv[]) {
  const int only{argc > 1 ? std::atoi(argv[1]) : 0};
  show(only, 11, "Three Values, One Status", three_values::run);
  show(only, 12, "Declaring a Struct", declaring::run);
  show(only, 13, "Member Access", member_access::run);
  show(only, 15, "Initializing a Struct", aggregate_init::run);
  show(only, 16, "Default Member Initializers", default_members::run);
  show(only, 17, "Designated Initializers (C++20)", designated::run);
  show(only, 18, "Structs in Memory", padding::run);
  show(only, 19, "Member Order", member_order::run);
  show(only, 20, "Structs and Functions", struct_functions::run);
  show(only, 21, "A Vector of Structs", vector_of_structs::run);
}
