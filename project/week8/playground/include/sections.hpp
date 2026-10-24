/**
 * @file sections.hpp
 * @brief The slides of each section file, for main.cpp.
 * @author Zeid Kootbally
 *
 * @details Each section file wraps its code in a namespace named after it, so
 * two files can both have a slide namespace with the same name without a
 * clash. Each one lists its slides in its own slides().
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace classes {
std::vector<Slide> slides();  ///< Section 1, Classes and Objects: classes.cpp
}

namespace struct_or_class {
std::vector<Slide> slides();  ///< Section 2, Struct or Class: struct_or_class.cpp
}

namespace encapsulation {
std::vector<Slide> slides();  ///< Section 3, Encapsulation: encapsulation.cpp
}

namespace accessors {
std::vector<Slide> slides();  ///< Section 4, Accessors and Mutators: accessors.cpp
}

namespace constructors {
std::vector<Slide> slides();  ///< Section 5, Constructors: constructors.cpp
}

namespace const_and_this {
std::vector<Slide> slides();  ///< Sections 6 and 7, const Objects and The this Pointer: const_and_this.cpp
}

namespace destructors {
std::vector<Slide> slides();  ///< Section 8, Destructors: destructors.cpp
}

namespace static_members {
std::vector<Slide> slides();  ///< Section 9, static Members: static_members.cpp
}

namespace discussions {
std::vector<Slide> slides();  ///< The code behind the discussion slides: discussions.cpp
}
