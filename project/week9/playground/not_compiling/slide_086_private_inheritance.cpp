// [Slide 86] Step 1: public Inheritance. With private inheritance, code
// outside RoboTaxi cannot treat a RoboTaxi as a Vehicle.
#include <iostream>

struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }
};

class RoboTaxi : private Vehicle {  // private, not public
 public:
  void drive(const Route& route) const override {
    std::cout << "RoboTaxi: drives itself " << route.length() << " m\n";
  }
};

void start_shift(const Vehicle& vehicle, const Route& route) { vehicle.drive(route); }

int main() {
  const Route maya_ride;
  RoboTaxi robotaxi;
  start_shift(robotaxi, maya_ride);  // does not compile
}
