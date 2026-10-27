/**
 * @file fleet.cpp
 * @brief L9, live coding: the methods of fleet.hpp that are not defined in a class.
 * @author Zeid Kootbally
 *
 * @details Vehicle's methods are L8's. Each "Step N" comment below says what
 * that step adds or changes here.
 */
#include "fleet.hpp"

#include <algorithm>  // Step 2: std::find
#include <iostream>   // Step 4: drive() prints
#include <stdexcept>
#include <utility>    // Step 1: std::move

// L8: the member initializer list sets id_, max_speed_ and color_, then the body
// checks the maximum speed.
Vehicle::Vehicle(double max_speed, const std::string& color)
    : id_{next_id_++}, max_speed_{max_speed}, color_{color} {
  if (max_speed_ <= 0.0) {
    throw std::invalid_argument{"maximum speed must be greater than 0"};
  }
}

// L8: the delegating constructor.
Vehicle::Vehicle(double max_speed, double speed) : Vehicle{max_speed} {
  set_speed(speed);
}

// L8.
void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

// L8.
int Vehicle::add_sensor(SensorType type, double range) {
  const int id{static_cast<int>(sensors_.size()) + 1};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

// Step 4: Vehicle::drive() prints "Vehicle: drives ", the route's length and " m".
// Step 6: Vehicle::drive() is deleted: a pure virtual method has no body.

// Step 1: Passenger's constructor moves the name into name_, then throws
//         std::invalid_argument if name_ is empty.

// Step 2: Fleet::add() throws std::invalid_argument for a null pointer, and
//         adds the vehicle only if std::find does not find it in vehicles_.
//         Fleet::remove() erases the vehicle with that id (std::erase_if) and
//         returns true if it removed one.

// Step 4: Fleet::drive_all() calls drive() on every vehicle, in one loop.
// Step 4: RoboTaxi::drive() prints "RoboTaxi: drives itself ", the route's
//         length and " m". Taxi::drive() prints "Taxi: ", the driver's name,
//         " drives ", the route's length and " m".
