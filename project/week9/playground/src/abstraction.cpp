/**
 * @file abstraction.cpp
 * @brief L9 Sections 4 and 5, Abstraction and final: the code of every slide.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. These slides use the final classes of
 * the design, in ../../common: the abstract Vehicle, RoboTaxi and Taxi (both
 * final), Fleet, Passenger, Route and Position.
 *
 * The slides that do not compile are kept here as comments, with the error
 * g++ 13.3 gives (-std=c++20 -Wall -Wextra -pedantic-errors -Wshadow).
 *
 * The frames that Live Coding: Steps 6 and 7 replace are in
 * ../../appendix/src/appendix.cpp: [Appendix vi], [Appendix vii] and [Appendix viii].
 */
#include <memory>
#include <vector>

#include "fleet.hpp"
#include "passenger.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "sections.hpp"
#include "taxi.hpp"
#include "vehicle.hpp"

namespace abstraction {

using fleet::Fleet;
using fleet::Passenger;
using fleet::Position;
using fleet::RoboTaxi;
using fleet::Route;
using fleet::SensorType;
using fleet::Taxi;
using fleet::Vehicle;

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route maya_ride;
  maya_ride.add(Position{0.0, 0.0});
  maya_ride.add(Position{300.0, 400.0});
  return maya_ride;
}

// [Slide 101] Forgetting One Override. Taxi without drive():
//   Taxi taxi{13.4, "Ana"};
// error: cannot declare variable 'taxi' to be of abstract type 'Taxi'
// note:   because the following virtual functions are pure within 'Taxi':
// note:     'virtual void Vehicle::drive(const Route&) const'

// [Slide 102] The Whole Program
namespace whole_program {
void run() {
  const Route maya_ride{make_trip()};
  auto robotaxi{std::make_shared<RoboTaxi>(13.4)};
  robotaxi->add_sensor(SensorType::lidar, 100.0);
  robotaxi->add_sensor(SensorType::camera, 80.0);
  auto taxi{std::make_shared<Taxi>(13.4, "Ana")};

  Fleet fleet;
  fleet.add(robotaxi);  // std::shared_ptr<RoboTaxi> converts
  fleet.add(taxi);
  Passenger maya{"Maya"};
  maya.board(robotaxi);

  const std::vector<std::shared_ptr<Vehicle>> today{robotaxi, taxi};
  for (const std::shared_ptr<Vehicle>& vehicle : today) {
    vehicle->drive(maya_ride);
  }
}
}  // namespace whole_program

// [Slide 108] A final Method. Does not compile:
//   class Taxi : public Vehicle {  // Taxi not final, drive() final
//    public:
//     void drive(const Route& route) const final;
//   };
//   class NightTaxi : public Taxi {
//    public:
//     void drive(const Route& route) const override;
//   };
// error: virtual function 'virtual void NightTaxi::drive(const Route&) const' overriding final function
// note: overridden function is 'virtual void Taxi::drive(const Route&) const'

std::vector<Slide> slides() {
  return {
      {102, "The Whole Program", whole_program::run},
  };
}

}  // namespace abstraction
