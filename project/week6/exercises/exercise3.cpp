#include <concepts>

template <typename T>
T larger_of(T a, T b) { return a > b ? a : b; }

template <std::integral T>
T half_of(T n) { return n / 2; }

int main() {
  auto a = larger_of(2, 9);            // 1
  auto b = larger_of(2.5, 9);          // 2
  auto c = larger_of<double>(2.5, 9);  // 3
  auto d = half_of(9);                 // 4
  auto e = half_of(9.0);               // 5
}

// [Slide 47] Exercise 3: Which Calls Compile?
// This file is NOT a build target: some of its calls do not compile, which is
// the exercise. Write your answer first, then compile it by hand and read the
// errors: 702g++ exercise3.cpp
