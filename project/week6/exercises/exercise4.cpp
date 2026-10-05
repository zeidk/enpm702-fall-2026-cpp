// [Slide 64] Exercise 4: Choose a Robot
//
// About 5 minutes. Build and run:  702build week6_ex4 && 702run week6_ex4
//
// 1. Count the robots that can take a task: idle, with at least
//    min_battery_pct. Use std::count_if and a lambda that captures
//    min_battery_pct. Print the count.
// 2. Sort the fleet by distance to pickup, nearest first, with
//    std::ranges::sort and a projection that captures pickup. Distance is
//    std::hypot(dx, dy). Print the ids in order.
//
// Expected output:
//   2
//   4 3 2 1
#include <algorithm>
#include <cmath>
#include <iostream>
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

int main() {
  std::vector<RobotStatus> fleet{make_fleet()};

  // Step 1: un-comment the next line, then count with std::count_if.
  // const double min_battery_pct{40.0};

  // Step 2: un-comment the next line, then sort with std::ranges::sort and
  // print the ids.
  // const Position pickup{5.0, 5.0};
}
