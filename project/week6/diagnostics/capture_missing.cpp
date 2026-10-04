int main() {
  double limit_pct{40.0};
  auto is_low = [](double pct) { return pct < limit_pct; };
}

// [Slide 57] Captures
// DOES NOT COMPILE. A lambda body sees only its parameters and what it
// captures; [limit_pct] fixes it. Compile it by hand: 702g++ capture_missing.cpp
