/**
 * @file vehicle.hpp
 * @brief L8, live coding: the class Vehicle, which we write in class, step by step.
 * @author Zeid Kootbally
 *
 * @details A Vehicle holds its maximum speed, its speed, its color and its
 * mileage (the distance it has driven). Its invariant: the speed stays from 0 to
 * the maximum speed. The "Live Coding" slides of L8 fill this file in, one step
 * at a time. Each "Step N" comment below says what that step adds here.
 *
 * Speeds are in meters per second. 13.4 m/s is about 30 mph
 * (30 * 0.44704 = 13.41), a value chosen for the examples.
 */
// Step 2: #pragma once (L5) goes here, before the includes.

#include <cstddef>  // Step 5: std::size_t
#include <memory>   // Step 5: std::unique_ptr
#include <string>
#include <vector>   // Step 5: std::vector

#include "sensor.hpp"  // Step 5: Sensor and SensorType, in ../../common/include

// Step 2: the class Vehicle of main.cpp, with only the declarations of its
//         methods set_speed() and get_speed(). Their bodies go to vehicle.cpp.
// Step 3: get_speed() moves into the class, body and all, as a one-line
//         accessor that is [[nodiscard]], const and noexcept.
// Step 4: the constructors explicit Vehicle(double max_speed,
//         const std::string& color = "white") and Vehicle(double max_speed,
//         double speed), and the accessor get_color(). The data members
//         max_speed_ and color_ lose their { } values: the constructor sets them.
// Step 5: the data member sensors_, a std::vector<std::unique_ptr<Sensor>>, and
//         the methods add_sensor(), sensor_count() and get_sensors().
