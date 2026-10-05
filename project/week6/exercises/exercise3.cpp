// [Slide 49] Exercise 3: The Largest Value
//
// About 5 minutes. Build and run:  702build week6_ex3 && 702run week6_ex3
//
// 1. Write a function template largest_of that takes a const std::vector<T>&
//    and returns its largest element. The vector is never empty.
// 2. Constrain T with the standard concept std::totally_ordered.
//    Un-comment the first line in main, build and run.
// 3. Un-comment the second line in main, build, and read the first error.
//    Which requirement does RobotStatus fail? Then comment it out again.
//
// Expected output after step 2:
//   4 82.5 dock
#include <concepts>
#include <iostream>
#include <string>
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

// Steps 1 and 2: largest_of goes here.

int main() {
  const std::vector<int> ids{3, 1, 4, 2};
  const std::vector<double> battery_pct{82.5, 35.0, 64.0, 18.0};
  const std::vector<std::string> zones{"dock", "aisle 4", "aisle 7"};
  const std::vector<RobotStatus> fleet{{1, 82.5}, {2, 35.0}};

  // std::cout << largest_of(ids) << ' ' << largest_of(battery_pct) << ' ' << largest_of(zones) << '\n';
  // RobotStatus best{largest_of(fleet)};
}
