/**
 * @file no_const.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs:
 *        Mistake: A Comparison Without const.
 * @author Zeid Kootbally
 *
 * @details A Route cut down to its length, whose operator< has no const at
 * the end. It works on two Route objects that are not const. The function
 * is_shorter, which takes them by const reference, is commented out: it does
 * not compile.
 * Run it: 702run week9_operators no_const
 */
#include <iostream>

#include "parts.hpp"

namespace no_const {
class Route {  // only its length, in m
 public:
  explicit Route(double length) : length_{length} {}
  bool operator<(const Route& other);  // no const at the end: wrong

 private:
  double length_;
};

bool Route::operator<(const Route& other) { return length_ < other.length_; }

// bool is_shorter(const Route& a, const Route& b) {
//   return a < b;  // does not compile: a is const
// }

void run() {
  Route maya_ride{500.0};
  Route airport_ride{1000.0};
  std::cout << std::boolalpha << (maya_ride < airport_ride) << '\n';
}
}  // namespace no_const
