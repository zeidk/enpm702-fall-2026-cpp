/**
 * @file robotaxi.hpp
 * @brief RoboTaxi: a Vehicle that drives itself (L9).
 * @author Zeid Kootbally
 *
 * @details A RoboTaxi is a Vehicle (inheritance). Its own drive() checks the
 * sensors first: a robotaxi with no sensor does not move (the requirement FR-4
 * asks for one or more). It adds no data member: the sensors are Vehicle's.
 *
 * RoboTaxi is final: no class derives from it.
 */
#pragma once

#include "route.hpp"
#include "vehicle.hpp"

/// A taxi that drives itself with its sensors.
class RoboTaxi final : public Vehicle {
 public:
  /// Starts parked, with no sensor. @param max_speed In m/s, passed to Vehicle.
  explicit RoboTaxi(double max_speed) : Vehicle{max_speed} {}

  /// Drives the route, or stays parked with no sensor.
  void drive(const Route& route) const override;
};
