# ---------------------------------------------------------------------------
# 1. The playground: every code snippet from the L5 slides, in one file.
#    Nothing here is documented and nothing is included by anything else.
# ---------------------------------------------------------------------------
add_executable(week5 playground/src/main.cpp)

# [Slide 55] [Slide 56] The dangling-reference snippets. Uncomment both lines,
# rebuild, run, then comment them back out before you submit.
# target_compile_options(week5 PRIVATE -fsanitize=address,undefined)
# target_link_options(week5 PRIVATE -fsanitize=address,undefined)

# ---------------------------------------------------------------------------
# 2. The Doxygen demo: a complete, documented, multi-file project.
#    [Slide 94] Uncomment the four lines below when you reach the Documenting
#    Functions section, then run 702configure and 702build.
#
#    Every .cpp of the program is listed. Leave one out and the build fails at
#    the LINK step with "undefined reference", not at compile time. A .hpp is
#    never listed: target_include_directories says where to find it.
# ---------------------------------------------------------------------------
# add_executable(week5_doxygen_demo doxygen_demo/src/main.cpp
#     doxygen_demo/src/kinematics.cpp
#     doxygen_demo/src/joint_limits.cpp)
# target_include_directories(week5_doxygen_demo PRIVATE doxygen_demo/include)
