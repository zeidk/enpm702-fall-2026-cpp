/**
 * @file undefined.cpp
 * @brief L9: the code of every slide that has undefined behavior.
 * @author Zeid Kootbally
 *
 * @details Always compiled with -fsanitize=address,undefined (see
 * ../../CMakeLists.txt). The reports on the slides come from these runs.
 */
#include "undefined.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "position.hpp"
#include "route.hpp"

namespace undefined {

using fleet::Position;
using fleet::Route;

// [Slide 78] Deleting through a Base Pointer
// ~Vehicle is not virtual. The std::unique_ptr<Vehicle> deletes a Taxi through
// a Vehicle*: undefined behavior ([expr.delete] 7.6.2.8 paragraph 3). Only
// ~Vehicle runs. AddressSanitizer stops the program; with that check off,
//   ASAN_OPTIONS=new_delete_type_mismatch=0 702run week9_playground 78
// LeakSanitizer finds the driver's name, which ~Taxi never gave back. The name
// is longer than 15 characters, so std::string keeps it on the heap.
namespace nonvirtual_dtor {
class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}
  ~Vehicle() { std::cout << "~Vehicle\n"; }  // not virtual
  virtual void drive(const Route& route) const {
    std::cout << "Vehicle: drives " << route.length() << " m\n";
  }

 private:
  double max_speed_;
  double speed_{0.0};
};

class Taxi : public Vehicle {
 public:
  Taxi(double max_speed, std::string driver_name)
      : Vehicle{max_speed}, driver_name_{std::move(driver_name)} {}
  ~Taxi() { std::cout << "~Taxi\n"; }
  void drive(const Route& route) const override {
    std::cout << "Taxi: " << driver_name_ << " drives " << route.length() << " m\n";
  }

 private:
  std::string driver_name_;
};

void run() {
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  std::unique_ptr<Vehicle> vehicle{std::make_unique<Taxi>(13.4, "Ana Maria Rodrigues")};
  vehicle->drive(trip);
}  // deletes a Vehicle*: only ~Vehicle runs
}  // namespace nonvirtual_dtor

std::vector<Slide> slides() {
  return {
      {78, "Deleting through a Base Pointer", nonvirtual_dtor::run, true},
  };
}

}  // namespace undefined
