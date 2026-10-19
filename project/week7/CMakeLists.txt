# =============================================================================
# Week 7: Smart Pointers and Move Semantics. The software of an autonomous car.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-7 program
#   702run week7_playground       run the code of every slide
#   702run week7_playground 14    run only the code of [Slide 14]
#   702run week7_appendix         run the code of every appendix frame
#   702run week7_appendix ii      run only the code of [Appendix ii]
#   702run week7_allocations      [Appendix xiv]: count make_shared's allocations
#
# Folders:
#   playground/   the code of every slide, one file per section (week7_playground)
#   appendix/     the code of every appendix frame (week7_appendix)
#   common/       run_slides(), and the Pose and Obstacle types every file shares
#   undefined/    the slides that leak or have undefined behavior, built with sanitizers
# =============================================================================

# -----------------------------------------------------------------------------
# 1. Leaks and undefined behavior: the code of those slides is in
#    undefined/undefined.cpp, and both programs below list it. This one file is
#    always compiled with AddressSanitizer and UndefinedBehaviorSanitizer, so a
#    double delete or a read of freed memory stops with a report, and
#    LeakSanitizer reports a leak when the program ends. The other files are
#    compiled as usual. Each program is linked with -fsanitize as well, which
#    adds the sanitizer runtime that prints the reports.
# -----------------------------------------------------------------------------
set_source_files_properties(undefined/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")

# -----------------------------------------------------------------------------
# 2. The playground: the code of every slide, in one program. Pass a slide
#    number to run only that slide. A slide whose code leaks or has undefined
#    behavior runs only when asked for by its number.
# -----------------------------------------------------------------------------
add_executable(week7_playground playground/main.cpp
    playground/ownership.cpp
    playground/moves.cpp
    playground/unique.cpp
    playground/shared.cpp
    playground/weak.cpp
    undefined/undefined.cpp
    common/slides.cpp)
target_include_directories(week7_playground PRIVATE common playground undefined)
target_link_options(week7_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 3. The appendix: the code of every appendix frame, in one program. Pass the
#    frame's number, as a roman numeral (ii) or not (2).
# -----------------------------------------------------------------------------
add_executable(week7_appendix appendix/appendix.cpp
    undefined/undefined.cpp
    common/slides.cpp)
target_include_directories(week7_appendix PRIVATE common undefined)
target_link_options(week7_appendix PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 4. [Appendix xiv] counts heap allocations by replacing the global operator
#    new. The sanitizers replace it as well, so this one frame is a program of
#    its own, built without them.
# -----------------------------------------------------------------------------
add_executable(week7_allocations appendix/allocations.cpp)
target_include_directories(week7_allocations PRIVATE common)
