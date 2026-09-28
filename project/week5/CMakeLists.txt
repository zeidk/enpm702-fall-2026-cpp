# ---------------------------------------------------------------------------
# 1. The snippets: every code snippet from the L5 slides, in one file that
#    stands alone. Each block sits between #if 0 and #endif; set one to #if 1
#    to try it.
# ---------------------------------------------------------------------------
add_executable(week5_snippets playground/src/snippets.cpp)

# [Slide 54] [Slide 55] [Slide 57] The dangling snippets. Uncomment both lines,
# rebuild, run, then comment them back out before you submit.
# target_compile_options(week5_snippets PRIVATE -fsanitize=address,undefined)
# target_link_options(week5_snippets PRIVATE -fsanitize=address,undefined)

# ---------------------------------------------------------------------------
# 2. The arm demo: the complete program from the slides, in several files
#    and documented with Doxygen. The Header Files slides and Exercise 1 use
#    it, and so does the Documenting Functions section.
#    [Slide 17] Uncomment the four lines below when you reach the Header Files
#    section, then run 702configure and 702build week5_arm_demo.
#
#    Every .cpp of the program is listed. Leave one out and the build fails at
#    the LINK step with "undefined reference", not at compile time. A .hpp is
#    never listed: target_include_directories says where to find it.
# ---------------------------------------------------------------------------
# add_executable(week5_arm_demo arm_demo/src/main.cpp
#     arm_demo/src/kinematics.cpp
#     arm_demo/src/joint_limits.cpp)
# target_include_directories(week5_arm_demo PRIVATE arm_demo/include)
