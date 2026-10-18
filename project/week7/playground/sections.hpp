/**
 * @file sections.hpp
 * @brief The slides of each section file, for main.cpp.
 * @author Zeid Kootbally
 *
 * @details Each section file wraps its code in a namespace named after it, so
 * two files can both have a slide namespace such as @c creating without a
 * clash. Each one lists its slides in its own slides().
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace ownership {
std::vector<Slide> slides();  ///< Section 1, Ownership: ownership.cpp
}

namespace moves {
std::vector<Slide> slides();  ///< Section 2, Move Semantics: moves.cpp
}

namespace unique {
std::vector<Slide> slides();  ///< Section 3, Smart Pointers, std::unique_ptr: unique.cpp
}

namespace shared {
std::vector<Slide> slides();  ///< Section 3, Smart Pointers, std::shared_ptr: shared.cpp
}

namespace weak {
std::vector<Slide> slides();  ///< Section 3, Smart Pointers, std::weak_ptr: weak.cpp
}
