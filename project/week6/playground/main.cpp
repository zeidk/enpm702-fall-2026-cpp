/**
 * @file main.cpp
 * @brief L6, Functions, Advanced Topics: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_playground.
 *
 * @code
 * 702build week6_playground
 * 702run week6_playground        # every slide, in order
 * 702run week6_playground 40     # only [Slide 40]
 * @endcode
 *
 * @c "[Slide N]" is the number in the top-left corner of the slide. The code
 * is in one file per section of the deck: grouping.cpp, results.cpp,
 * templates.cpp, lambdas.cpp and higher_order.cpp. Each lists its slides in
 * slides(), and main() joins the lists. run_slides() in
 * @c ../common/slides.cpp sorts them by number and runs them.
 *
 * Code that throws, or that has undefined behavior, runs only when you ask for
 * its slide by number: a full run skips it. The code with undefined behavior
 * is in @c ../undefined/undefined.cpp, built with AddressSanitizer.
 *
 * The appendix frames are a program of their own: @c ../appendix/, target
 * @c week6_appendix.
 */
#include <vector>

#include "sections.hpp"
#include "slides.hpp"

int main(int argc, char* argv[]) {
  std::vector<Slide> slides;
  for (const std::vector<Slide>& section : {grouping::slides(), results::slides(), deduced::slides(),
                                            templates::slides(), higher_order::slides(),
                                            lambdas::slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::lecture);
}
