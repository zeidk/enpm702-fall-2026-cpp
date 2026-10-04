int main() {
  auto speed_for = [](double battery_pct) {
    if (battery_pct < 20.0) { return 0; }
    return 0.01 * battery_pct;
  };
}

// [Slide 64] The Return Type
// DOES NOT COMPILE. One return gives an int, the other a double.
// Compile it by hand: 702g++ lambda_return.cpp
