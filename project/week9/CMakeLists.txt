# =============================================================================
# Week 9: OOP Advanced. Class relationships, inheritance, polymorphism and
# abstraction, on the robotaxi's classes.
# Author: Zeid Kootbally
#
# From this folder, once per terminal: source week9.sh. Then:
#   week9                           build every week-9 program, then run the
#                                   ride service
#   week9 75                        build, then run only the code of [Slide 75]
#   week9 xl                        build, then run only the code of [Appendix xl]
#
# The programs, for 702run after 702build:
#   week9_ride_service              the classes of the ride service
#   week9_playground                the code of every slide and appendix frame
#   week9_operators                 the reading page Operator Overloading; pass a
#                                   part's key to run only that part (print)
#   week9_cycle                     two headers that need each other, built with
#                                   forward declarations
#
# Two folders:
#   ride_service/ the classes of the robotaxi ride service (week9_ride_service):
#                 Vehicle, Sensor, SensorType, AutomationLevel, Route, Position,
#                 Passenger, Fleet, RoboTaxi and Taxi, one header and one source
#                 file per class (Position, SensorType and AutomationLevel: a
#                 header only), and main.cpp. virtual, override and final are
#                 left out: they are added in class.
#   playground/   the code of every slide and appendix frame, and the finished
#                 classes of the class diagram (week9_playground); in
#                 playground/operators, the program of the reading page Operator
#                 Overloading (week9_operators); in playground/cycle, two classes
#                 that refer to each other (week9_cycle); in
#                 playground/not_compiling, the code of the slides that do not
#                 compile, outside every build; in playground/uml, the PlantUML
#                 source of the diagrams. playground/README.md lists them.
# =============================================================================

# -----------------------------------------------------------------------------
# The ride service: Vehicle and the classes of L9, one header and one source
# file per class in ride_service/, and main.cpp, which has one block per step
# of the lecture. drive() is written in Vehicle, RoboTaxi and Taxi; in class,
# Steps 4 to 6 add virtual, override, = 0 and final. A block of main.cpp deletes a
# Taxi through a Vehicle pointer, so the whole program is built with
# AddressSanitizer and UndefinedBehaviorSanitizer: without the virtual
# destructor, the run would stop with a report.
# -----------------------------------------------------------------------------
add_executable(week9_ride_service ride_service/src/main.cpp
    ride_service/src/vehicle.cpp
    ride_service/src/passenger.cpp
    ride_service/src/fleet.cpp
    ride_service/src/robotaxi.cpp
    ride_service/src/taxi.cpp
    ride_service/src/sensor.cpp
    ride_service/src/route.cpp)
target_include_directories(week9_ride_service PRIVATE ride_service/include)
target_compile_options(week9_ride_service PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
target_link_options(week9_ride_service PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# week9_playground: the code of every slide and appendix frame. Pass the
# frame's number: 75 for [Slide 75], xl for [Appendix xl].
#
# Undefined behavior: the code of those slides is in
# playground/src/undefined.cpp. This one file is always compiled with
# AddressSanitizer and UndefinedBehaviorSanitizer, so a delete through the wrong
# type stops with a report. The program is linked with -fsanitize as well, which
# adds the runtime that prints the reports.
# -----------------------------------------------------------------------------
set_source_files_properties(playground/src/undefined.cpp PROPERTIES
    COMPILE_OPTIONS "-fsanitize=address,undefined;-fno-omit-frame-pointer")

add_executable(week9_playground playground/src/main.cpp
    playground/src/relationships.cpp
    playground/src/inheritance.cpp
    playground/src/polymorphism.cpp
    playground/src/abstraction.cpp
    playground/src/undefined.cpp
    playground/src/appendix.cpp
    playground/src/sensor.cpp
    playground/src/route.cpp
    playground/src/vehicle.cpp
    playground/src/robotaxi.cpp
    playground/src/taxi.cpp
    playground/src/fleet.cpp
    playground/src/passenger.cpp
    playground/src/slides.cpp)
target_include_directories(week9_playground PRIVATE playground/include)
target_link_options(week9_playground PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# The reading page Operator Overloading (Read the Docs): one program, one
# source file per part of the page. Pass the part's key, the name of its file:
# 702run week9_operators print. Position and Route are copies of the playground's,
# with the operators the page adds, so the program builds on its own. It is
# compiled with AddressSanitizer and UndefinedBehaviorSanitizer, as
# week8_rule_of_five is.
# -----------------------------------------------------------------------------
add_executable(week9_operators playground/operators/src/main.cpp
    playground/operators/src/calls.cpp
    playground/operators/src/print.cpp
    playground/operators/src/compare.cpp
    playground/operators/src/member_output.cpp
    playground/operators/src/by_value.cpp
    playground/operators/src/no_const.cpp
    playground/operators/src/position.cpp
    playground/operators/src/route.cpp)
target_include_directories(week9_operators PRIVATE playground/operators/include)
target_compile_options(week9_operators PRIVATE -fsanitize=address,undefined
    -fno-omit-frame-pointer)
target_link_options(week9_operators PRIVATE -fsanitize=address,undefined)

# -----------------------------------------------------------------------------
# [Appendix xxxviii] Two Headers That Include Each Other
# [Appendix xxxix] A Forward Declaration Breaks the Cycle
# A class A that refers to a B and a class B that refers back to its A: a
# two-way association. Each header needs the other class's name, and
# a forward declaration gives it the name only. The program prints "built".
#
# [Appendix xxxviii] includes each header from the other instead: those two
# headers are in playground/not_compiling/appendix_xxxviii_cycle, with a main.cpp
# that stops with the error on the slide.
# -----------------------------------------------------------------------------
add_executable(week9_cycle playground/cycle/src/main.cpp
    playground/cycle/src/a.cpp
    playground/cycle/src/b.cpp)
target_include_directories(week9_cycle PRIVATE playground/cycle/include)
