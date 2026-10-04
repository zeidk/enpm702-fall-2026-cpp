#include <iostream>

int main() {
  int speed{10};
  auto by_value = [speed] { return speed; };
  auto by_ref = [&speed] { return speed; };
  auto counter = [n = speed]() mutable { return n++; };
  speed = 20;

  std::cout << by_value() << ' ' << by_ref() << '\n';
  std::cout << counter() << ' ' << counter() << ' '
            << speed << '\n';
}

// [Slide 62] Exercise 4: Capture Timing
// Write your answer first, then: 702build week6_ex4 && 702run week6_ex4
