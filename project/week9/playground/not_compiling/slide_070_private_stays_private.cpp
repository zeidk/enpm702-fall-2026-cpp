// [Slide 70] private Stays private. A RoboTaxi object holds speed_, but
// RoboTaxi's methods cannot name it.
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  [[nodiscard]] double get_speed() const noexcept { return speed_; }
  [[nodiscard]] double get_max_speed() const noexcept { return max_speed_; }

 private:
  double max_speed_;
  double speed_{0.0};
};

class RoboTaxi : public Vehicle {  // Vehicle: speed_ is private
 public:
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}
  void go();
};

void RoboTaxi::go() { speed_ = get_max_speed(); }  // does not compile

int main() {
  RoboTaxi robotaxi{13.4};
  robotaxi.go();
}
