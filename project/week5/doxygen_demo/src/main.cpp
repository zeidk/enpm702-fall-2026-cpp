/**
 * @file main.cpp
 * @brief Entry point of the arm program.
 */

#include <array>
#include <cstddef>
#include <iostream>
#include <string>

#include "kinematics.hpp"

/**
 * @brief Report where the tool of the three-joint planar arm ends up.
 *
 * With three arguments they are the joint angles in degrees, shoulder first.
 * With none, the defaults below are used. Any other count is an error.
 *
 * @param argc The number of command-line arguments, including the program name.
 * @param argv The arguments themselves.
 * @return 0 on success, 1 when the wrong number of arguments was given.
 */
int main(int argc, char* argv[]) {
    std::array<double, 3> deg{0.0, 45.0, -30.0};  // used when no angles are given

    if (argc == 1 + static_cast<int>(deg.size())) {
        for (std::size_t i{0}; i < deg.size(); ++i) {
            deg[i] = std::stod(argv[i + 1]);
        }
    } else if (argc != 1) {
        std::cerr << "usage: " << argv[0] << " [q1_deg q2_deg q3_deg]\n";
        return 1;
    }

    double x{};
    double y{};
    forward_kinematics(deg[0], deg[1], deg[2], x, y);
    print_pose(x, y);

    return 0;
}
