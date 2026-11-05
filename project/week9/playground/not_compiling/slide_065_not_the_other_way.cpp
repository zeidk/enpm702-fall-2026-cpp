// [Slide 65] Not the Other Way. A Taxi* converts to a Vehicle* on its own;
// a Vehicle* does not convert back to a Taxi*.
#include <string>
#include <utility>

class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

 private:
  double max_speed_;
  double speed_{0.0};
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;
};

int main() {
  Taxi taxi{13.4, "Ana"};
  Vehicle* pointer{&taxi};
  Taxi* back{pointer};  // does not compile
  (void)back;
}
