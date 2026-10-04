template <typename T>
T clamp_value(T value, T low, T high) {
  if (value < low) { return low; }
  if (value > high) { return high; }
  return value;
}

int main() {
  double pct{clamp_value(104, 0.0, 100.0)};
  return pct > 50.0;
}

// [Slide 37] One T for Every Argument
// DOES NOT COMPILE. 104 is an int and 0.0 a double: no single T fits all
// three arguments. Compile it by hand: 702g++ deduce_conflict.cpp
