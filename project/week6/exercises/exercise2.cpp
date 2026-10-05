// [Slide 33] Exercise 2: Find a Robot
//
// About 5 minutes. Build and run:  702build week6_ex2 && 702run week6_ex2
//
// 1. Write std::optional<RobotStatus> find_robot(
//        const std::vector<RobotStatus>& fleet, int id)
//    It returns the robot with that id, or an empty optional.
// 2. In main, look up robot 3. If it is found, unpack it with a structured
//    binding and print its four members on one line.
// 3. Look up robot 9, and print that it was not found.
//
// Expected output:
//   robot 3: 64 % at (2, 3), idle
//   robot 9: not found
#include <iostream>
#include <optional>
#include <vector>

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

// The demo fleet from the slides.
std::vector<RobotStatus> make_fleet() {
  return {{1, 82.5, {0.0, 0.0}, false},
          {2, 35.0, {4.0, 1.0}, true},
          {3, 64.0, {2.0, 3.0}, false},
          {4, 18.0, {6.0, 2.0}, false}};
}

// Step 1: find_robot goes here.

int main() {
  const std::vector<RobotStatus> fleet{make_fleet()};

  // Step 2: robot 3.

  // Step 3: robot 9.
}
