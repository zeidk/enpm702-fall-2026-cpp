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

Route make_trip() {  // 500 m: (0, 0) to (300, 400)
  Route trip;
  trip.add(Position{0.0, 0.0});
  trip.add(Position{300.0, 400.0});
  return trip;
}

// [Slide 76] No Vehicle Objects. Does not compile:
//   Vehicle vehicle{13.4};
// error: cannot declare variable 'vehicle' to be of abstract type 'Vehicle'
// note:   because the following virtual functions are pure within 'Vehicle':
// note:     'virtual void Vehicle::drive(const Route&) const'
// A reference or a pointer to Vehicle is fine:
namespace no_vehicle {
void run() {
  RoboTaxi robotaxi{13.4};
  robotaxi.add_sensor(SensorType::lidar, 100.0);
  const Vehicle& vehicle{robotaxi};
  vehicle.drive(make_trip());  // RoboTaxi: 1 sensor checked, drives 500 m
}
}  // namespace no_vehicle

// [Slide 78] Forgetting One Override. Taxi without drive():
//   Taxi taxi{13.4, "Ana"};
// error: cannot declare variable 'taxi' to be of abstract type 'Taxi'

// [Slide 79] RoboTaxi and Taxi
namespace concrete {
void run() {
  const Route trip{make_trip()};
  RoboTaxi robotaxi{13.4};
  robotaxi.drive(trip);  // RoboTaxi: no sensor, stays parked
  robotaxi.add_sensor(SensorType::lidar, 100.0);
  robotaxi.drive(trip);  // RoboTaxi: 1 sensor checked, drives 500 m
  Taxi taxi{13.4, "Ana"};
  taxi.drive(trip);      // Taxi: Ana drives 500 m
}
}  // namespace concrete

// [Slide 80] The Whole Program
namespace whole_program {
void run() {
  const Route trip{make_trip()};
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
    vehicle->drive(trip);
  }
}
}  // namespace whole_program

// [Slide 84] A final Class. Does not compile:
//   class SuperRoboTaxi : public RoboTaxi {};
// error: cannot derive from 'final' base 'RoboTaxi' in derived type 'SuperRoboTaxi'

// [Slide 85] A final Method. Does not compile:
//   class Taxi : public Vehicle {  // Taxi not final, drive() final
//    public:
//     void drive(const Route& route) const final;
//   };
//   class NightTaxi : public Taxi {
//    public:
//     void drive(const Route& route) const override;
//   };
// error: virtual function 'virtual void NightTaxi::drive(const Route&) const' overriding final function

std::vector<Slide> slides() {
  return {
      {76, "No Vehicle Objects", no_vehicle::run},
      {79, "RoboTaxi and Taxi", concrete::run},
      {80, "The Whole Program", whole_program::run},
  };
}

}  // namespace abstraction
