/**
 * @file sensor.cpp
 * @brief The methods of Sensor that are not defined in the class.
 * @author Zeid Kootbally
 */
#include "sensor.hpp"

#include <stdexcept>

Sensor::Sensor(int id, SensorType type, double range)
    : id_{id}, type_{type}, range_{range} {
  if (range_ < 0.0) {
    throw std::invalid_argument{"range must be at least 0"};
  }
}
