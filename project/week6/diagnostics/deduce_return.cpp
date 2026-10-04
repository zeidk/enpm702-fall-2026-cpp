template <typename T>
T make_zero() {
  return T{};
}

int main() {
  double z{make_zero()};
  return z > 0.0;
}

// [Slide 44] Deduction Uses the Arguments Only
// DOES NOT COMPILE. No argument mentions T, and the variable receiving the
// result is not used for deduction. Compile it by hand: 702g++ deduce_return.cpp
