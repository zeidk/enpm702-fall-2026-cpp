// [Slide 98] Forgetting One Override. Taxi inherits drive(...) = 0 and gives
// it no body, so Taxi is abstract too.
#include <string>
#include <utility>

struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const = 0;
};

class Taxi : public Vehicle {  // no drive()
 public:
  explicit Taxi(double /*max_speed*/, std::string driver_name)
      : driver_name_{std::move(driver_name)} {}

 private:
  std::string driver_name_;
};

int main() {
  Taxi taxi{13.4, "Ana"};  // does not compile
}
