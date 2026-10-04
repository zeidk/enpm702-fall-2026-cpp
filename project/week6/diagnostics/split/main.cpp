#include "stats.hpp"

int main() {
  double pct{clamp_value(104.2, 0.0, 100.0)};
  return pct > 50.0;
}

// [Slide 40] Templates Go in Headers
// COMPILES, then FAILS TO LINK. main.cpp sees only the declaration, so it
// cannot write clamp_value<double>, and stats.cpp never wrote it either.
// Try it by hand: 702g++ main.cpp stats.cpp
