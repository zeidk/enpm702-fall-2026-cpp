// [Slide 104] The Reason for Each final. FastRoboTaxi compiles only if
// RoboTaxi were not final. RoboTaxi is final, so it does not compile.
#include <iostream>

struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const = 0;
};

class RoboTaxi final : public Vehicle {
 public:
  void drive(const Route& route) const override {  // checks the sensors first
    std::cout << "RoboTaxi: drives " << route.length() << " m\n";
  }
};

class FastRoboTaxi : public RoboTaxi {  // does not compile: RoboTaxi is final
 public:
  void drive(const Route& route) const override;
};

// no sensor check: it drives with 0 sensors
void FastRoboTaxi::drive(const Route& route) const {
  std::cout << "FastRoboTaxi: drives " << route.length() << " m\n";
}

int main() {}
