/**
 * @file slides.hpp
 * @brief The list of slides a program can run, and the function that runs it.
 * @author Zeid Kootbally
 *
 * @details week6_playground and week6_appendix both use it: each builds a
 * list of Slide entries, one per frame that has code, and hands it to
 * run_slides(). Slide is a struct (Grouping Values), and @c run is a pointer
 * to a function (appendix, Function Pointers).
 */
#pragma once

#include <string_view>
#include <vector>

/// One frame of the deck that has code.
struct Slide {
  int number;              ///< The number in the frame's top-left corner.
  std::string_view title;  ///< The frame's title.
  void (*run)();           ///< Runs the frame's code.
  /// True when the code has undefined behavior or throws: a full run skips
  /// it, and it runs only when asked for by its number.
  bool run_alone{false};
};

/// Which part of the deck a list of slides comes from.
enum class Part {
  lecture,   ///< numbered 1, 2, 3, ...: "[Slide 12]"
  appendix,  ///< numbered i, ii, iii, ...: "[Appendix iv]"
};

/**
 * @brief Runs the code of every slide in the list, or of the one slide asked
 *        for on the command line.
 *
 * With no argument, runs every slide in order of its number, except those
 * marked run_alone. With one argument, runs only that slide: a number such as
 * @c 12 for the lecture, a number or a roman numeral such as @c iv for the
 * appendix.
 *
 * @param slides The slides with code, in any order.
 * @param argc The argument count from main().
 * @param argv The arguments from main().
 * @param part Whether the slides are lecture frames or appendix frames.
 * @return 0 on success, 1 when the argument is not a slide of the list.
 */
int run_slides(std::vector<Slide> slides, int argc, char* argv[], Part part);
