int main() {
  int assigned{0};
  auto assign = [assigned]() { ++assigned; };
  assign();
}

// [Slide 56] mutable and Init-capture
// DOES NOT COMPILE. A copy captured by value is read-only inside the body.
// mutable lifts that. Compile it by hand: 702g++ capture_const.cpp
