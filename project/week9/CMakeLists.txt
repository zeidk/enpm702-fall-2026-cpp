# =============================================================================
# Week 9: OOP Advanced. Class relationships, inheritance, polymorphism and
# abstraction, on the robotaxi's classes.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-9 program
#   702run week9_appendix         run the code of every appendix frame
#   702run week9_appendix viii   run only the code of [Appendix viii]
#   702run week9_live             run the classes of the Live Coding steps
#   702run week9_operators        run every part of the reading page Operator
#                                 Overloading
#   702run week9_operators print  run only the part "print" of that page
#   702run week9_cycle            two headers that need each other, built with
#                                 forward declarations
#
# Folders (each keeps its .cpp files in src/ and its .hpp files in include/):
#   appendix/     the code of the appendix frames (week9_appendix)
#   common/       run_slides() and the classes of the class diagram: Vehicle
#                 (abstract), RoboTaxi and its AutomationLevel, Taxi, Fleet, Passenger, and
#                 L8's Sensor, SensorType, Position and Route
#   operators/    the program of the reading page Operator Overloading on Read
#                 the Docs, one file per part (week9_operators), with its own
#                 Position and Route
#   cycle/        two small classes, A and B, that refer to each other, with
#                 forward declarations instead of a cyclic include (week9_cycle)
#   live/         the classes of the "Live Coding" slides (week9_live):
#                 Vehicle, Sensor, SensorType, AutomationLevel, Route and
#                 Position are given; Passenger, Fleet, RoboTaxi and Taxi as
#                 Steps 1 to 3 leave them are given too and reviewed in class;
#                 Step 4 on your own, 5 and 6 at home. One header and one source file
#                 per class (Position, SensorType and AutomationLevel: a header
#                 only), and main.cpp
# =============================================================================

# -----------------------------------------------------------------------------
# week9_appendix is built from common/ and appendix/, which hold the finished
# classes of the Live Coding steps. Those folders join the repository after the
# Live Coding deadline (Nov 10); until then, only the programs below are built.
# -----------------------------------------------------------------------------
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/common AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/appendix)

# -----------------------------------------------------------------------------
# The appendix: the code of every appendix frame that has code. Pass the
# frame's number, as a roman numeral (x) or not (10). The program is linked
# with -fsanitize, so
# the run of "With a virtual Destructor" is checked for a leak and a wrong
# delete.
# -----------------------------------------------------------------------------
add_executable(week9_appendix appendix/src/appendix.cpp
    common/src/sensor.cpp
    common/src/route.cpp
    common/src/vehicle.cpp
    common/src/robotaxi.cpp
    common/src/taxi.cpp
    common/src/fleet.cpp
    common/src/slides.cpp)
target_include_directories(week9_appendix PRIVATE common/include appendix/include)
target_link_options(week9_appendix PRIVATE -fsanitize=address,undefined)

endif()

# -----------------------------------------------------------------------------
# The reading page Operator Overloading (Read the Docs): one program, one
# source file per part of the page. Pass the part's key, the name of its file:
# 702run week9_operators print. Position and Route are copies of common's,
# with the operators the page adds, so the program builds on its own. It is
# compiled with AddressSanitizer and UndefinedBehaviorSanitizer, as
# week8_rule_of_five is.
# -----------------------------------------------------------------------------
add_executable(week9_operators operators/src/main.cpp
    operators/src/calls.cpp
    operators/src/print.cpp
    operators/src/compare.cpp
    operators/src/member_output.cpp
    operators/src/by_value.cpp
    operators/src/no_const.cpp
    operators/src/position.cpp
    operators/src/route.cpp)
target_include_directories(week9_operators PRIVATE operators/include)
target_compile_options(week9_operators PRIVATE -fsanitize=address,undefined
    -fno-omit-frame-pointer)
target_link_options(week9_operators PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# Live coding: Vehicle and the classes of L9, step by step (the "Live Coding"
# slides): Steps 1 to 3 are given and reviewed in class, Step 4 on your own,
# Steps 5 and 6 at home. One header and one source file per class in live/, and
# main.cpp, which has one block per step (Steps 4 to 6 commented out). Route, Position,
# Sensor, SensorType and AutomationLevel are given in live/ too, so week9_live
# is built from live/ alone. The Step 4 block deletes a Taxi through a Vehicle pointer,
# so the whole program is built with AddressSanitizer and
# UndefinedBehaviorSanitizer: without the virtual destructor
# that Step 4 adds to Vehicle, the run would stop with a report.
# -----------------------------------------------------------------------------
add_executable(week9_live live/src/main.cpp
    live/src/vehicle.cpp
    live/src/passenger.cpp
    live/src/fleet.cpp
    live/src/robotaxi.cpp
    live/src/taxi.cpp
    live/src/sensor.cpp
    live/src/route.cpp)
target_include_directories(week9_live PRIVATE live/include)
target_compile_options(week9_live PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(week9_live PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# [Appendix xxxvii] Two Headers That Include Each Other
# [Appendix xxxviii] A Forward Declaration Breaks the Cycle
# A class A that refers to a B and a class B that refers back to its A: a
# two-way association. Each header needs the other class's name, and
# a forward declaration gives it the name only. The program prints "built".
#
# [Appendix xxxvii] includes each header from the other instead. These two headers,
# comment line and "// ..." line included, are the files that gave the error
# below.
#
#   // a.hpp
#   #pragma once
#   #include "b.hpp"
#   class A {
#     // ...
#    private:
#     B* b_;
#   };
#
#   // b.hpp
#   #pragma once
#   #include "a.hpp"
#   class B {
#     // ...
#    private:
#     A* a_;
#   };
#
# A main.cpp whose first line is #include "a.hpp" stops with this error, the
# only one (g++ 13.3, -std=c++20 -Wall -Wextra -pedantic-errors -Wshadow):
#
#   b.hpp:7:3: error: 'A' does not name a type
#
# a.hpp includes b.hpp, which includes a.hpp again; #pragma once skips that
# copy, so B is compiled before A is declared.
# -----------------------------------------------------------------------------
add_executable(week9_cycle cycle/src/main.cpp
    cycle/src/a.cpp
    cycle/src/b.cpp)
target_include_directories(week9_cycle PRIVATE cycle/include)
