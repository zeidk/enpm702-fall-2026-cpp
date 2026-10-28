# =============================================================================
# Week 8: OOP Basics. Classes for the software of a robotaxi.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every program (the four of week 8 too)
#   702run week8_playground       run the code of every slide
#   702run week8_playground 9     run only the code of [Slide 9]
#   702run week8_appendix         run the code of every appendix frame
#   702run week8_appendix ii      run only the code of [Appendix ii]
#   702run week8_rule_of_five     run every part of the reading page Rule of
#                                 Five and Rule of Zero that is safe to run
#   702run week8_rule_of_five five  run only the part "five" of that page
#   702run week8_live             run the class we write together in class
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   playground/   the code of every slide, one file per section (week8_playground).
#                 Each file also holds the appendix frames of its section.
#   appendix/     the program of the appendix frames (week8_appendix)
#   common/       run_slides(), and the classes of the lecture: Vehicle, Sensor
#                 with its SensorType, the Position struct, and Route (drive() takes one)
#   rule_of_five/ the program of the reading page Rule of Five and Rule of
#                 Zero on Read the Docs, one file per part (week8_rule_of_five)
#   live/         the class Vehicle we write together on the "Live Coding" slides
#                 (week8_live): one header, one source file and main.cpp
# =============================================================================

# -----------------------------------------------------------------------------
# 1. The section files: one per section of the deck. Each lists the slides of
#    the lecture in its slides() and the appendix frames of its section in its
#    appendix_slides(), so both programs below build them, with the classes of
#    common/.
# -----------------------------------------------------------------------------
set(WEEK8_SECTIONS
    playground/src/classes.cpp
    playground/src/struct_or_class.cpp
    playground/src/encapsulation.cpp
    playground/src/accessors.cpp
    playground/src/constructors.cpp
    playground/src/const_and_this.cpp
    playground/src/destructors.cpp
    playground/src/static_members.cpp
    playground/src/discussions.cpp
    common/src/vehicle.cpp
    common/src/sensor.cpp
    common/src/route.cpp
    common/src/slides.cpp)

# -----------------------------------------------------------------------------
# 2. The playground: the code of every slide, in one program. Pass a slide
#    number to run only that slide. A slide whose code stops the program runs
#    only when asked for by its number.
# -----------------------------------------------------------------------------
add_executable(week8_playground playground/src/main.cpp ${WEEK8_SECTIONS})
target_include_directories(week8_playground PRIVATE common/include playground/include)
target_link_options(week8_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 3. The appendix: the code of every appendix frame that has code, in one
#    program. Pass the frame's number, as a roman numeral (xi) or not (11).
# -----------------------------------------------------------------------------
add_executable(week8_appendix appendix/src/appendix.cpp
    appendix/src/speed_user.cpp
    ${WEEK8_SECTIONS})
target_include_directories(week8_appendix PRIVATE common/include playground/include
    appendix/include)
target_link_options(week8_appendix PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 4. The reading page Rule of Five and Rule of Zero (Read the Docs): one
#    program, one source file per part of the page. Pass the part's key, the
#    name of its file: 702run week8_rule_of_five five. Every file is compiled
#    with AddressSanitizer and UndefinedBehaviorSanitizer: a part that leaks
#    or deletes memory twice stops with a report (a full run skips it), and
#    the other parts show that they do neither.
# -----------------------------------------------------------------------------
add_executable(week8_rule_of_five rule_of_five/src/main.cpp
    rule_of_five/src/which_one.cpp
    rule_of_five/src/zero.cpp
    rule_of_five/src/raw_owner.cpp
    rule_of_five/src/no_destructor.cpp
    rule_of_five/src/double_free.cpp
    rule_of_five/src/five.cpp
    rule_of_five/src/move_noexcept.cpp
    rule_of_five/src/no_move.cpp
    rule_of_five/src/move_bug.cpp
    rule_of_five/src/move_only.cpp
    rule_of_five/src/file_owner.cpp
    common/src/sensor.cpp
    common/src/slides.cpp)
target_include_directories(week8_rule_of_five PRIVATE common/include rule_of_five/include)
target_compile_options(week8_rule_of_five PRIVATE -fsanitize=address,undefined
    -fno-omit-frame-pointer)
target_link_options(week8_rule_of_five PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 5. Live coding: the class Vehicle we write together in class, step by step
#    (the "Live Coding" slides). One header and one source file in live/, and
#    main.cpp, which has one commented block per step. Step 6 uses the course's
#    Sensor, so the target also builds common/src/sensor.cpp and finds
#    sensor.hpp in common/include. live/include comes first, so "vehicle.hpp"
#    is the one we write, not common's.
# -----------------------------------------------------------------------------
add_executable(week8_live live/src/main.cpp
    live/src/vehicle.cpp
    common/src/sensor.cpp)
target_include_directories(week8_live PRIVATE live/include common/include)
