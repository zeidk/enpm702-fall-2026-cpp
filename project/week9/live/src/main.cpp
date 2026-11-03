/**
 * @file main.cpp
 * @brief L9, live coding: the program that uses the classes of the live coding.
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
 * L9 builds on the given Vehicle in six steps. Steps 1 to 3 (Passenger,
 * Fleet's methods, RoboTaxi and Taxi) are given, and we review their code in
 * class. You write Steps 4, 5 and 6 in class, on your own.
 * main() has one block for each step that prints something. Only the Step 1
 * block runs at first: for each next step, uncomment its block, comment out
 * the block of the step before, then build and run. Each block prints
 * "[Step N]", then what that step adds.
 * Each class has its own header in ../include and its own source file here.
 * Every class is in the namespace fleet, so main() writes fleet:: before each
 * name.
 */
#include <iostream>
#include <memory>

#include "fleet.hpp"
#include "passenger.hpp"
#include "position.hpp"
#include "robotaxi.hpp"
#include "route.hpp"
#include "taxi.hpp"

// Step 6: a class that derives from RoboTaxi, here, above main():
//         class SuperRoboTaxi : public fleet::RoboTaxi {};
//         Build, read the error, then delete it.

/**
 * @brief Runs one block for each step of the live coding.
 *
 * Each block prints "[Step N]", then what that step adds.
 * @return 0.
 */
int main() {
  // maya_ride, the 500 m route of the slides: (0, 0) to (300, 400), the long side of a
  // 300, 400, 500 right triangle. Step 4 drives it.
  fleet::Route maya_ride;
  maya_ride.add(fleet::Position{0.0, 0.0});
  maya_ride.add(fleet::Position{300.0, 400.0});

  // Step 1: a Passenger does not keep its Vehicle alive.
  {
    std::cout << "[Step 1]\n";
    auto car_a{std::make_shared<fleet::Vehicle>(13.4)};  // Step 5: RoboTaxi
    fleet::Passenger maya{"Maya"};
    maya.board(car_a);
    std::cout << car_a.use_count() << '\n';
    std::weak_ptr<fleet::Vehicle> watcher{car_a};
    car_a.reset();  // the only owner lets go
    std::cout << std::boolalpha << watcher.expired() << '\n';
    maya.leave();
  }

  // Step 2: a Fleet shares its vehicles.
  // {
  //   std::cout << "[Step 2]\n";
  //   auto car_a{std::make_shared<fleet::Vehicle>(13.4)};  // Step 5: RoboTaxi
  //   auto car_b{std::make_shared<fleet::Vehicle>(13.4)};  // Step 5: RoboTaxi
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

  // Step 4: one loop drives both kinds, each with its own drive(). Then a Taxi
  //         deleted through a Vehicle pointer: the name has 19 characters, so
  //         std::string keeps it on the heap. First add the virtual destructor
  //         in vehicle.hpp, then uncomment the block, build and run: the Taxi
  //         drives 500 m, and AddressSanitizer reports nothing.
  // {
  //   std::cout << "[Step 4]\n";
  //   fleet::Fleet fleet;
  //   fleet.add(std::make_shared<fleet::RoboTaxi>(13.4));
  //   fleet.add(std::make_shared<fleet::Taxi>(13.4, "Ana"));
  //   fleet.drive_all(maya_ride);
  //   std::unique_ptr<fleet::Vehicle> vehicle{std::make_unique<fleet::Taxi>(13.4, "Ana Maria Rodrigues")};
  //   vehicle->drive(maya_ride);
  // }

  // Step 5: no Vehicle object. Uncomment the line, build, read the error, then
  //         comment it out again. In the Step 1 and Step 2 blocks, change
  //         std::make_shared<Vehicle> to std::make_shared<RoboTaxi>.
  // fleet::Vehicle vehicle{13.4};

  // Step 6: no new block. Write the class SuperRoboTaxi above main().
}
