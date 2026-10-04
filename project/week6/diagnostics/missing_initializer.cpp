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

int main() {
  RobotStatus b{3};  // the rest are 0 and false
  return b.id;
}

// [Slide 10] Initializing a Struct
// COMPILES, WITH A WARNING under -Wextra: the braced list stops before the
// last three members. They are set to zero anyway; the warning asks whether
// you meant it. Compile it by hand: 702g++ missing_initializer.cpp
