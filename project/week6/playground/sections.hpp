/**
 * @file sections.hpp
 * @brief The slides of each section file, for main.cpp.
 * @author Zeid Kootbally
 *
 * @details Each section file wraps its code in a namespace named after it, so
 * two files can both have a slide namespace such as @c by_value_reference
 * without a clash. Each one lists its slides in its own slides().
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace grouping {
std::vector<Slide> slides();  ///< Section 1, Grouping Values: grouping.cpp
}

namespace results {
std::vector<Slide> slides();  ///< Section 2, Multiple and Optional Results: results.cpp
}

namespace deduced {
std::vector<Slide> slides();  ///< Section 3, Deduced Return Types: deduced.cpp
}

namespace templates {
std::vector<Slide> slides();  ///< Section 4, Function Templates: templates.cpp
}

namespace higher_order {
std::vector<Slide> slides();  ///< Sections 5 and 7, Higher-Order Functions and Storing and Adapting Callables: higher_order.cpp
}

namespace lambdas {
std::vector<Slide> slides();  ///< Section 6, Lambdas: lambdas.cpp
}
