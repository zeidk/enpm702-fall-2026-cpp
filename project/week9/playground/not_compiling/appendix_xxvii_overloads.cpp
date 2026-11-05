// [Appendix xxvii] Overloads Cannot Meet the Challenge. The compiler picks an
// overload from the static type: *vehicle is a const Vehicle, and no overload
// takes a Vehicle.
#include <string>
#include <utility>
#include <vector>

struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}

 private:
  double max_speed_;
};

class RoboTaxi : public Vehicle {
 public:
  using Vehicle::Vehicle;
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;
};

void start_shift(const RoboTaxi& robotaxi, const Route& route);
void start_shift(const Taxi& taxi, const Route& route);

int main() {
  Route maya_ride;
  RoboTaxi robotaxi{13.4};
  Taxi taxi{13.4, "Ana"};
  std::vector<Vehicle*> by_pointer{&robotaxi, &taxi};  // by_pointer of the challenge
  for (const Vehicle* vehicle : by_pointer) {
    start_shift(*vehicle, maya_ride);  // does not compile
  }
}
