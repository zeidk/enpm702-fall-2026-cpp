// [Slide 48] No Specifier Means private Inheritance.
// RoboTaxi names Vehicle with no access specifier, so the base is private.
#include <iostream>

class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

 private:
  double max_speed_;
  double speed_{0.0};
};

class RoboTaxi : Vehicle {  // no public before Vehicle
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
};

int main() {
  RoboTaxi robotaxi{13.4};
  std::cout << robotaxi.get_speed() << '\n';  // an error
}
