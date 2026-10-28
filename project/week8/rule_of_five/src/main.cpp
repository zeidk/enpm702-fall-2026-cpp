/**
 * @file main.cpp
 * @brief The program of the L8 reading page Rule of Five and Rule of Zero, on
 *        Read the Docs: target week8_rule_of_five.
 * @author Zeid Kootbally
 *
 * @details One source file per part of the page. Each part has a key, the
 * name of its file and of its namespace:
 * @code
 * 702build week8_rule_of_five
 * 702run week8_rule_of_five          # every part that is safe to run
 * 702run week8_rule_of_five five     # only the part five
 * @endcode
 * Every file is compiled with AddressSanitizer and UndefinedBehaviorSanitizer
 * (see ../../CMakeLists.txt). The parts that leak or delete memory twice stop
 * with a report; a full run skips them, and each runs only when asked for by
 * its key.
 */
#include <vector>

#include "parts.hpp"
#include "slides.hpp"

int main(int argc, char* argv[]) {
  // One entry per part, in the order of the page: 0 (it has no slide number),
  // its title, the function that runs it, whether it runs only on its own
  // (true: it leaks or deletes memory twice), and its key.
  const std::vector<Slide> parts{
      {0, "Which Function Runs", which_one::run, false, "which_one"},
      {0, "What the Compiler Writes Now", zero::run, false, "zero"},
      {0, "A Class That Owns Memory", raw_owner::run, false, "raw_owner"},
      {0, "Without the Destructor", no_destructor::run, true, "no_destructor"},
      {0, "Copying an Owner", double_free::run, true, "double_free"},
      {0, "All Five, Run", five::run, false, "five"},
      {0, "Why the Moves Are noexcept", move_noexcept::run, false, "move_noexcept"},
      {0, "Without the Moves", no_move::run, true, "no_move"},
      {0, "Discussion: Who Deletes What?", move_bug::run, true, "move_bug"},
      {0, "= default and = delete", move_only::run, false, "move_only"},
      {0, "Beyond Memory: A File", file_owner::run, false, "file_owner"},
  };
  return run_slides(parts, argc, argv, Part::reading);
}
