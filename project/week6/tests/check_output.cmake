# Runs PROGRAM and compares what it prints with the file EXPECTED.
# Called by the week6.output.* tests:
#   cmake -DPROGRAM=<exe> -DEXPECTED=<file> -P check_output.cmake
#
# The expected files hold what each slide says the code prints. If a slide's
# "// 2" comment and the program disagree, this check fails and shows both.

execute_process(COMMAND ${PROGRAM}
                OUTPUT_VARIABLE actual
                RESULT_VARIABLE status)
if(NOT status EQUAL 0)
    message(FATAL_ERROR "${PROGRAM} exited with status ${status}")
endif()

file(READ ${EXPECTED} expected)
if(NOT actual STREQUAL expected)
    message(FATAL_ERROR "Output differs from ${EXPECTED}\n"
                        "--- expected ---\n${expected}"
                        "--- actual ---\n${actual}")
endif()
