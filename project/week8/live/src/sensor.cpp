/**
 * @file sensor.cpp
 * @brief L8, live coding: the methods of Sensor, given. Its constructor is
 *        defined in the class, in sensor.hpp.
 * @author Zeid Kootbally
 */
#include "sensor.hpp"

int Sensor::get_id() const noexcept { return id_; }

SensorType Sensor::get_type() const noexcept { return type_; }

double Sensor::get_range() const noexcept { return range_; }
