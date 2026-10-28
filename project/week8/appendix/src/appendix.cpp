/**
 * @file appendix.cpp
 * @brief L8, OOP Basics: the code of every appendix frame that has code,
 *        runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_appendix.
 *
 * @code
 * 702build week8_appendix
 * 702run week8_appendix         # every appendix frame
 * 702run week8_appendix ii      # only [Appendix ii]; 702run week8_appendix 2 works too
 * @endcode
 *
 * The appendix frames are numbered i, ii, iii, ... in their top-left corner,
 * and @c "[Appendix ii]" is that number. Most appendix frames belong to a
 * section of the lecture, so their code is in that section's file in
 * ../../playground/src/, listed in its appendix_slides(): State and Behavior
 * (classes.cpp), Header and Source (encapsulation.cpp), The Compiler Checks
 * const and A Broken Rule Is Caught (accessors.cpp), A Constructor That Throws
 * and The Class So Far (constructors.cpp), const Objects (const_and_this.cpp and discussions.cpp) and static Members
 * (static_members.cpp). This file holds One Definition in Many Files.
 *
 * The code of the reading page Rule of Five and Rule of Zero, on Read the
 * Docs, is its own program: ../../rule_of_five/, target week8_rule_of_five.
 */
#include <iostream>
#include <vector>

#include "sections.hpp"
#include "slides.hpp"
#include "speed.hpp"

double a();  // defined in speed_user.cpp

// [Appendix ii] One Definition in Many Files: kmh() is defined in
// ../include/speed.hpp, which this file and speed_user.cpp (the slide's a.cpp)
// both include, so the program holds two definitions of kmh(). They are the
// same, and kmh() is inline, so the program links. This run() is the slide's
// main(). Remove inline in speed.hpp and the program does not link (g++ 13.3,
// paths cut to ...):
// /usr/bin/ld: CMakeFiles/week8_appendix.dir/appendix/src/speed_user.cpp.o: in function `kmh(double)':
// .../appendix/src/speed_user.cpp:15: multiple definition of `kmh(double)';
//   CMakeFiles/week8_appendix.dir/appendix/src/appendix.cpp.o:.../appendix/include/speed.hpp:15: first defined here
// collect2: error: ld returned 1 exit status
// (the slide builds a.cpp and main.cpp without -g, so its error names them
// and shows (.text+0x0) instead of a line)
namespace one_definition {
void run() {
  std::cout << kmh(12.0) << ' '
            << a() << '\n';  // 43.2 36
}
}  // namespace one_definition

// Appendix frames xii, A Method Defined in Its Class, and xiii, inline Does
// Not Force Inlining, have no code to run. They compare the
// assembly g++ 13.3 writes for a call of the in-class get_speed() of Short
// Methods in the Class (lecture slide 42): with -O2 the call becomes one read
// of speed_, with -O0 it stays a call. With the body in another source file,
// the call stays a call at -O2.

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (11 is xi), its
  // title, and the function that runs it.
  std::vector<Slide> slides{
      {2, "One Definition in Many Files", one_definition::run},
  };
  // The frames of the other files.
  for (const std::vector<Slide>& section :
       {classes::appendix_slides(), encapsulation::appendix_slides(),
        accessors::appendix_slides(), constructors::appendix_slides(),
        const_and_this::appendix_slides(), discussions::appendix_slides(),
        static_members::appendix_slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::appendix);
}
