void convert_all(double* values, int n, double (*convert)(double)) {
  for (int i{0}; i < n; ++i) { values[i] = convert(values[i]); }
}

int main() {
  double battery[]{82.5, 35.0};
  double scale{2.0};
  convert_all(battery, 2, [](double x) { return x / 100.0; });  // OK
  convert_all(battery, 2, [scale](double x) { return scale * x; });
}

// [Slide 65] Passing a Function
// DOES NOT COMPILE. A lambda that captures carries data, and a function pointer
// has nowhere to put it. Compile it by hand: 702g++ fnptr_capture.cpp
