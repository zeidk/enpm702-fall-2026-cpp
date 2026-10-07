# =============================================================================
# Week 6: Functions, Advanced Topics. The fleet manager.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-6 program
#   702run week6_fleet            run the finished program
#   702run week6_playground       run the code of every slide
#   702run week6_playground 8     run only the code of [Slide 8]
#   702run week6_appendix         run the code of every appendix frame
#   702run week6_appendix xiii    run only the code of [Appendix xiii]
#
# Folders:
#   fleet/        the finished program (week6_fleet), headers and sources
#   playground/   the code of every slide, one file per section (week6_playground)
#   appendix/     the code of every appendix frame (week6_appendix)
#   common/       run_slides(), which both programs use to pick a slide
#   undefined/    the slides whose code has undefined behavior, built with AddressSanitizer
# =============================================================================

# -----------------------------------------------------------------------------
# 1. The fleet manager. Every .cpp is listed; a .hpp never is (Lecture 5).
# -----------------------------------------------------------------------------
add_executable(week6_fleet fleet/src/main.cpp
    fleet/src/fleet_queries.cpp
    fleet/src/dispatcher.cpp)
target_include_directories(week6_fleet PRIVATE fleet/include)

# -----------------------------------------------------------------------------
# 2. The playground: the code of every slide, in one program. Pass a slide
#    number to run only that slide. A slide whose code throws or has undefined
#    behavior runs only when asked for by its number.
#
#    The code with undefined behavior is in undefined/undefined.cpp. That one
#    file is always compiled with AddressSanitizer, so the dangling capture
#    stops with a report that names the bug and the line. The other files are
#    compiled as usual. The program is linked with -fsanitize as well, which
#    adds the sanitizer runtime that prints the report.
# -----------------------------------------------------------------------------
set_source_files_properties(undefined/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address;-fno-omit-frame-pointer")

add_executable(week6_playground playground/main.cpp
    playground/grouping.cpp
    playground/results.cpp
    playground/templates.cpp
    playground/lambdas.cpp
    playground/higher_order.cpp
    undefined/undefined.cpp
    common/slides.cpp)
target_include_directories(week6_playground PRIVATE common playground undefined)
target_link_options(week6_playground PRIVATE -fsanitize=address)

# -----------------------------------------------------------------------------
# 3. The appendix: the code of every appendix frame, in one program. Pass the
#    frame's number, as a roman numeral (xiii) or not (13).
# -----------------------------------------------------------------------------
add_executable(week6_appendix appendix/appendix.cpp common/slides.cpp)
target_include_directories(week6_appendix PRIVATE common)
