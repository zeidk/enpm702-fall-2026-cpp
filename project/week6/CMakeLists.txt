# =============================================================================
# Week 6: Functions, Advanced Topics. The fleet manager.
#
#   702configure                  once, after pulling
#   702build                      build every week-6 program
#   702run week6_types            run every snippet of Section 1
#   702run week6_types 12         run only the snippet of [Slide 12]
#   702test week6                 run every week-6 check (needs GoogleTest)
#
# Folders:
#   fleet/        the finished program (week6_fleet), headers and sources
#   snippets/     one program per lecture section, every slide's code runs
#   diagnostics/  one file per slide whose code does NOT compile, or warns
#   throws/       the two slides whose code throws an exception
#   undefined/    the two slides whose code has undefined behavior
#   exercises/    one program per exercise
#   tests/        GoogleTest tests, and the expected output of each program
# =============================================================================

# -----------------------------------------------------------------------------
# 1. The fleet manager. Every .cpp is listed; a .hpp never is (Lecture 5).
#    The sources are kept in a variable because the tests compile them too.
# -----------------------------------------------------------------------------
set(WEEK6_FLEET_SOURCES
    fleet/src/fleet_queries.cpp
    fleet/src/dispatcher.cpp)

add_executable(week6_fleet fleet/src/main.cpp ${WEEK6_FLEET_SOURCES})
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

# -----------------------------------------------------------------------------
# 6. The tests. Skipped, with a message, when GoogleTest is not installed:
#      sudo apt install libgtest-dev
# -----------------------------------------------------------------------------
find_package(GTest)
if(GTest_FOUND)
    include(GoogleTest)
    add_executable(week6_tests
        tests/test_average_battery.cpp
        tests/test_fleet.cpp
        ${WEEK6_FLEET_SOURCES})
    target_include_directories(week6_tests PRIVATE fleet/include)
    target_link_libraries(week6_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(week6_tests TEST_PREFIX "week6.")
else()
    message(STATUS "week6: GoogleTest not found, week6_tests is skipped. "
                   "Install it with: sudo apt install libgtest-dev")
endif()

# -----------------------------------------------------------------------------
# 7. Checks that need no GoogleTest. Each one runs with 702test week6.
#
#    a. Each snippet program prints exactly what its slides say it prints.
#       The expected output is in tests/expected/.
# -----------------------------------------------------------------------------
foreach(program types results templates lambdas callables ex1 ex2 ex4 fleet)
    add_test(NAME week6.output.${program}
             COMMAND ${CMAKE_COMMAND}
                     -DPROGRAM=$<TARGET_FILE:week6_${program}>
                     -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/tests/expected/${program}.txt
                     -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_output.cmake)
endforeach()

#    b. The code that throws stops, and names the exception.
foreach(program optional_value function_empty)
    if(program STREQUAL "optional_value")
        set(pattern "bad_optional_access")
    else()
        set(pattern "bad_function_call")
    endif()
    add_test(NAME week6.throws.${program}
             COMMAND ${CMAKE_COMMAND}
                     -DPROGRAM=$<TARGET_FILE:week6_${program}>
                     -DPATTERN=${pattern}
                     -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_throws.cmake)
endforeach()

#    c. AddressSanitizer catches the dangling capture.
add_test(NAME week6.asan.dangling_capture COMMAND week6_dangling)
set_tests_properties(week6.asan.dangling_capture PROPERTIES
                     PASS_REGULAR_EXPRESSION "stack-use-after-return")

#    d. Each file in diagnostics/ fails to compile, or warns, with the message
#       on its slide. The flags are the ones every target here is built with.
#       GCC writes its quotes as curly UTF-8 characters, so no pattern below
#       contains a quote.
set(WEEK6_FLAGS -std=c++20 -Wall -Wextra -pedantic-errors -Wshadow -fsyntax-only)
function(week6_diagnostic name pattern)
    add_test(NAME week6.diagnostic.${name}
             COMMAND ${CMAKE_CXX_COMPILER} ${WEEK6_FLAGS}
                     ${CMAKE_CURRENT_SOURCE_DIR}/diagnostics/${name}.cpp)
    set_tests_properties(week6.diagnostic.${name} PROPERTIES
                         PASS_REGULAR_EXPRESSION "${pattern}")
endfunction()

week6_diagnostic(missing_semicolon "after struct definition")
week6_diagnostic(incomplete_type "has incomplete type")
week6_diagnostic(missing_initializer "missing initializer for member")
week6_diagnostic(designated_order "designator order for field")
week6_diagnostic(designated_mixed "either all initializer clauses should be designated")
week6_diagnostic(binding_count "only 2 names provided for structured binding")
week6_diagnostic(deduce_conflict "deduced conflicting types for parameter")
week6_diagnostic(deduce_return "deduce template parameter")
week6_diagnostic(average_int "constraints not satisfied")
week6_diagnostic(capture_missing "is not captured")
week6_diagnostic(capture_const "increment of read-only variable")
week6_diagnostic(lambda_return "deduced for lambda return type")
week6_diagnostic(template_lambda "deduced conflicting types for parameter")
week6_diagnostic(fnptr_capture "cannot convert")

#    e. Exercise 3: calls 2 and 5 are rejected, with these two messages.
add_test(NAME week6.exercise3
         COMMAND ${CMAKE_CXX_COMPILER} ${WEEK6_FLAGS}
                 ${CMAKE_CURRENT_SOURCE_DIR}/exercises/exercise3.cpp)
set_tests_properties(week6.exercise3 PROPERTIES
                     PASS_REGULAR_EXPRESSION "larger_of.double, int.*constraints not satisfied")

#    f. A template defined in a .cpp: compiles, then fails to LINK.
add_test(NAME week6.diagnostic.template_in_source
         COMMAND ${CMAKE_CXX_COMPILER} -std=c++20
                 ${CMAKE_CURRENT_SOURCE_DIR}/diagnostics/split/main.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/diagnostics/split/stats.cpp
                 -o ${CMAKE_CURRENT_BINARY_DIR}/week6_split_never_built)
set_tests_properties(week6.diagnostic.template_in_source PROPERTIES
                     PASS_REGULAR_EXPRESSION "undefined reference to .*clamp_value<double>")
