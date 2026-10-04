struct RobotStatus {
  int id{0};
  double battery_pct{100.0};
  bool busy{false};
};

int main() {
  RobotStatus d{.id = 4, 18.0};
  return d.id;
}

// [Slide 13] Designated Initializers (C++20)
// DOES NOT COMPILE. Every value is named, or none is.
// Compile it by hand: 702g++ designated_mixed.cpp
