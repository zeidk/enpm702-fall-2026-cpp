/**
 * @file speed_user.cpp
 * @brief [Appendix ii] One Definition in Many Files: the slide's a.cpp.
 * @author Zeid Kootbally
 *
 * @details A second source file that includes speed.hpp, so the program
 * holds a second definition of kmh(). appendix.cpp holds the first one and
 * calls a().
 */
#include "speed.hpp"

double a() { return kmh(10.0); }
