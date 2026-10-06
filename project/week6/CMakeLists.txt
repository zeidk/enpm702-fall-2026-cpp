# =============================================================================
# Week 6: Functions, Advanced Topics. The fleet manager.
# Author: Zeid Kootbally
#
#   702configure                  once, after pulling
#   702build                      build every week-6 program
#   702run week6_grouping         run every snippet of Section 1
#   702run week6_grouping 8       run only the snippet of [Slide 8]
#
# Folders:
#   fleet/        the finished program (week6_fleet), headers and sources
#   snippets/     one program per lecture section, every slide's code runs
#   throws/       the two slides whose code throws an exception
#   undefined/    the two slides whose code has undefined behavior
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
add_executable(week6_grouping snippets/grouping.cpp)
add_executable(week6_results snippets/results.cpp)
add_executable(week6_templates snippets/templates.cpp)
add_executable(week6_lambdas snippets/lambdas.cpp)
add_executable(week6_higher_order snippets/higher_order.cpp)

# -----------------------------------------------------------------------------
# 3. Code that throws. It compiles and runs, then stops with exit status 134.
# -----------------------------------------------------------------------------
add_executable(week6_optional_value throws/optional_value.cpp)
add_executable(week6_function_empty throws/function_empty.cpp)

# -----------------------------------------------------------------------------
# 4. Undefined behavior. AddressSanitizer is always on for the dangling
#    capture, so its report appears without editing anything.
# -----------------------------------------------------------------------------
add_executable(week6_dangling undefined/dangling_capture.cpp)
target_compile_options(week6_dangling PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_options(week6_dangling PRIVATE -fsanitize=address)

add_executable(week6_optional_star undefined/optional_star.cpp)
