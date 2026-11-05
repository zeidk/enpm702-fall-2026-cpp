/**
 * @file main.cpp
 * @brief L9, OOP Advanced: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week9_playground.
 *
 * @code
 * 702build week9_playground
 * 702run week9_playground        # every slide, then every appendix frame
 * 702run week9_playground 75     # only [Slide 75]
 * 702run week9_playground xl     # only [Appendix xl]
 * @endcode
 *
 * @c "[Slide N]" is the number in the top-left corner of the slide. The code
 * is in one file per section of the deck, and one for the appendix. Each lists
 * its frames in slides(), and main() joins the lists. run_slides(), in
 * slides.cpp, sorts them by number and runs them.
 */
#include <vector>

#include "sections.hpp"
#include "slides.hpp"
#include "undefined.hpp"

int main(int argc, char* argv[]) {
  std::vector<Slide> slides;
  for (const std::vector<Slide>& section :
       {relationships::slides(), inheritance::slides(), polymorphism::slides(),
        abstraction::slides(), undefined::slides(), appendix::slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv);
}
