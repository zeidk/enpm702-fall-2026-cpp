/**
 * @file sensor_type.hpp
 * @brief L9, live coding: the enumeration SensorType, given.
 * @author Zeid Kootbally
 *
 * @details A robotaxi carries three kinds of sensors: a lidar, a camera and a
 * radar. The type is a scoped enumeration, not an int, so a number such as 7
 * does not convert to a SensorType on its own. A cast still makes one:
 * static_cast<SensorType>(7) compiles and gives a SensorType that names no
 * enumerator. The underlying type of a scoped enumeration is int, and the
 * enumeration holds every value of it (C++20 [dcl.enum] paragraph 8). Sensor's
 * constructor rejects such a value.
 *
 * With g++ 13.3 on x86-64: sizeof(SensorType) is 4 (its underlying type is
 * int).
 */
#pragma once

namespace fleet {

/// What kind of sensor it is: a scoped enumeration.
enum class SensorType {
  lidar,   ///< light detection and ranging
  camera,  ///< a video camera
  radar,   ///< radio detection and ranging
};

}  // namespace fleet
