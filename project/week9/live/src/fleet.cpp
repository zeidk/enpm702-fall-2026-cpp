/**
 * @file fleet.cpp
 * @brief L9, live coding: the methods of the classes of fleet.hpp. The
 *        constructors are defined in the classes, in fleet.hpp.
 * @author Zeid Kootbally
 *
 * @details Vehicle's methods are L8's. Each "Step N" comment below says what
 * that step adds or changes here. We write Step 1 together; you write Steps 2
 * to 7 on your own.
 */
#include "fleet.hpp"

#include <algorithm>  // Step 2: std::find
#include <iostream>   // Step 4: drive() prints
#include <stdexcept>
#include <utility>    // Step 2: std::move

namespace fleet {

// L8.
void Vehicle::set_speed(double speed) {
  if (speed < 0.0 || speed > max_speed_) {
    throw std::invalid_argument{"speed must be from 0 to the maximum speed"};
  }
  speed_ = speed;
}

// L8.
double Vehicle::get_speed() const noexcept { return speed_; }

// L8.
const std::string& Vehicle::get_color() const noexcept { return color_; }

// L8.
int Vehicle::add_sensor(SensorType type, double range) {
  const int id{static_cast<int>(sensors_.size()) + 1};
  sensors_.push_back(std::make_unique<Sensor>(id, type, range));
  return id;
}

// L8.
std::size_t Vehicle::sensor_count() const noexcept { return sensors_.size(); }

// L8.
const std::vector<std::unique_ptr<Sensor>>& Vehicle::get_sensors() const noexcept {
  return sensors_;
}

// L8.
int Vehicle::get_id() const noexcept { return id_; }

// L8. The definition does not repeat static.
int Vehicle::next_id() noexcept { return next_id_; }

// Step 4: Vehicle::drive() prints "Vehicle: drives ", the route's length and " m".
// Step 6: Vehicle::drive() is deleted: a pure virtual method has no body.

// Step 1: Passenger::board() stores the vehicle in ride_. Passenger::leave()
//         resets ride_, and is noexcept.

// Step 2: Fleet::add() throws std::invalid_argument for a null pointer, and
//         adds the vehicle only if std::find does not find it in vehicles_.
//         Fleet::remove() erases the vehicle with that id (std::erase_if) and
//         returns true if it removed one. Fleet::size() returns how many
//         vehicles the fleet holds.

// Step 4: Fleet::drive_all() calls drive() on every vehicle, in one loop.
// Step 4: RoboTaxi::drive() prints "RoboTaxi: drives itself ", the route's
//         length and " m". Taxi::drive() prints "Taxi: ", the driver's name,
//         " drives ", the route's length and " m".

}  // namespace fleet
