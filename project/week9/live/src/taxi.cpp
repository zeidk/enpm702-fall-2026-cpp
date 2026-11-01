/**
 * @file taxi.cpp
 * @brief L9, live coding: the methods of Taxi (Steps 3 and 4).
 * @author Zeid Kootbally
 */
#include "taxi.hpp"

#include <iostream>  // Step 4: drive() prints
#include <utility>   // Step 3: std::move

namespace fleet {

// Step 3: assign_driver() moves its parameter into driver_name_ (std::move).

// Step 4: define Taxi::drive(const Route& route) const. Its body prints one
//         line to std::cout, in one of two cases:
//         1. If driver_name_ is empty, it prints
//            "Taxi: no driver, stays parked" and a new line, then returns.
//         2. Otherwise it prints "Taxi: ", then driver_name_, then " drives ",
//            then route.length(), then " m" and a new line. For Ana and the
//            500 m route it prints "Taxi: Ana drives 500 m".
//         The definition does not repeat override: only the declaration in
//         taxi.hpp has it.

}  // namespace fleet
