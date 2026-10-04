#include <iostream>

auto make_filter(double limit_pct) {
  return [&limit_pct](double pct) { return pct < limit_pct; };
}

int main() {
  auto is_low = make_filter(40.0);
  std::cout << is_low(30.0) << '\n';  // should be 1
}

// [Slide 57] A Dangling Capture
// UNDEFINED BEHAVIOR. limit_pct is a parameter: it dies when make_filter
// returns, and the lambda keeps a reference to it. This target is always
// built with AddressSanitizer, so the run stops with a report that names the
// bug and the line. Run: 702run week6_dangling
