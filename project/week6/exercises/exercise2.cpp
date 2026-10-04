#include <iostream>
#include <optional>
#include <utility>
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

std::optional<double> find_lowest_battery(const std::vector<RobotStatus>& fleet) {
  if (fleet.empty()) { return std::nullopt; }
  double lowest{fleet.front().battery_pct};
  for (const auto& r : fleet) {
    if (r.battery_pct < lowest) { lowest = r.battery_pct; }
  }
  return lowest;
}

int main() {
  std::vector<RobotStatus> fleet{{1, 82.5}, {2, 35.0}, {3, 64.0}, {4, 18.0}};

  std::pair<int, double> charge{3, 64.0};
  auto [id, pct] = charge;
  pct = 100.0;
  auto& [id_ref, pct_ref] = charge;
  pct_ref = 70.0;
  std::cout << charge.first << ' ' << charge.second << '\n';

  std::cout << find_lowest_battery(fleet).value_or(-1.0) << ' '
            << find_lowest_battery({}).value_or(-1.0) << '\n';
}

// [Slide 30] Exercise 2: Bindings and Optionals
// Write your answer first, then: 702build week6_ex2 && 702run week6_ex2
