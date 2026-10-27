# =============================================================================
# Week 8: OOP Basics. Classes for the software of a robotaxi.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every program (the three of week 8 too)
#   702run week8_playground       run the code of every slide
#   702run week8_playground 9     run only the code of [Slide 9]
#   702run week8_appendix         run the code of every appendix frame, then
#                                 of the reading page Rule of Five and Rule of Zero
#   702run week8_appendix ii      run only the code of [Appendix ii]
#   702run week8_appendix five    run only the part "five" of the reading page
#   702run week8_live             run the class we write together in class
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   playground/   the code of every slide, one file per section (week8_playground).
#                 Each file also holds the appendix frames of its section.
#   appendix/     the program of the appendix frames and of the reading page
#                 Rule of Five and Rule of Zero (week8_appendix)
#   common/       run_slides(), and the classes of the lecture: Vehicle, Sensor
#                 with its SensorType, the Position struct, and Route (drive() takes one)
#   undefined/    the parts of the reading page that leak or have undefined
#                 behavior, built with sanitizers
#   live/         the class Vehicle we write together on the "Live Coding" slides
#                 (week8_live): one header, one source file and main.cpp
# =============================================================================

# -----------------------------------------------------------------------------
# 1. Leaks and undefined behavior: the code of those reading parts is in
#    undefined/src/undefined.cpp. This one file is always compiled with
#    AddressSanitizer and UndefinedBehaviorSanitizer, so a double free stops with
#    a report, and LeakSanitizer reports a leak when the program ends. The other
#    files are compiled as usual. Each program is linked with -fsanitize as well,
#    which adds the sanitizer runtime: it prints the reports, and it reports a
#    leak or a double free anywhere in the program.
# -----------------------------------------------------------------------------
set_source_files_properties(undefined/src/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")

# -----------------------------------------------------------------------------
# 2. The section files: one per section of the deck. Each lists the slides of
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
# 3. The playground: the code of every slide, in one program. Pass a slide
#    number to run only that slide. A slide whose code stops the program runs
#    only when asked for by its number.
# -----------------------------------------------------------------------------
add_executable(week8_playground playground/src/main.cpp ${WEEK8_SECTIONS})
target_include_directories(week8_playground PRIVATE common/include playground/include)
target_link_options(week8_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# 4. The appendix: the code of every appendix frame that has code, then the
#    code of the reading page Rule of Five and Rule of Zero, in one program.
#    Pass the frame's number, as a roman numeral (xi) or not (11), or the key
#    of a reading part (five). A part whose code leaks or has undefined
#    behavior runs only when asked for by its key.
# -----------------------------------------------------------------------------
add_executable(week8_appendix appendix/src/appendix.cpp
    appendix/src/speed_user.cpp
    undefined/src/undefined.cpp
    ${WEEK8_SECTIONS})
target_include_directories(week8_appendix PRIVATE common/include playground/include
    undefined/include appendix/include)
target_link_options(week8_appendix PRIVATE -fsanitize=address,undefined)

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
