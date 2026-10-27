/**
 * @file main.cpp
 * @brief L9, OOP Advanced: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_playground.
 *
 * @code
 * 702build week9_playground
 * 702run week9_playground        # every slide, in order
 * 702run week9_playground 55     # only [Slide 55]
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
#include "undefined.hpp"

int main(int argc, char* argv[]) {
  std::vector<Slide> slides;
  for (const std::vector<Slide>& section :
       {relationships::slides(), inheritance::slides(), polymorphism::slides(),
        abstraction::slides(), discussions::slides(), undefined::slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::lecture);
}
