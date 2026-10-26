/**
 * @file main.cpp
 * @brief L8, OOP Basics: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_playground.
 *
 * @code
 * 702build week8_playground
 * 702run week8_playground        # every slide, in order
 * 702run week8_playground 11     # only [Slide 11]
 * @endcode
 *
 * @c "[Slide N]" is the number in the top-left corner of the slide. The code
 * is in one file per section of the deck. Each lists its slides in slides(),
 * and main() joins the lists. run_slides() in @c ../../common/src/slides.cpp
 * sorts them by number and runs them.
 */
#include <vector>

#include "sections.hpp"
#include "slides.hpp"

int main(int argc, char* argv[]) {
  std::vector<Slide> slides;
  for (const std::vector<Slide>& section : {classes::slides(), struct_or_class::slides(),
                                            encapsulation::slides(), accessors::slides(),
                                            constructors::slides(), const_and_this::slides(),
                                            destructors::slides(), static_members::slides(),
                                            discussions::slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::lecture);
}
