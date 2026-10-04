# Runs PROGRAM, which must stop on an uncaught exception, and checks that what
# it printed on stderr matches PATTERN. Called by the week6.throws.* tests:
#   cmake -DPROGRAM=<exe> -DPATTERN=<regex> -P check_throws.cmake
#
# A plain add_test cannot do this: CTest reports a program that aborts as
# failed, whatever it printed.

execute_process(COMMAND ${PROGRAM}
                OUTPUT_VARIABLE out
                ERROR_VARIABLE err
                RESULT_VARIABLE status)
if(status EQUAL 0)
    message(FATAL_ERROR "${PROGRAM} finished normally; it should have stopped on an exception")
endif()
if(NOT err MATCHES "${PATTERN}")
    message(FATAL_ERROR "${PROGRAM} stopped (${status}), but stderr does not match "
                        "'${PATTERN}':\n${err}")
endif()
