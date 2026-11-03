/**
 * @file main.cpp
 * @brief L8, live coding: the program that uses the class Vehicle, finished.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_live.
 *
 * @code
 * 702build week8_live
 * 702run week8_live
 * @endcode
 *
 * main() has one block for each "Live Coding" slide of L8 that prints
 * something. Each block prints "[Step N]", then what that slide adds. There is
 * no [Step 3] block: Step 3 moved the class to ../include/vehicle.hpp and
 * vehicle.cpp, and the output did not change. Sensor is the course's own, in
 * ../../common/include.
 */
#include <iostream>
#include <memory>     // std::unique_ptr, std::make_unique
#include <stdexcept>  // std::invalid_argument
#include <string>

#include "vehicle.hpp"

/**
 * @brief Runs one block for each step of the live coding.
 * @return 0.
 */
int main() {
  // Two objects of one class, each with its own speed.
  {
    std::cout << "[Step 1]\n";
    Vehicle car_a{13.4};
    Vehicle car_b{13.4};
    car_a.set_speed(12.0);
    car_b.set_speed(10.0);
    std::cout << car_a.get_speed() << ' ' << car_b.get_speed() << '\n';
    std::cout << sizeof(Vehicle) << '\n';
  }

  // One object on the stack, one on the heap.
  {
    std::cout << "[Step 2]\n";
    Vehicle car_a{13.4};
    car_a.set_speed(12.0);
    std::cout << car_a.get_speed() << '\n';

    auto car_b{std::make_unique<Vehicle>(13.4)};
    car_b->set_speed(10.0);
    std::cout << car_b->get_speed() << '\n';
  }

  // set_speed() refuses a speed above the maximum.
  {
    std::cout << "[Step 4]\n";
    Vehicle car_a{13.4};
    car_a.set_speed(12.0);
    try {
      car_a.set_speed(20.0);  // above the maximum, 13.4
    } catch (const std::invalid_argument& e) {
      std::cout << "caught: " << e.what() << '\n';
    }
    std::cout << car_a.get_speed() << '\n';
  }

  // The constructors.
  {
    std::cout << "[Step 5]\n";
    Vehicle car_a{13.4};        // white: the default argument
    Vehicle car_b{13.4, "blue"};
    Vehicle car_c{13.4, 12.0};  // the delegating constructor
    std::cout << car_a.get_color() << ' ' << car_b.get_color() << ' '
              << car_c.get_speed() << '\n';
    try {
      Vehicle bad_car{0.0};  // breaks the invariant
      std::cout << "not reached\n";
    } catch (const std::invalid_argument& e) {
      std::cout << "caught: " << e.what() << '\n';
    }
  }

  // A Vehicle that owns its sensors.
  {
    std::cout << "[Step 6]\n";
    Vehicle car_a{13.4};
    car_a.add_sensor(SensorType::lidar, 100.0);
    car_a.add_sensor(SensorType::camera, 80.0);
    std::cout << car_a.sensor_count() << " sensors\n";
    for (const std::unique_ptr<Sensor>& sensor : car_a.get_sensors()) {
      std::cout << sensor->get_id() << ' ' << sensor->get_range() << '\n';
    }
  }
}
