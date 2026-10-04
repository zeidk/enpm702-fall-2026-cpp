#include <iostream>

struct ImuSample {
  float accel_x;
  double stamp_s;
  float accel_y;
};

struct ImuSampleSorted {
  double stamp_s;
  float accel_x;
  float accel_y;
};

struct Task {
  int id{0};
  double x{0.0};
  double y{0.0};
  double deadline_s{60.0};
};

int main() {
  Task t{.id = 17, .y = 2.0};
  Task* tp{&t};
  tp->x += 1.5;
  std::cout << sizeof(ImuSample) << ' ' << sizeof(ImuSampleSorted) << '\n';
  std::cout << t.id << ' ' << t.x << ' ' << t.y << ' ' << t.deadline_s << '\n';
}

// [Slide 19] Exercise 1: Sizes and Defaults
// Write your answer first, then: 702build week6_ex1 && 702run week6_ex1
