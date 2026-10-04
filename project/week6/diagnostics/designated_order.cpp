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
  RobotStatus a{.id = 1, .battery_pct = 82.5};  // idle at (0, 0)
  RobotStatus b{.id = 2, .busy = true};         // battery 100
  RobotStatus c{.battery_pct = 50.0, .id = 5};  // wrong order
  return a.id + b.id + c.id;
}

// [Slide 13] Designated Initializers (C++20)
// DOES NOT COMPILE. The names must follow the order of the declaration.
// Compile it by hand: 702g++ designated_order.cpp
