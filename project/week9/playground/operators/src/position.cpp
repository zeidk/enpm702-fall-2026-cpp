/**
 * @file position.cpp
 * @brief Operator Overloading, the L9 reading on Read the Docs: the
 *        operator<< of Position.
 * @author Zeid Kootbally
 */
#include "position.hpp"

namespace fleet {

std::ostream& operator<<(std::ostream& out, const Position& p) {
  out << '(' << p.x << ", " << p.y << ')';
  return out;
}

}  // namespace fleet
