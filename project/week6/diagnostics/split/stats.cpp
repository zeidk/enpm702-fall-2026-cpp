#include "stats.hpp"

template <typename T>
T clamp_value(T value, T low, T high) {
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

// [Slide 40] Templates Go in Headers
// Compiled alone, this file produces no function at all: nothing here calls
// clamp_value, so nothing is instantiated. Check: 702g++ -c stats.cpp && nm -C stats.o
