/**
 * @file main.cpp
 * @brief Entry point of the arm program.
 */

#include <array>
#include <charconv>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <system_error>

#include "kinematics.hpp"  // brings joint_limits.hpp, and so clamp_joint, with it

/**
 * @brief Report where the tool of the three-joint planar arm ends up.
 *
 * Reads the three joint angles in degrees, shoulder first: from the command
 * line when three are given, otherwise by asking for them in the terminal.
 * Then prints one row per joint (the angle given, the angle after clamping to
 * that joint's limit, and that angle in radians) and the tool position.
 *
 *     ./week5_arm_demo 180 95 -10     # angles on the command line
 *     ./week5_arm_demo                # asks for the three angles
 *
 * @param argc The number of command-line arguments, including the program name.
 * @param argv The arguments themselves.
 * @return 0 on success, 1 when the arguments are the wrong count or an angle
 *         is not a number.
 */
int main(int argc, char* argv[]) {
    std::array<double, 3> deg{};

    if (argc == 1 + static_cast<int>(deg.size())) {
        for (std::size_t i{0}; i < deg.size(); ++i) {
            // from_chars reports a bad number through ec instead of throwing,
            // and ptr shows whether the whole argument was read.
            const std::string_view arg{argv[i + 1]};
            const char* end{arg.data() + arg.size()};
            const std::from_chars_result result{std::from_chars(arg.data(), end, deg[i])};
            if (result.ec != std::errc{} || result.ptr != end) {
                std::cerr << "not a number: " << arg << '\n';
                return 1;
            }
        }
    } else if (argc == 1) {
        for (std::size_t i{0}; i < deg.size(); ++i) {
            std::cout << "Enter angle " << i + 1 << " (deg): ";
            if (!(std::cin >> deg[i])) {  // the read fails on text that is not a number
                std::cerr << "not a number\n";
                return 1;
            }
        }
    } else {
        std::cerr << "usage: " << argv[0] << " [q1_deg q2_deg q3_deg]\n";
        return 1;
    }

    // Step 1 and 2: each angle as given, clamped to its joint's limit, and in
    // radians.
    const std::array<double, 3> limit{shoulder_max_deg, elbow_max_deg, wrist_max_deg};
    std::array<double, 3> clamped{};
    std::cout << "joint   input deg   clamped deg   radians\n" << std::fixed;
    for (std::size_t i{0}; i < deg.size(); ++i) {
        clamped[i] = clamp_joint(deg[i], limit[i]);
        std::cout << "q" << i + 1 << std::setprecision(1) << std::setw(14) << deg[i]
                  << std::setw(14) << clamped[i] << std::setprecision(3) << std::setw(10)
                  << convert_deg_to_rad(clamped[i]) << '\n';
    }

    // Step 3: where the tool ends up, from the clamped angles.
    double x{};
    double y{};
    forward_kinematics(clamped[0], clamped[1], clamped[2], x, y);
    print_pose(x, y);

    return 0;
}
