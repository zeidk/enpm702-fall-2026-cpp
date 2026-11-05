// [Appendix xli] override Catches Both. The two mistakes of [Appendix xl], A
// Typo That Compiles, now marked override: both stop the build.
struct Route {
  [[nodiscard]] double length() const noexcept { return 500.0; }
};

class Vehicle {
 public:
  virtual ~Vehicle() = default;
  virtual void drive(const Route& route) const;
};

class RoboTaxi : public Vehicle {
 public:
  void drvie(const Route& route) const override;  // does not compile
  void drive(const Route& route) override;        // does not compile
};

int main() {}
