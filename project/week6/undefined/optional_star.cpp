#include <iostream>
#include <optional>

int main() {
  std::optional<int> idle;
  std::cout << *idle << '\n';
}

// [Slide 28] An Empty Optional
// UNDEFINED BEHAVIOR. * does not check whether the optional holds a value.
// Whatever it prints means nothing, and may change with the next build.
// Run: 702run week6_optional_star
