// [Appendix vii] A final Method. NightTaxi can derive from Taxi, but cannot
// override Taxi's final drive().
#include <iostream>

struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const = 0;
};

class Taxi : public Vehicle {  // Taxi is not final here
 public:
  void drive(const Route& route) const final {  // the last override
    std::cout << "Taxi: drives " << route.length() << " m\n";
  }
};

class NightTaxi : public Taxi {
 public:
  void drive(const Route& route) const override;  // does not compile
};

int main() {}
