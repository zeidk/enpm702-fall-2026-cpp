/**
 * @file slides.hpp
 * @brief The list of slides the playground can run, and the function that runs it.
 * @author Zeid Kootbally
 *
 * @details week9_playground builds a list of Slide entries, one per frame that
 * has code, in the main deck and in the appendix, and hands it to
 * run_slides(). Slide is a struct (Lecture 6), and @c run is a pointer
 * to a function (Lecture 6, appendix).
 */
#pragma once

#include <string_view>
#include <vector>

/// Which part of the deck a frame is in.
enum class Part {
  lecture,   ///< numbered 1, 2, 3, ...: "[Slide 11]"
  appendix,  ///< numbered i, ii, iii, ...: "[Appendix iv]"
};

/// One frame of the deck that has code.
struct Slide {
  int number;              ///< The number in the frame's top-left corner.
  std::string_view title;  ///< The frame's title.
  void (*run)();           ///< Runs the frame's code.
  /// True when the code has undefined behavior or throws: a full run skips
  /// it, and it runs only when asked for by its number.
  bool run_alone{false};
  Part part{Part::lecture};  ///< The main deck or the appendix.
};

/**
 * @brief Runs every slide, or the one named on the command line.
 *
 * "12" names [Slide 12]; "iv" (or "IV") names [Appendix iv].
 * @param slides The frames with code, in any order.
 * @return 0, or 1 when the argument names no frame with code.
 */
int run_slides(std::vector<Slide> slides, int argc, char* argv[]);
