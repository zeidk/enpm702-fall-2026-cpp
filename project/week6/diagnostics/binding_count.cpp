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

int main() {
  RobotStatus r{3, 64.0};
  auto [id, battery_pct] = r;
}

// [Slide 25] One Name per Member
// DOES NOT COMPILE. A structured binding needs one name for each of the four
// members. Compile it by hand: 702g++ binding_count.cpp
