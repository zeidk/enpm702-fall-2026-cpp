add_executable(week4 src/main.cpp)

# AddressSanitizer names the use-after-free on the iterator-invalidation
# slides and in Exercise 5. Uncomment both lines, rebuild, run, then comment
# them back out before you submit.
# target_compile_options(week4 PRIVATE -fsanitize=address,undefined)
# target_link_options(week4 PRIVATE -fsanitize=address,undefined)
