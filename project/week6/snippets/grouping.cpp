/**
 * @file grouping.cpp
 * @brief L6 Section 1, Grouping Values: the code of every slide, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week6_grouping. This file stands alone: it includes
 * no project header.
 *
 * @par How to use it
 * @code
 * 702build week6_grouping
 * 702run week6_grouping       # every slide of the section, in order
 * 702run week6_grouping 7    # only [Slide 7]
 * @endcode
 * The number is the frame number in the slide's top-left corner. Each slide's
 * code sits in its own namespace, so two slides can both declare a
 * @c RobotStatus without a clash. The comment above a namespace names its
 * slide, and @c main() at the bottom runs them.
 *
 * Code that does not compile, or that warns, is commented out where its slide
 * shows it. Uncomment one line, build, and you get the slide's message.
 */
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// [Slide 7] Three Values, One Status
namespace three_values {
// Without a struct: three reference parameters carry the answer back.
void get_status(int id, double &battery_pct, double &x, double &y) {
    if (id == 3) {
        battery_pct = 64.0;
        x = 2.0;
        y = 3.0;
    } else {
        battery_pct = 0.0;
        x = 0.0;
        y = 0.0;
    }
}

// With a struct: one return value carries all three.
struct RobotStatus {
    double battery_pct;
    double x;
    double y;
};

RobotStatus get_status(int id) {
    if (id == 3) {
        return {64.0, 2.0, 3.0};
    }
    return {0.0, 0.0, 0.0};
}

void run() {
    double battery_pct{};
    double x{};
    double y{};
    get_status(3, battery_pct, x, y);
    std::cout << "battery_pct: " << battery_pct << ", x: " << x << ", y: " << y << '\n'; // 64 2 3

    RobotStatus status{get_status(3)};
    std::cout << "status.battery_pct: " << status.battery_pct << ", status.x: " << status.x << ", status.y: " << status.y
              << '\n'; // 64 2 3
}
} // namespace three_values

// [Slide 8] Declaring a struct
namespace declaring {
struct Position {
    double x; // meters, warehouse frame
    double y;
};

struct RobotStatus {
    int id;
    double battery_pct;
    Position position; // a struct inside a struct
    bool busy;
};

void run() {
    RobotStatus robot_status{3, 64.0, {2.0, 3.0}, false};
    std::cout << "robot_status.id: " << robot_status.id << ", robot_status.battery_pct: " << robot_status.battery_pct << '\n'; // 3 64
}
} // namespace declaring

// [Slide 9] Where a struct Goes
namespace where_struct_goes {
struct RobotStatus; // declared, not defined

void run() {
    // Does not compile: an object needs the size.
    // RobotStatus robot_status{};
    std::cout << "does not compile: uncomment the line in where_struct_goes::run()\n";
}
} // namespace where_struct_goes

// [Slide 10] Initializing a struct
namespace aggregate_init {
struct Position {
    double x;
    double y;
};

struct RobotStatus {
    int id;
    double battery_pct;
    Position position;
    bool busy;
};

void print(const RobotStatus &robot_status) {
    std::cout << "id: " << robot_status.id << ", battery_pct: " << robot_status.battery_pct << ", position: ("
              << robot_status.position.x << ", " << robot_status.position.y << "), " << (robot_status.busy ? "busy" : "idle")
              << '\n';
}

void run() {
    RobotStatus robot_3{3, 64.0, {2.0, 3.0}, false};
    // Warns under -Wextra: missing initializer for member 'battery_pct'.
    // RobotStatus id_only{3};
    RobotStatus all_zero{};
    // Garbage: reading d is undefined behavior.
    // RobotStatus no_braces;
    print(robot_3); // 3 64 (2, 3) idle
    print(all_zero); // 0 0 (0, 0) idle
}
} // namespace aggregate_init

// [Slide 11] Default Member Initializers
namespace default_members {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0}; // a new robot starts charged
    Position position{};
    bool busy{false};
};

void print(const RobotStatus &robot_status) {
    std::cout << "id: " << robot_status.id << ", battery_pct: " << robot_status.battery_pct << ", position: ("
              << robot_status.position.x << ", " << robot_status.position.y << "), " << (robot_status.busy ? "busy" : "idle")
              << '\n';
}

void run() {
    RobotStatus new_robot{};
    RobotStatus robot_4{4, 18.0};
    RobotStatus robot_2{2, 35.0, {4.0, 1.0}, true};
    RobotStatus no_braces; // no braces: the defaults still apply
    print(new_robot);      // 0 100 (0, 0) idle
    print(robot_4);      // 4 18 (0, 0) idle
    print(robot_2);      // 2 35 (4, 1) busy
    print(no_braces);      // 0 100 (0, 0) idle
}
} // namespace default_members

