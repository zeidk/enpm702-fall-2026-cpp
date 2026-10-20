/**
 * @file main.cpp
 * @brief L7, Smart Pointers and Move Semantics: the code of every slide,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week7_playground.
 *
 * @code
 * 702build week7_playground
 * 702run week7_playground        # every slide, in order
 * 702run week7_playground 12     # only [Slide 12]
 * @endcode
 *
 * @c "[Slide N]" is the number in the top-left corner of the slide. The code
 * is in one file per section of the deck: ownership.cpp, moves.cpp,
 * unique.cpp, shared.cpp and weak.cpp. Each lists its slides in slides(), and
 * main() joins the lists. run_slides() in @c ../common/slides.cpp sorts them
 * by number and runs them.
 *
 * Code that leaks, or that has undefined behavior, runs only when you ask for
 * its slide by number: a full run skips it. That code is in
 * @c ../undefined/undefined.cpp, built with the sanitizers.
 *
 * The appendix frames are a program of their own: @c ../appendix/, target
 * @c week7_appendix.
 */
#include <vector>

#include "sections.hpp"
#include "slides.hpp"

int main(int argc, char* argv[]) {
  std::vector<Slide> slides;
  for (const std::vector<Slide>& section : {ownership::slides(), moves::slides(),
                                            unique::slides(), shared::slides(),
                                            weak::slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::lecture);
}
