# ---------------------------------------------------------------------------
# 1. The snippets: every code snippet from the L4 slides, in one file that
#    stands alone. Each block sits between #if 0 and #endif; set one to #if 1
#    to try it.
# ---------------------------------------------------------------------------
add_executable(week4 src/main.cpp)

# [Slide 29] [Slide 60] [Slide 76] and the C-string appendix frames. The
# sanitizer names the out-of-bounds writes and the use-after-free. Uncomment
# both lines, rebuild, run, then comment them back out before you submit.
# target_compile_options(week4 PRIVATE -fsanitize=address,undefined)
# target_link_options(week4 PRIVATE -fsanitize=address,undefined)

# ---------------------------------------------------------------------------
# 2. The exercises: your answers to the Lecture 4 C++ exercises, one block
#    per exercise. Submit a copy of the file named firstname_lastname.cpp.
# ---------------------------------------------------------------------------
add_executable(week4_exercises exercises/exercises.cpp)

# Exercise 5 asks for AddressSanitizer. Uncomment both lines, rebuild, run,
# then comment them back out before you submit.
# target_compile_options(week4_exercises PRIVATE -fsanitize=address,undefined)
# target_link_options(week4_exercises PRIVATE -fsanitize=address,undefined)
