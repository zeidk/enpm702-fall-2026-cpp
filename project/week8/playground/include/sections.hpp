/**
 * @file sections.hpp
 * @brief The slides of each section file, for main.cpp and appendix.cpp.
 * @author Zeid Kootbally
 *
 * @details Each section file wraps its code in a namespace named after it, so
 * two files can both have a slide namespace with the same name without a
 * clash. Each one lists the frames of the lecture in its slides(), which
 * week8_playground runs, and the appendix frames of its section in its
 * appendix_slides(), which week8_appendix runs. A section with no frame of
 * one kind has no function for it.
 */
#pragma once

#include <vector>

#include "slides.hpp"

namespace classes {
std::vector<Slide> slides();           ///< Classes and Objects: classes.cpp
std::vector<Slide> appendix_slides();  ///< its appendix frame: State and Behavior
}  // namespace classes

namespace struct_or_class {
std::vector<Slide> slides();  ///< Struct or Class: struct_or_class.cpp
}

namespace encapsulation {
std::vector<Slide> slides();           ///< Encapsulation: encapsulation.cpp
std::vector<Slide> appendix_slides();  ///< its appendix frames: Header and Source
}  // namespace encapsulation

namespace accessors {
std::vector<Slide> slides();           ///< Accessors and Mutators: accessors.cpp
std::vector<Slide> appendix_slides();  ///< its appendix frames
}  // namespace accessors

namespace constructors {
std::vector<Slide> slides();           ///< Constructors: constructors.cpp
std::vector<Slide> appendix_slides();  ///< its appendix frames
}  // namespace constructors

namespace const_and_this {
std::vector<Slide> slides();           ///< The this Pointer: const_and_this.cpp
std::vector<Slide> appendix_slides();  ///< const Objects, in the appendix
}  // namespace const_and_this

namespace destructors {
std::vector<Slide> slides();  ///< Destructors: destructors.cpp
}  // namespace destructors

namespace static_members {
std::vector<Slide> appendix_slides();  ///< static Members, in the appendix: static_members.cpp
}

namespace discussions {
std::vector<Slide> slides();           ///< The code behind the discussion slides: discussions.cpp
std::vector<Slide> appendix_slides();  ///< The appendix discussion: Will It Compile?
}  // namespace discussions
