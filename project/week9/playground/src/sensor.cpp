/**
 * @file sensor.cpp
 * @brief The methods of Sensor. Its constructor is defined in the class, in
 *        sensor.hpp.
 * @author Zeid Kootbally
 */
#include "sensor.hpp"

namespace fleet {

int Sensor::get_id() const noexcept { return id_; }

SensorType Sensor::get_type() const noexcept { return type_; }

double Sensor::get_range() const noexcept { return range_; }

}  // namespace fleet
