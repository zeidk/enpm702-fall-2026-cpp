# =============================================================================
# Week 9: OOP Advanced. Class relationships, inheritance, polymorphism and
# abstraction, on the robotaxi's classes.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-9 program
#   702run week9_playground       run the code of every slide
#   702run week9_playground 20    run only the code of [Slide 20]
#   702run week9_appendix         run the code of every appendix frame
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   playground/   the code of every slide, one file per section (week9_playground)
#   appendix/     the code of the appendix frames (week9_appendix)
#   common/       run_slides() and the classes of the robotaxi design: Vehicle
#                 (abstract), RoboTaxi, Taxi, Fleet, Passenger, and L8's Sensor,
#                 SensorType, Position and Route
#   undefined/    the slides that have undefined behavior, built with sanitizers
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
# frame's number, as a roman numeral (iv) or not (4).
# -----------------------------------------------------------------------------
add_executable(week9_appendix appendix/src/appendix.cpp
    common/src/sensor.cpp
    common/src/route.cpp
    common/src/vehicle.cpp
    common/src/taxi.cpp
    common/src/slides.cpp)
target_include_directories(week9_appendix PRIVATE common/include)
