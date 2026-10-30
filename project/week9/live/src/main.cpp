/**
 * @file main.cpp
 * @brief L9, live coding: the program that uses the classes you write in class.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_live, built with AddressSanitizer and
 * UndefinedBehaviorSanitizer.
 *
 * @code
 * 702build week9_live
 * 702run week9_live
 * @endcode
 *
 * The seven "Live Coding" slides of L9 add to L8's Vehicle in seven steps. We
 * write Step 1 together; you write Steps 2 to 7 on your own.
 * main() has one commented block for each step that prints something: when we
 * finish a step, we uncomment its block, then build and run. Each block prints
 * "[Step N]", then what that step adds. Each class has its own header in ../include and its own source file here. Route, Position and Sensor are the
 * course's own, in ../../common/include. Every class is in the namespace
 * fleet, so main() writes fleet:: before each name.
 */
#include <iostream>
#include <memory>

#include "fleet.hpp"
#include "passenger.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "taxi.hpp"

// Step 7: a class that derives from RoboTaxi, here, above main():
//         class SuperRoboTaxi : public fleet::RoboTaxi {};
//         Build, read the error, then delete it.

int main() {
  // The 500 m trip of the slides: (0, 0) to (300, 400), the long side of a
  // 300, 400, 500 right triangle. Steps 4 and 5 drive it.
  fleet::Route trip;
  trip.add(fleet::Position{0.0, 0.0});
  trip.add(fleet::Position{300.0, 400.0});

  // Step 1: a Passenger does not keep its Vehicle alive.
  // {
  //   std::cout << "[Step 1]\n";
  //   auto car_a{std::make_shared<fleet::Vehicle>(13.4)};  // Step 6: RoboTaxi
  //   fleet::Passenger maya{"Maya"};
  //   maya.board(car_a);
  //   std::cout << car_a.use_count() << '\n';
  //   std::weak_ptr<fleet::Vehicle> watcher{car_a};
  //   car_a.reset();  // the only owner lets go
  //   std::cout << std::boolalpha << watcher.expired() << '\n';
  //   maya.leave();
  // }

  // Step 2: a Fleet shares its vehicles.
  // {
  //   std::cout << "[Step 2]\n";
  //   auto car_a{std::make_shared<fleet::Vehicle>(13.4)};  // Step 6: RoboTaxi
  //   auto car_b{std::make_shared<fleet::Vehicle>(13.4)};  // Step 6: RoboTaxi
  //   fleet::Fleet fleet;
  //   fleet.add(car_a);
  //   fleet.add(car_b);
  //   fleet.add(car_a);  // already in the fleet: not added again
  //   std::cout << fleet.size() << ' ' << car_a.use_count() << '\n';
  //   fleet.remove(car_b->get_id());
  //   std::cout << fleet.size() << ' ' << car_b.use_count() << '\n';
  // }

  // Step 3: RoboTaxi and Taxi use Vehicle's methods, and a Fleet holds both.
  // {
  //   std::cout << "[Step 3]\n";
  //   fleet::RoboTaxi robotaxi{13.4};
  //   robotaxi.add_sensor(fleet::SensorType::lidar, 100.0);
  //   fleet::Taxi taxi{13.4, "Ana"};
  //   taxi.set_speed(10.0);
  //   std::cout << robotaxi.sensor_count() << ' ' << taxi.get_speed() << '\n';
  //   fleet::Fleet fleet;
  //   fleet.add(std::make_shared<fleet::RoboTaxi>(13.4));
  //   fleet.add(std::make_shared<fleet::Taxi>(13.4, "Ana"));
  //   std::cout << fleet.size() << '\n';
  // }

  // Step 4: one loop drives both kinds, each with its own drive().
  // {
  //   std::cout << "[Step 4]\n";
  //   fleet::Fleet fleet;
  //   fleet.add(std::make_shared<fleet::RoboTaxi>(13.4));
  //   fleet.add(std::make_shared<fleet::Taxi>(13.4, "Ana"));
  //   fleet.drive_all(trip);
  // }

  // Step 5: a Taxi deleted through a Vehicle pointer. The name has 19
  //         characters, so std::string keeps it on the heap. Uncomment the
  //         block before Vehicle has a virtual destructor, and run.
  // {
  //   std::cout << "[Step 5]\n";
  //   std::unique_ptr<fleet::Vehicle> vehicle{std::make_unique<fleet::Taxi>(13.4, "Ana Maria Rodrigues")};
  //   vehicle->drive(trip);
  // }

  // Step 6: no Vehicle object. Uncomment the line, build, read the error, then
  //         comment it out again. In the Step 1 and Step 2 blocks, change
  //         std::make_shared<Vehicle> to std::make_shared<RoboTaxi>.
  // fleet::Vehicle vehicle{13.4};

  // Step 7: no new block. Write the class SuperRoboTaxi above main().
}
