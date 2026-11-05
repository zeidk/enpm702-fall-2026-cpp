/**
 * @file compare.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        operator< and operator> for Route.
 * @author Zeid Kootbally
 *
 * @details Compares two routes by length, then sorts three with std::sort,
 * which calls Route::operator<. The lengths are chosen for the example:
 * 500 m, 1000 m and 700 m.
 * Run it: 702run week9_operators compare
 */
#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

#include "parts.hpp"
#include "route.hpp"

namespace compare {
namespace {
void print_lengths(std::string_view label, const std::vector<fleet::Route>& rides) {
  std::cout << label;
  for (const fleet::Route& ride : rides) {
    std::cout << ' ' << ride.length();
  }
  std::cout << '\n';
}
}  // namespace

void run() {
  fleet::Route maya_ride;  // 500 m
  maya_ride.add(fleet::Position{0.0, 0.0});
  maya_ride.add(fleet::Position{300.0, 400.0});
  fleet::Route airport_ride;  // 1000 m
  airport_ride.add(fleet::Position{0.0, 0.0});
  airport_ride.add(fleet::Position{600.0, 800.0});
  fleet::Route campus_ride;  // 300 m north, then 400 m east: 700 m
  campus_ride.add(fleet::Position{0.0, 0.0});
  campus_ride.add(fleet::Position{0.0, 300.0});
  campus_ride.add(fleet::Position{400.0, 300.0});

  std::cout << std::boolalpha;
  std::cout << "maya_ride < airport_ride: " << (maya_ride < airport_ride) << '\n';
  std::cout << "maya_ride > airport_ride: " << (maya_ride > airport_ride) << '\n';
  std::cout << "maya_ride.operator<(airport_ride): "
            << maya_ride.operator<(airport_ride) << '\n';

  std::vector<fleet::Route> rides{airport_ride, maya_ride, campus_ride};
  print_lengths("before std::sort:", rides);
  std::sort(rides.begin(), rides.end());  // calls Route::operator<
  print_lengths("after std::sort: ", rides);
}
}  // namespace compare
