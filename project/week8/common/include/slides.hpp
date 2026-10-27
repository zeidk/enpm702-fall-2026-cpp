/**
 * @file slides.hpp
 * @brief The list of slides a program can run, and the function that runs it.
 * @author Zeid Kootbally
 *
 * @details week8_playground and week8_appendix use it: each builds a list of
 * Slide entries, one per frame that has code, and hands it to run_slides().
 * Slide is a struct (Lecture 6), and @c run is a pointer to a function
 * (Lecture 6, appendix).
 *
 * A few entries are not frames of the deck: they hold the code of a reading
 * page on Read the Docs, such as Rule of Five and Rule of Zero. Each of those
 * has a name, its @c key, instead of a number: @c 702run @c week8_appendix
 * @c five.
 */
#pragma once

#include <string_view>
#include <vector>

/// One frame of the deck that has code, or one part of a reading page.
struct Slide {
  int number;              ///< The number in the frame's top-left corner; 0 for a reading part.
  std::string_view title;  ///< The frame's title.
  void (*run)();           ///< Runs the frame's code.
  /// True when the code stops the program, leaks or has undefined behavior:
  /// a full run skips it, and it runs only when asked for by its number.
  bool run_alone{false};
  /// The name of a reading part, such as "five", typed instead of a number.
  /// Empty for a frame of the deck.
  std::string_view key{};
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
 * With no argument, runs every slide in order of its number, then the reading
 * parts in the order of the list, except those marked run_alone. With one
 * argument, runs only that slide: a number such as @c 12 for the lecture, a
 * number or a roman numeral such as @c iv for the appendix, or the key of a
 * reading part, such as @c five.
 *
 * @param slides The slides with code, in any order.
 * @param argc The argument count from main().
 * @param argv The arguments from main().
 * @param part Whether the slides are lecture frames or appendix frames.
 * @return 0 on success, 1 when the argument is not a slide of the list.
 */
int run_slides(std::vector<Slide> slides, int argc, char* argv[], Part part);

/**
 * @brief Says where the code of a slide is, when that code does not compile
 *        or makes the compiler warn.
 *
 * Such code stays commented out, so the programs build with no warning. The
 * run of its slide calls this function instead.
 *
 * @param file The file that holds the commented-out code, such as
 *             "playground/src/constructors.cpp".
 */
void print_commented_out(std::string_view file);
