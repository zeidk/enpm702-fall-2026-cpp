/**
 * @file appendix.cpp
 * @brief L9: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_appendix.
 *
 * @code
 * 702run week9_appendix        # every appendix frame
 * 702run week9_appendix iv     # only [Appendix iv]
 * @endcode
 */
#include <iostream>
#include <memory>
#include <string>

#include "position.hpp"
#include "route.hpp"
#include "slides.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace {

// [Appendix 4] Multiple Inheritance
namespace multiple {
class Chargeable {  // anything that can be charged
 public:
  virtual ~Chargeable() = default;
  virtual void charge() = 0;
};

class ElectricTaxi final : public Vehicle, public Chargeable {
 public:
  explicit ElectricTaxi(double max_speed) : Vehicle{max_speed} {}
  void drive(const Route& route) const override {
    std::cout << "ElectricTaxi: drives " << route.length() << " m\n";
  }
  void charge() override { std::cout << "ElectricTaxi: charges\n"; }
};

void run() {
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  ElectricTaxi taxi{13.4};
  Vehicle& vehicle{taxi};
  Chargeable& chargeable{taxi};
  vehicle.drive(trip);  // ElectricTaxi: drives 500 m
  chargeable.charge();  // ElectricTaxi: charges
}
}  // namespace multiple

// [Appendix 7] Copying a Vehicle. Neither compiles:
//   Taxi copy{taxi};             // the sensors cannot be copied (std::unique_ptr, L8)
//   Taxi moved{std::move(taxi)};  // the declared destructor stops the moves
// The errors are on the frame.

}  // namespace

int main(int argc, char* argv[]) {
  return run_slides({{4, "Multiple Inheritance", multiple::run}}, argc, argv,
                    Part::appendix);
}
