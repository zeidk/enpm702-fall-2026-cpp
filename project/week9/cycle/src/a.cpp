/**
 * @file a.cpp
 * @brief A's method. It stores a pointer and calls nothing on B, so it needs
 *        only a.hpp.
 * @author Zeid Kootbally
 */
// [Appendix xxxviii] A Forward Declaration Breaks the Cycle
#include "a.hpp"

void A::set_b(B* b) { b_ = b; }
