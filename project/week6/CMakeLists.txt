# =============================================================================
# Week 6: Functions, Advanced Topics. The fleet manager.
#
#   702configure                  once, after pulling
#   702build                      build every week-6 program
#   702run week6_types            run every snippet of Section 1
#   702run week6_types 7         run only the snippet of [Slide 7]
#
# Folders:
#   fleet/        the finished program (week6_fleet), headers and sources
#   snippets/     one program per lecture section, every slide's code runs
#   diagnostics/  one file per slide whose code does NOT compile, or warns
#   throws/       the two slides whose code throws an exception
#   undefined/    the two slides whose code has undefined behavior
#   exercises/    one program per exercise
# =============================================================================

# -----------------------------------------------------------------------------
# 1. The fleet manager. Every .cpp is listed; a .hpp never is (Lecture 5).
# -----------------------------------------------------------------------------
add_executable(week6_fleet fleet/src/main.cpp
    fleet/src/fleet_queries.cpp
    fleet/src/dispatcher.cpp)
target_include_directories(week6_fleet PRIVATE fleet/include)

# -----------------------------------------------------------------------------
# 2. The snippets: one program per section of the slides.
# -----------------------------------------------------------------------------
add_executable(week6_types snippets/types.cpp)
add_executable(week6_results snippets/results.cpp)
add_executable(week6_templates snippets/templates.cpp)
add_executable(week6_lambdas snippets/lambdas.cpp)
add_executable(week6_callables snippets/callables.cpp)

# -----------------------------------------------------------------------------
# 3. The exercises: compile and run each one after you have written your answer.
#    Exercise 3 is not a target: some of its calls do not compile, which is the
#    exercise. Compile it by hand: 702g++ exercises/exercise3.cpp
#    The files in diagnostics/ are not targets either, for the same reason.
# -----------------------------------------------------------------------------
add_executable(week6_ex1 exercises/exercise1.cpp)
add_executable(week6_ex2 exercises/exercise2.cpp)
add_executable(week6_ex4 exercises/exercise4.cpp)

# -----------------------------------------------------------------------------
# 4. Code that throws. It compiles and runs, then stops with exit status 134.
# -----------------------------------------------------------------------------
add_executable(week6_optional_value throws/optional_value.cpp)
add_executable(week6_function_empty throws/function_empty.cpp)

# -----------------------------------------------------------------------------
# 5. Undefined behavior. AddressSanitizer is always on for the dangling
#    capture, so its report appears without editing anything.
# -----------------------------------------------------------------------------
add_executable(week6_dangling undefined/dangling_capture.cpp)
target_compile_options(week6_dangling PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(week6_dangling PRIVATE -fsanitize=address)

add_executable(week6_optional_star undefined/optional_star.cpp)
