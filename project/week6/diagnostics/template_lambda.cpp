int main() {
  auto larger = [](const auto& a, const auto& b) {
    return a > b ? a : b;
  };
  auto larger_same = []<typename T>(const T& a, const T& b) {
    return a > b ? a : b;
  };
  larger(3, 7.5);       // 7.5
  larger_same(3, 7.5);  // rejected
}

// [Slide 65] Template Lambdas (C++20)
// DOES NOT COMPILE, on purpose. T appears twice, so both arguments must have
// one type. Compile it by hand: 702g++ template_lambda.cpp
