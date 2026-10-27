/**
 * @file speed.hpp
 * @brief [Appendix ii] One Definition in Many Files: a function defined in a
 *        header.
 * @author Zeid Kootbally
 *
 * @details appendix.cpp and speed_user.cpp both include this header, so each
 * of them holds a definition of kmh(): two definitions in one program. inline
 * makes that legal (C++20 [basic.def.odr], 6.3 paragraph 13). Remove inline
 * and the program does not link; appendix.cpp shows the error.
 */
#pragma once

/// Converts a speed from m/s to km/h.
inline double kmh(double mps) {
  return mps * 3.6;
}
