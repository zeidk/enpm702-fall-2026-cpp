add_executable(week3 src/main.cpp)

find_program(VALGRIND_EXECUTABLE valgrind)

if(VALGRIND_EXECUTABLE)
    add_custom_target(memcheck
        COMMAND ${VALGRIND_EXECUTABLE}
                --leak-check=full
                --show-leak-kinds=all
                --track-origins=yes
                --error-exitcode=1
                $<TARGET_FILE:week3>
        DEPENDS week3
        COMMENT "Running Valgrind on week3"
        VERBATIM)
endif()

# target_compile_options(week3 PRIVATE -fsanitize=address,undefined)
# target_link_options(week3 PRIVATE -fsanitize=address,undefined)
