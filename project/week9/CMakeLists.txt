# =============================================================================
# Week 9: OOP Advanced. Class relationships, inheritance, polymorphism and
# abstraction, on the robotaxi's classes.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-9 program
#   702run week9_playground       run the code of every slide
#   702run week9_playground 57    run only the code of [Slide 57]
#   702run week9_appendix         run the code of every appendix frame
#   702run week9_appendix x       run only the code of [Appendix x]
#   702run week9_live             run the classes of the Live Coding steps
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   playground/   the code of every slide, one file per section (week9_playground)
#   appendix/     the code of the appendix frames (week9_appendix)
#   common/       run_slides() and the classes of the robotaxi design: Vehicle
#                 (abstract), RoboTaxi, Taxi, Fleet, Passenger, and L8's Sensor,
#                 SensorType, Position and Route
#   undefined/    the slides that have undefined behavior, built with sanitizers
#   live/         L8's Vehicle and the classes of L9, written on the "Live
#                 Coding" slides (week9_live): Step 1 with the instructor,
#                 Steps 2 to 7 on your own. One header, one source file and
#                 main.cpp
# =============================================================================

# -----------------------------------------------------------------------------
# Undefined behavior: the code of those slides is in undefined/src/undefined.cpp.
# This one file is always compiled with AddressSanitizer and
# UndefinedBehaviorSanitizer, so a delete through the wrong type stops with a
# report. The program is linked with -fsanitize as well,
# which adds the runtime that prints the reports.
# -----------------------------------------------------------------------------
set_source_files_properties(undefined/src/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")

add_executable(week9_playground playground/src/main.cpp
    playground/src/relationships.cpp
    playground/src/inheritance.cpp
    playground/src/polymorphism.cpp
    playground/src/abstraction.cpp
    playground/src/discussions.cpp
    undefined/src/undefined.cpp
    common/src/sensor.cpp
    common/src/route.cpp
    common/src/vehicle.cpp
    common/src/robotaxi.cpp
    common/src/taxi.cpp
    common/src/fleet.cpp
    common/src/passenger.cpp
    common/src/slides.cpp)
target_include_directories(week9_playground PRIVATE
    common/include playground/include undefined/include)
target_link_options(week9_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# The appendix: the code of every appendix frame that has code. Pass the
# frame's number, as a roman numeral (x) or not (10). The program is linked
# with -fsanitize, as week9_playground is, so
# the run of "With a virtual Destructor" is checked for a leak and a wrong
# delete.
# -----------------------------------------------------------------------------
add_executable(week9_appendix appendix/src/appendix.cpp
    common/src/sensor.cpp
    common/src/route.cpp
    common/src/vehicle.cpp
    common/src/robotaxi.cpp
    common/src/taxi.cpp
    common/src/slides.cpp)
target_include_directories(week9_appendix PRIVATE common/include)
target_link_options(week9_appendix PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# Live coding: L8's Vehicle and the classes of L9, written in class, step by
# step (the "Live Coding" slides): Step 1 with the instructor, Steps 2 to 7 on
# your own. One header and one source
# file in live/, and main.cpp, which has one commented block per step. Route,
# Position and Sensor are the course's, so the target also builds
# common/src/route.cpp and common/src/sensor.cpp and finds their headers in
# common/include. live/include comes first, so "fleet.hpp" is the one you write,
# not common's. Step 5 deletes a Taxi through a Vehicle pointer, so the whole
# program is built with AddressSanitizer and UndefinedBehaviorSanitizer, as
# undefined/ is: the run stops with a report until Vehicle has a virtual
# destructor.
# -----------------------------------------------------------------------------
add_executable(week9_live live/src/main.cpp
    live/src/fleet.cpp
    common/src/route.cpp
    common/src/sensor.cpp)
target_include_directories(week9_live PRIVATE live/include common/include)
target_compile_options(week9_live PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(week9_live PRIVATE -fsanitize=address,undefined)
