/**
 * @file vehicle.cpp
 * @brief L8, live coding: the methods of Vehicle. Its constructors are defined in
 *        the class, in vehicle.hpp.
 * @author Zeid Kootbally
 *
 * @details Each "Step N" comment below says what that step adds here.
 */
#include "vehicle.hpp"

#include <stdexcept>  // Step 4: std::invalid_argument

// Step 3: the bodies of set_speed() and get_speed(), each named with Vehicle::.
// Step 4: get_speed() gets const and noexcept here too, not [[nodiscard]].
//         set_speed() throws std::invalid_argument when the speed is below 0
//         or above max_speed_, and changes speed_ only after that check.
// Step 5: the body of get_color(). The constructors go in vehicle.hpp.
// Step 6: add_sensor() creates a Sensor with the next id (1, 2, ...), stores it
//         in sensors_, and returns its id. sensor_count() returns
//         sensors_.size(), and get_sensors() returns sensors_ by const&.
