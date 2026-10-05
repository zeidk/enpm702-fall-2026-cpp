#include <iostream>
#include <optional>

int main() {
  std::optional<int> idle;
  std::cout << idle.value() << '\n';
}

// [Slide 31] An Empty Optional
// THROWS. value() on an empty optional throws std::bad_optional_access. Nothing
// catches it, so the program stops with exit status 134. How to catch it is in
// the exceptions reading. Run: 702run week6_optional_value
