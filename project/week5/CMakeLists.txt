# =============================================================================
# Week 5: Functions.
# Author: Zeid Kootbally
#
#   702configure                       once, after pulling
#   702build                           build every week-5 program
#   702run week5_playground            run the code of every slide
#   702run week5_playground 12         run only the code of [Slide 12]
#   702run week5_appendix              run the code of every appendix frame
#   702run week5_appendix iv           run only the code of [Appendix iv]
#   702run week5_arguments 30 -45 60   [Slide 77], with its own arguments
#
# Folders:
#   playground/   the code of every slide, in one program (week5_playground)
#   appendix/     the code of every appendix frame, in one program (week5_appendix)
#   common/       run_slides(), which both programs use to pick a slide
#   undefined/    the slides whose code has undefined behavior, built with sanitizers
#   arguments/    [Slide 77] Command-line Arguments, a program with its own main
#   arm_demo/     the arm program split into headers and sources (week5_arm_demo)
# =============================================================================

# ---------------------------------------------------------------------------
# 1. Undefined behavior: the code of those slides is in undefined/undefined.cpp,
#    and both programs below list it. This one file is always compiled with
#    AddressSanitizer and UndefinedBehaviorSanitizer, so each run prints a
#    report that names the bug and the line. The other files are compiled as
#    usual, so their addresses and timings are the ordinary ones. Each program
#    is then linked with -fsanitize as well, which adds the sanitizer runtime
#    that prints the reports.
# ---------------------------------------------------------------------------
set_source_files_properties(undefined/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")

# ---------------------------------------------------------------------------
# 2. The playground: the code of every slide, in one program. Pass a slide
#    number to run only that slide. A slide with undefined behavior runs only
#    when asked for by its number.
# ---------------------------------------------------------------------------
add_executable(week5_playground playground/src/playground.cpp
    common/slides.cpp
    undefined/undefined.cpp)
target_include_directories(week5_playground PRIVATE common undefined)
target_link_options(week5_playground PRIVATE -fsanitize=address,undefined)

# ---------------------------------------------------------------------------
# 3. The appendix: the code of every appendix frame, in one program. Pass the
#    frame's number, as a roman numeral (iv) or not (4).
# ---------------------------------------------------------------------------
add_executable(week5_appendix appendix/src/appendix.cpp
    common/slides.cpp
    undefined/undefined.cpp)
target_include_directories(week5_appendix PRIVATE common undefined)
target_link_options(week5_appendix PRIVATE -fsanitize=address,undefined)

# ---------------------------------------------------------------------------
# 4. [Slide 77] Command-line Arguments: a program with its own main, which
#    reads its own command line, so it cannot take a slide number.
# ---------------------------------------------------------------------------
add_executable(week5_arguments arguments/command_line.cpp)

# ---------------------------------------------------------------------------
# 5. The arm demo: the complete program from the slides, in several files
#    and documented with Doxygen. The Header Files slides and Exercise 1 use
#    it, and so does the Documenting Functions section. It reads three joint
#    angles from its own command line, so it takes no slide number.
#    [Slide 17] Uncomment the four lines below when you reach the Header Files
#    section, then run 702configure and 702build week5_arm_demo.
#
#    Every .cpp of the program is listed. Leave one out and the build fails at
#    the LINK step with "undefined reference", not at compile time. A .hpp is
#    never listed: target_include_directories says where to find it.
# ---------------------------------------------------------------------------
# add_executable(week5_arm_demo arm_demo/src/main.cpp
#     arm_demo/src/kinematics.cpp
#     arm_demo/src/joint_limits.cpp)
# target_include_directories(week5_arm_demo PRIVATE arm_demo/include)
