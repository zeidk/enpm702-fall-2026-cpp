/**
 * @file a.cpp
 * @brief The method of A (L9, the cyclic include).
 * @author Zeid Kootbally
 *
 * @details add stores the address of a B and calls none of its methods, so
 * the name from the forward declaration is enough: this file does not include
 * b.hpp.
 */
// [Slide 27] A Forward Declaration Breaks the Cycle
#include "a.hpp"
void A::add(const B& b) { bs_.push_back(&b); }
