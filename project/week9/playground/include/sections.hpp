/**
 * @file sections.hpp
 * @brief The slides of each section file, for main.cpp.
 * @author Zeid Kootbally
 *
 * @details Each section file wraps its code in a namespace named after it, so
 * two files can both define a class with the same name (several sections have
 * their own version of Vehicle) without a clash. Each one lists its slides in
 * its own slides().
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace relationships {
std::vector<Slide> slides();  ///< Section 2, Class Relationships, up to inheritance: relationships.cpp
}

namespace inheritance {
std::vector<Slide> slides();  ///< Section 2, Class Relationships, inheritance: inheritance.cpp
}

namespace polymorphism {
std::vector<Slide> slides();  ///< Section 3, Polymorphism: polymorphism.cpp
}

namespace abstraction {
std::vector<Slide> slides();  ///< Sections 4 and 5, Abstraction and final: abstraction.cpp
}

namespace discussions {
std::vector<Slide> slides();  ///< The code behind the discussion slides: discussions.cpp
}
