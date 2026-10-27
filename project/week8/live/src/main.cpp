/**
 * @file main.cpp
 * @brief L8, live coding: the program that uses the class Vehicle we write in class.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_live.
 *
 * @code
 * 702build week8_live
 * 702run week8_live
 * @endcode
 *
 * The six "Live Coding" slides of L8 build one class, Vehicle, in six steps. We
 * write Step 1 together; you write Steps 2 to 6 on your own.
 * main() has one commented block for each step that prints something: when we
 * finish a step, we uncomment its block, then build and run. Each block prints
 * "[Step N]", then what that step adds. From Step 3 on, the class is in
 * ../include/vehicle.hpp and vehicle.cpp. Sensor is the course's own, in
 * ../../common/include.
 */
#include <iostream>
#include <memory>     // Step 6: std::unique_ptr
#include <stdexcept>  // Step 4: std::invalid_argument
#include <string>

#include "vehicle.hpp"

// Step 1: the class Vehicle goes here, above main(). Its private data members
//         are max_speed_{13.4}, speed_{0.0}, color_{"white"} and mileage_{0.0}.
//         Its public methods are set_speed() and get_speed(), with their bodies
//         in the class. set_speed() checks nothing yet.
// Step 3: the class moves to vehicle.hpp, and the bodies of its methods to
//         vehicle.cpp.

int main() {
  // Step 1: two objects of one class, each with its own speed.
  // {
  //   std::cout << "[Step 1]\n";
  //   Vehicle car_a;  // Step 5: Vehicle car_a{13.4};
  //   Vehicle car_b;  // Step 5: Vehicle car_b{13.4};
  //   car_a.set_speed(12.0);
  //   car_b.set_speed(10.0);
  //   std::cout << car_a.get_speed() << ' ' << car_b.get_speed() << '\n';
  //   std::cout << sizeof(Vehicle) << '\n';
  // }

  // Step 2: write this block yourself. Make car_a on the
  // stack and car_b on the heap with std::make_unique<Vehicle>(), set their
  // speeds to 12 and 10, and print each speed: "." for car_a, "->" for car_b.
  // It prints [Step 2], then 12, then 10.
  // {
  //   std::cout << "[Step 2]\n";
  //
  // }

  // Step 3: no new block. The class moves to vehicle.hpp and vehicle.cpp, and
  //         the output does not change.

  // Step 4: set_speed() refuses a speed above the maximum.
  // {
  //   std::cout << "[Step 4]\n";
  //   Vehicle car_a;  // Step 5: Vehicle car_a{13.4};
  //   car_a.set_speed(12.0);
  //   try {
  //     car_a.set_speed(20.0);  // above the maximum, 13.4
  //   } catch (const std::invalid_argument& e) {
  //     std::cout << "caught: " << e.what() << '\n';
  //   }
  //   std::cout << car_a.get_speed() << '\n';
  // }

  // Step 5: the constructors.
  // {
  //   std::cout << "[Step 5]\n";
  //   Vehicle car_a{13.4};          // white: the default argument
  //   Vehicle car_b{13.4, "blue"};
  //   Vehicle car_c{13.4, 12.0};    // the delegating constructor
  //   std::cout << car_a.get_color() << ' ' << car_b.get_color() << ' '
  //             << car_c.get_speed() << '\n';
  //   try {
  //     Vehicle bad_car{0.0};  // breaks the invariant
  //     std::cout << "not reached\n";
  //   } catch (const std::invalid_argument& e) {
  //     std::cout << "caught: " << e.what() << '\n';
  //   }
  // }

  // Step 6: a Vehicle that owns its sensors.
  // {
  //   std::cout << "[Step 6]\n";
  //   Vehicle car_a{13.4};
  //   car_a.add_sensor(SensorType::lidar, 100.0);
  //   car_a.add_sensor(SensorType::camera, 80.0);
  //   std::cout << car_a.sensor_count() << " sensors\n";
  //   for (const std::unique_ptr<Sensor>& sensor : car_a.get_sensors()) {
  //     std::cout << sensor->get_id() << ' ' << sensor->get_range() << '\n';
  //   }
  // }
}
