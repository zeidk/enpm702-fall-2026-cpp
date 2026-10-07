/**
 * @file command_line.cpp
 * @brief L5 [Slide 77] Command-line Arguments: the slide's program, on its own.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week5_arguments. It has its own @c main(), so it
 * reads its own command line. Run it from the folder that holds it, as on the
 * slide:
 * @code
 * cd build/project/week5
 * ./week5_arguments 30 -45 60
 * @endcode
 * or from anywhere with @c "702run week5_arguments 30 -45 60". Then
 * @c argv[0] is the full path that @c 702run used, not @c ./week5_arguments.
 */
#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "Number of arguments: " << argc << '\n';
  for (int i{0}; i < argc; ++i) {
    std::cout << "argv[" << i << "]: " << argv[i] << '\n';
  }
}