// [Slide 12] Designated Initializers (C++20)
namespace designated {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0};
    Position position{};
    bool busy{false};
};

void print(const RobotStatus &robot_status) {
    std::cout << "id: " << robot_status.id << ", battery_pct: " << robot_status.battery_pct << ", position: ("
              << robot_status.position.x << ", " << robot_status.position.y << "), " << (robot_status.busy ? "busy" : "idle")
              << '\n';
}

void run() {
    RobotStatus robot_1{.id = 1, .battery_pct = 82.5}; // idle at (0, 0)
    RobotStatus robot_2{.id = 2, .busy = true};        // battery 100
    // Does not compile: wrong order.
    // RobotStatus robot_5{.battery_pct = 50.0, .id = 5};
    print(robot_1); // 1 82.5 (0, 0) idle
    print(robot_2); // 2 100 (0, 0) busy
}
} // namespace designated

// [Slide 13] Member Access
namespace member_access {
struct Position {
    double x;
    double y;
};

struct RobotStatus {
    int id;
    double battery_pct;
    Position position;
    bool busy;
};

void run() {
    RobotStatus robot_status{3, 64.0, {2.0, 3.0}, false};
    std::cout << "robot_status.id: " << robot_status.id << ", robot_status.position.x: " << robot_status.position.x << '\n'; // 3 2

    RobotStatus *status_ptr{&robot_status};
    status_ptr->battery_pct -= 10.0; // the same as (*status_ptr).battery_pct -= 10.0;
    std::cout << "robot_status.battery_pct: " << robot_status.battery_pct << '\n'; // 54
}
} // namespace member_access

// [Slide 14] A struct in Memory
namespace padding {
struct Position {
    double x;
    double y;
};

struct RobotStatus {
    int id;             // 4 bytes
    double battery_pct; // 8
    Position position;  // 16: two doubles
    bool busy;          // 1
};

void run() {
    std::cout << "sizeof int: " << sizeof(int) << ", double: " << sizeof(double)
              << ", Position: " << sizeof(Position) << ", bool: " << sizeof(bool)
              << '\n'; // 4 8 16 1
    std::cout << "sizeof(RobotStatus): " << sizeof(RobotStatus) << '\n'; // 40
    std::cout << "alignof(double): " << alignof(double) << '\n';         // 8
    std::cout << "offsetof id: " << offsetof(RobotStatus, id)
              << ", battery_pct: " << offsetof(RobotStatus, battery_pct)
              << ", position: " << offsetof(RobotStatus, position)
              << ", busy: " << offsetof(RobotStatus, busy) << '\n'; // 0 8 16 32
}
} // namespace padding

// [Slide 16] Member Order
namespace member_order {
struct Position {
    double x;
    double y;
};

struct RobotStatusSorted {
    double battery_pct;
    Position position;
    int id;
    bool busy;
};

void run() {
    std::cout << "sizeof(RobotStatusSorted): " << sizeof(RobotStatusSorted) << '\n'; // 32
    std::cout << "offsetof battery_pct: " << offsetof(RobotStatusSorted, battery_pct)
              << ", position: " << offsetof(RobotStatusSorted, position)
              << ", id: " << offsetof(RobotStatusSorted, id)
              << ", busy: " << offsetof(RobotStatusSorted, busy) << '\n'; // 0 8 24 28
}
} // namespace member_order

// [Slide 17] Passing a struct
namespace struct_functions {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0};
    Position position{};
    bool busy{false};
};

// The slide's other choice, by value. Both cannot exist at once: a call
// would be ambiguous.
// double get_battery(RobotStatus robot_status);  // copies 40 bytes

double get_battery(const RobotStatus &robot_status) { // copies nothing
    return robot_status.battery_pct;
}

RobotStatus make_new_robot(int id) { // returns by value
    return {id, 100.0, {0.0, 0.0}, false};
}

void run() {
    std::cout << "sizeof(RobotStatus): " << sizeof(RobotStatus) << '\n'; // 40
    std::cout << "get_battery(make_new_robot(5)): " << get_battery(make_new_robot(5))
              << '\n'; // 100
}
} // namespace struct_functions

// [Slide 18] A Vector of RobotStatus
namespace vector_of_structs {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0};
    Position position{};
    bool busy{false};
};

void run() {
    std::vector<RobotStatus> fleet{{1, 82.5, {0.0, 0.0}, false},
                                   {2, 35.0, {4.0, 1.0}, true},
                                   {3, 64.0, {2.0, 3.0}, false},
                                   {4, 18.0, {6.0, 2.0}, false}};
    for (const auto &robot : fleet) {
        std::cout << "robot " << robot.id << ": " << robot.battery_pct << " %"
                  << (robot.busy ? ", busy\n" : ", idle\n");
    }
}
} // namespace vector_of_structs

