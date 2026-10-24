# =============================================================================
# Week 8: OOP Basics. Classes for the software of L7's autonomous car.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-8 program
#   702run week8_playground       run the code of every slide
#   702run week8_playground 10    run only the code of [Slide 10]
#   702run week8_appendix         run the code of every appendix frame
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   playground/   the code of every slide, one file per section (week8_playground)
#   appendix/     the code of the appendix frames (week8_appendix)
#   common/       run_slides(), the Pose and Obstacle structs of week 7, and the
#                 classes of the lecture (CruiseControl, Tracker)
#   undefined/    the slides that leak or have undefined behavior, built with sanitizers
# =============================================================================

# -----------------------------------------------------------------------------
# Leaks and undefined behavior: the code of those slides is in
# undefined/src/undefined.cpp. This one file is always compiled with
# AddressSanitizer and UndefinedBehaviorSanitizer, so a double free stops with a
# report, and LeakSanitizer reports a leak when the program ends. The program is
# linked with -fsanitize as well, which adds the runtime that prints the reports.
# -----------------------------------------------------------------------------
set_source_files_properties(undefined/src/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")


add_executable(week8_playground playground/src/main.cpp
    playground/src/classes.cpp
    playground/src/struct_or_class.cpp
    playground/src/encapsulation.cpp
    playground/src/accessors.cpp
    playground/src/constructors.cpp
    playground/src/const_and_this.cpp
    playground/src/destructors.cpp
    playground/src/static_members.cpp
    playground/src/discussions.cpp
    undefined/src/undefined.cpp
    common/src/cruise_control.cpp
    common/src/tracker.cpp
    common/src/slides.cpp)
target_include_directories(week8_playground PRIVATE
    common/include playground/include undefined/include)
target_link_options(week8_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# The appendix: the code of every appendix frame that has code. Pass the
# frame's number, as a roman numeral (iii) or not (3).
# -----------------------------------------------------------------------------
add_executable(week8_appendix appendix/src/appendix.cpp common/src/slides.cpp)
target_include_directories(week8_appendix PRIVATE common/include)
