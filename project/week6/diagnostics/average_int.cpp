#include <concepts>
#include <vector>

template <std::floating_point T>
T average_of(const std::vector<T>& values) {
  T sum{0};
  for (const T& v : values) { sum += v; }
  return sum / static_cast<T>(values.size());
}

int main() {
  double a{average_of(std::vector<double>{82.5, 35.0, 64.0, 18.0})};  // 49.875
  int b{average_of(std::vector<int>{80, 35, 64, 18})};  // rejected
  return a > b;
}

// [Slide 44] A Call That Compiles and Is Wrong
// DOES NOT COMPILE, on purpose. Without the concept, the int call compiles and
// returns 49 instead of 49.25 (integer division). With it, the call is refused.
// Compile it by hand: 702g++ average_int.cpp