// [Slide 19] push_back and emplace_back
namespace push_emplace {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0};
    Position position{};
    bool busy{false};
};

void run() {
    std::vector<RobotStatus> fleet{};
    // push_back takes a whole RobotStatus: build one, then it goes in
    fleet.push_back(RobotStatus{1, 82.5, {0.0, 0.0}, false});
    // braces build it too: 2 fits int id
    fleet.push_back({2, 35.0, {4.0, 1.0}, true});
    // emplace_back builds it inside the vector, from the arguments (C++20)
    fleet.emplace_back(3, 64.0, Position{2.0, 3.0}, false);
    fleet.emplace_back(4, 18.0); // position and busy: defaults
    for (const auto &robot : fleet) {
        std::cout << "robot " << robot.id << ": " << robot.battery_pct << " % at (" << robot.position.x << ", "
                  << robot.position.y << ")" << (robot.busy ? ", busy\n" : ", idle\n");
    }
}
} // namespace push_emplace

// [Slide 20] Braces and emplace_back
namespace emplace_braces {
struct Position {
    double x{0.0};
    double y{0.0};
};

struct RobotStatus {
    int id{0};
    double battery_pct{100.0};
    Position position{};
    bool busy{false};
};

void run() {
    std::vector<RobotStatus> fleet{};
    // Does not compile: a braced list has no type.
    // fleet.emplace_back({5, 90.0, {1.0, 1.0}, false});
    // fleet.emplace_back(5, 90.0, {1.0, 1.0}, false);
    fleet.emplace_back(5, 90.0, Position{1.0, 1.0}, false); // OK
    fleet.emplace_back(4.9, 50.0);                          // OK, and the id is 4
    // Does not compile: 4.9 cannot narrow into int id.
    // fleet.push_back({4.9, 50.0});
    std::cout << "fleet[0].id: " << fleet[0].id << ", fleet[1].id: " << fleet[1].id << '\n'; // 5 4
}
} // namespace emplace_braces

// [Slide 21] std::pair
namespace pair_basics {
void run() {
    std::pair<int, double> reading{3, 64.0}; // robot 3, battery 64 %
    std::cout << "reading.first: " << reading.first
              << ", reading.second: " << reading.second << '\n'; // 3 64
    reading.second = 60.0; // first and second are public
    std::cout << "reading.second: " << reading.second << '\n'; // 60
}
} // namespace pair_basics

// [Slide 22] std::tuple
namespace tuple_basics {
void run() {
    std::tuple<int, double, bool> status{3, 64.0, false}; // id, battery, busy
    std::cout << "std::get<0>(status): " << std::get<0>(status)
              << ", std::get<1>(status): " << std::get<1>(status)
              << ", std::get<2>(status): " << std::get<2>(status) << '\n'; // 3 64 0
    std::get<1>(status) = 60.0;
    std::cout << "std::get<1>(status): " << std::get<1>(status) << '\n'; // 60
    // Does not compile: tuple index must be in range.
    // std::cout << std::get<3>(status) << '\n';
}
} // namespace tuple_basics

// Runs one slide's code: always when only is 0, otherwise only on a match.
// run is a pointer to a function: see the appendix, Function Pointers.
void show(int only, int slide, const char *title, void (*run)()) {
    if (only != 0 && only != slide) {
        return;
    }
    const std::string header{"[Slide " + std::to_string(slide) + "] " + title};
    const std::string rule(header.size(), '-'); // ( ), not { }: { } means a list of two chars
    std::cout << rule << '\n' << header << '\n' << rule << '\n';
    run();
}

int main(int argc, char *argv[]) {
    const int only{argc > 1 ? std::atoi(argv[1]) : 0};
    show(only, 7, "Three Values, One Status", three_values::run);
    show(only, 8, "Declaring a struct", declaring::run);
    show(only, 9, "Where a struct Goes", where_struct_goes::run);
    show(only, 10, "Initializing a struct", aggregate_init::run);
    show(only, 11, "Default Member Initializers", default_members::run);
    show(only, 12, "Designated Initializers (C++20)", designated::run);
    show(only, 13, "Member Access", member_access::run);
    show(only, 14, "A struct in Memory", padding::run);
    show(only, 16, "Member Order", member_order::run);
    show(only, 17, "Passing a struct", struct_functions::run);
    show(only, 18, "A Vector of RobotStatus", vector_of_structs::run);
    show(only, 19, "push_back and emplace_back", push_emplace::run);
    show(only, 20, "Braces and emplace_back", emplace_braces::run);
    show(only, 21, "std::pair", pair_basics::run);
    show(only, 22, "std::tuple", tuple_basics::run);
}
