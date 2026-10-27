/**
 * @file vehicle.cpp
 * @brief L8, live coding: the methods of Vehicle that are not defined in the class.
 * @author Zeid Kootbally
 *
 * @details Each "Step N" comment below says what that step adds here.
 */
#include "vehicle.hpp"

#include <stdexcept>  // Step 3: std::invalid_argument

// Step 2: the bodies of set_speed() and get_speed(), each named with Vehicle::.
// Step 3: get_speed() leaves this file: its body moves into the class.
//         set_speed() throws std::invalid_argument when the speed is below 0
//         or above max_speed_, and changes speed_ only after that check.
// Step 4: the two constructors. The first sets max_speed_ and color_ in its
//         member initializer list, then throws std::invalid_argument when
//         max_speed_ is not greater than 0. The second delegates to the first,
//         then calls set_speed().
// Step 5: add_sensor() creates a Sensor with the next id (1, 2, ...), stores it
//         in sensors_, and returns its id.
