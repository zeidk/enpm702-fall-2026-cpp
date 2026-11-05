/**
 * @file abstraction.cpp
 * @brief L9 Sections 4 and 5, Abstraction and final: the code of every slide.
 * @author Zeid Kootbally
 *
 * @details Part of week9_playground. These slides use the final classes of
 * the design, in ../include: the abstract Vehicle, RoboTaxi and Taxi (both
 * final), Fleet, Passenger, Route and Position.
 *
 * A slide whose code does not compile has one file in ../not_compiling, with
 * the error g++ 13.3 gives. A single line that does not compile, inside code
 * that runs, stays here as a comment with its error.
 *
 * The frames that Live Coding: Steps 5 and 6 replace are in
 * appendix.cpp: [Appendix iv] to [Appendix vi].
 * A final Method is there too: [Appendix vii], and Discussion: Add a RoboBus:
 * [Appendix xxxiii].
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

// [Slide 98] Forgetting One Override: does not compile. The code and its error are in
// ../not_compiling/slide_098_forgetting_one_override.cpp.

// [Slide 99] Fleet and Passenger Do Not Change
namespace whole_program {
void run() {
  const Route maya_ride{make_trip()};
  auto robotaxi{std::make_shared<RoboTaxi>(13.4)};
  robotaxi->add_sensor(SensorType::lidar, 100.0);
  robotaxi->add_sensor(SensorType::camera, 80.0);
  auto taxi{std::make_shared<Taxi>(13.4, "Ana")};

  Fleet fleet;
  fleet.add(robotaxi);  // converts to std::shared_ptr<Vehicle>
  fleet.add(taxi);
  Passenger maya{"Maya"};
  maya.board(robotaxi);

  const std::vector<std::shared_ptr<Vehicle>> today{robotaxi, taxi};
  for (const std::shared_ptr<Vehicle>& vehicle : today) {
    vehicle->drive(maya_ride);  // maya_ride: the 500 m route
  }
}
}  // namespace whole_program

// [Slide 104] The Reason for Each final: does not compile. The code and its error are in
// ../not_compiling/slide_104_fast_robotaxi.cpp.

std::vector<Slide> slides() {
  return {
      {99, "Fleet and Passenger Do Not Change", whole_program::run},
  };
}

}  // namespace abstraction
