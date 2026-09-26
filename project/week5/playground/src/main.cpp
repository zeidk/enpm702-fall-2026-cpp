// =============================================================================
// ENPM702 - Introductory Robot Programming
// Week 5 playground - L5: Functions
//
// Every code snippet from the L5 slides, COMMENTED OUT, so you can try them
// one at a time. This file stands alone: it includes nothing from the rest of
// week5 and nothing includes it.
//
// The complete, documented, multi-file version of the arm program is the
// OTHER half of week5, in ../../doxygen_demo/. That is what the Documenting
// Functions section builds.
//
// HOW TO USE IT
//   Uncomment one block, build, run, then comment it back and move on.
//   In VS Code: select the block and press Ctrl+/ (Cmd+/ on macOS).
//
//   [Slide N] is the frame number printed in the TOP-LEFT corner of the slide.
//
//   Snippets that are whole functions live at NAMESPACE SCOPE, above main().
//   Snippets that are statements live inside main(), each in its own { }
//   block so the same variable name can be reused from one to the next.
//   Where a block needs both, the comment says which other block to uncomment
//   with it.
//
// FOUR THINGS TO EXPECT
//   1. Some snippets share a name with another snippet, because the slides
//      show the same function written two ways. The comment on each says
//      which other block it clashes with. Uncomment one of them at a time.
//   2. Blocks marked "DOES NOT COMPILE" are on the slides to show you an
//      error. Uncomment them on purpose, read the message, comment them back.
//   3. Blocks marked "UNDEFINED BEHAVIOR" are the point of the lecture, not
//      accidents. They may crash, print garbage, or appear to work. Build
//      them with AddressSanitizer, which names the bug: uncomment the two
//      -fsanitize lines in project/week5/CMakeLists.txt.
//   4. Every number in the comments was measured on the course machine,
//      g++ 13.3 with -std=c++20, on 2026-09-26. Addresses differ on every
//      run and on every machine; only their ORDER is the point.
// =============================================================================

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <numbers>
#include <span>
#include <string>
#include <string_view>
#include <vector>

// Several snippets clamp a joint to the arm's limit, so the constant is here
// once rather than inside each of them.
constexpr double max_deg{170.0};

// =============================================================================
// NAMESPACE SCOPE
//
// A function cannot be defined inside another function, so every snippet that
// is a whole function lives here. Uncomment it together with the matching
// block inside main().
// =============================================================================

// #############################################################################
// SECTION: FUNCTIONS
// #############################################################################

// --- [Slide 6] With a function: the logic is written once -------------------
// Shares the name clamp_joint with [Slide 29] and [Slide 48].
// double clamp_joint(double deg) {
//   if (deg > max_deg) { return max_deg; }
//   if (deg < -max_deg) { return -max_deg; }
//   return deg;
// }

// --- [Slide 7] The four parts of a function ---------------------------------
// Shares the name convert_deg_to_rad with [Slide 8] and [Slide 30].
// double convert_deg_to_rad(double deg) {
//   return deg * std::numbers::pi / 180.0;  // C++20, <numbers>
// }

// --- [Slide 8] Function header against function body ------------------------
// Shares the name convert_deg_to_rad with [Slide 7] and [Slide 30].
// constexpr double convert_deg_to_rad(double deg)  // the header
// {
//   return deg * std::numbers::pi / 180.0;         // the body
// }

// --- [Slide 9] The signature includes the namespace -------------------------
// Inside a namespace, so this one does NOT collide. The signature is
// robot::convert_deg_to_rad(double): the name, the namespace, and the
// parameter types. That is what the LINKER uses, and what shows up in
// "undefined reference to `robot::convert_deg_to_rad(double)'".
// namespace robot {
// constexpr double convert_deg_to_rad(double deg) {
//   return deg * std::numbers::pi / 180.0;
// }
// }  // namespace robot

// --- [Slide 10] Parameters and arguments ------------------------------------
// void set_speed(double linear, double angular) {  // parameters
//   std::cout << linear << ' ' << angular << '\n';
// }

// --- [Slide 12] Declaration order: the broken version -----------------------
// DOES NOT COMPILE: 'print_limits' was not declared in this scope.
// void report_arm() {
//   std::cout << "arm: ";
//   print_limits();  // not seen yet
// }
//
// void print_limits() { std::cout << "170 deg\n"; }

// --- [Slide 12] Declaration order: the fix ----------------------------------
// One declaration above both definitions. Uncomment this OR the broken
// version above, not both.
// void print_limits();  // the promise
//
// void report_arm() {
//   std::cout << "arm: ";
//   print_limits();  // OK
// }
//
// void print_limits() { std::cout << "170 deg\n"; }

// --- [Slide 13] Two functions that call each other --------------------------
// No order works without a declaration, because each one needs the other.
// Uncomment a clamp_joint block ([Slide 6], [Slide 29] or [Slide 48]) too.
// void retry_move(double deg);  // the declaration that breaks the cycle
//
// void move_joint(double deg) {
//   if (deg > max_deg) {
//     retry_move(deg);
//     return;
//   }
//   std::cout << "driving to " << deg << '\n';
// }
//
// void retry_move(double deg) { move_joint(clamp_joint(deg)); }

// --- [Slide 14] A missing definition ----------------------------------------
// Uncomment this AND the call block in main(), with no clamp_joint block
// uncommented, and you get the slide's link error. The two stages, one at a
// time:
//   702w5
//   g++ -std=c++20 -c playground/src/main.cpp -o /tmp/main.o  # compiles
//   g++ /tmp/main.o -o /tmp/lecture5                          # undefined reference
// double clamp_joint(double deg);  // promised, never delivered

// --- [Slide 25] The flow of control through a call --------------------------
// Uncomment this OR the [Slide 12] blocks: they share print_limits.
// void print_limits() { std::cout << "170 deg\n"; }
//
// void report_arm() {
//   std::cout << "arm: ";
//   print_limits();
// }

// --- [Slide 26] The return statement ----------------------------------------
// void print_range(double m) {
//   if (m < 0.0) {
//     std::cout << "invalid\n";
//     return;  // leave early
//   }
//   std::cout << m << " m\n";
// }  // returns here otherwise
//
// int calculate_sum(int a, int b) {
//   int result{a + b};
//   return result;
// }

// --- [Slide 27] Missing returns --- UNDEFINED BEHAVIOR ----------------------
// get_sign(0) falls off the end. GCC warns even without -Wall:
//   warning: control reaches end of non-void function [-Wreturn-type]
// int get_sign(int number) {
//   if (number > 0) {
//     return 1;
//   } else if (number < 0) {
//     return -1;
//   }
// }  // number == 0 falls off the end

// --- [Slide 28] Conversion on return ----------------------------------------
// Silent under -Wall -Wextra -pedantic-errors. Add -Wconversion to see:
//   warning: conversion from 'double' to 'int' may change value
// int truncate_value() {
//   double value{99.99};
//   return value;  // converted to int: 99
// }

// --- [Slide 29] [[nodiscard]] -----------------------------------------------
// Shares the name clamp_joint with [Slide 6] and [Slide 48].
// [[nodiscard]] double clamp_joint(double deg) {
//   return std::clamp(deg, -max_deg, max_deg);
// }

// --- [Slide 30] [Slide 31] constexpr: allowed at compile time --------------
// constexpr ALLOWS the compiler to run the call while compiling. It does not
// require it. Shares the name convert_deg_to_rad with [Slide 7] and [Slide 8].
// constexpr double convert_deg_to_rad(double deg) {
//   return deg * std::numbers::pi / 180.0;
// }

// --- [Slide 32] consteval: required at compile time -------------------------
// consteval REQUIRES it. Every call needs constant arguments.
// consteval int steps_for(int deg) { return deg * 10; }  // C++20

// #############################################################################
// SECTION: PASSING ARGUMENTS
// #############################################################################

// --- [Slide 35] Pass by value -----------------------------------------------
// The parameter is a new object, initialized from the argument, exactly as
// if you had written double deg{q2}.
// void nudge_joint(double deg) {
//   deg += 10.0;  // changes the copy
// }

// --- [Slide 36] The cost of a copy ------------------------------------------
// void print_average(std::vector<double> angles) {  // a copy
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   std::cout << sum / static_cast<double>(angles.size()) << '\n';
// }

// --- [Slide 37] Pass by reference -------------------------------------------
// Uncomment this OR [Slide 35]: with both, nudge_joint(q2) is ambiguous.
// void nudge_joint(double& deg) {
//   deg += 10.0;  // changes the caller's q2
// }

// --- [Slide 38] A swap function: by value, then by reference ----------------
// Uncomment one at a time: together, swap_deg(q2, q3) is ambiguous.
// void swap_deg(double a, double b) {  // swaps two copies: does nothing
//   double tmp{a};
//   a = b;
//   b = tmp;
// }
//
// void swap_deg(double& a, double& b) {  // swaps the caller's variables
//   double tmp{a};
//   a = b;
//   b = tmp;
// }

// --- [Slide 39] Pass by const reference -------------------------------------
// No copy, and the function may only read. This is the default for anything
// bigger than a few words. Uncomment this OR [Slide 36].
// void print_average(const std::vector<double>& angles) {  // no copy
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   // angles.push_back(0.0);  // error: discards qualifiers
//   std::cout << sum / static_cast<double>(angles.size()) << '\n';
// }

// --- [Slide 40] String parameters -------------------------------------------
// A std::string_view accepts a std::string or a literal, and copies neither.
// void log_joint(std::string_view name) {  // C++17, <string_view>
//   std::cout << name << '\n';
// }

// --- [Slide 41] [Slide 43] std::span ----------------------------------------
// A view of a contiguous sequence: a pointer and a count, 16 bytes, whether
// the sequence holds 2 elements or 2 million.
// double average_angle(std::span<const double> angles) {  // C++20, <span>
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   return sum / static_cast<double>(angles.size());
// }

// --- [Slide 46] Exercise 3: four calls --------------------------------------
// Write your answer down before you run it. GCC gives away one of the four:
// "parameter 'x' set but not used" on f1, because f1 changes nothing the
// caller can see.
// void f1(int x) { x = 99; }
// void f2(int& x) { x = 99; }
// void f3(int* p) { p = nullptr; }
// void f4(int* p) { *p = 99; }

// #############################################################################
// SECTION: RETURNING VALUES
// #############################################################################

// --- [Slide 48] Return by value ---------------------------------------------
// Shares the name clamp_joint with [Slide 6] and [Slide 29].
// On x86-64 a small result such as a double comes back in the register xmm0.
// double clamp_joint(double deg) {
//   return std::clamp(deg, -max_deg, max_deg);
// }

// --- [Slide 49] A large result ----------------------------------------------
// No copy is made: the vector is built directly in the caller's variable.
// std::vector<double> plan_path() {
//   std::vector<double> angles(1'000'000);
//   return angles;  // copy a million doubles? no.
// }

// --- [Slide 51] [Slide 52] Measured elision ---------------------------------
// Tracer prints a word when it is built, copied or moved. Writing a type like
// this is Lecture 6; today just use it.
// struct Tracer {
//   Tracer() { std::cout << "construct\n"; }
//   Tracer(const Tracer&) { std::cout << "copy\n"; }
//   Tracer(Tracer&&) noexcept { std::cout << "move\n"; }
//   Tracer& operator=(const Tracer&) { std::cout << "copy-assign\n"; return *this; }
//   Tracer& operator=(Tracer&&) noexcept { std::cout << "move-assign\n"; return *this; }
// };
//
// Tracer make_rvo() { return Tracer{}; }     // a temporary: always elided
// Tracer make_nrvo() { Tracer t; return t; }  // a named local: usually elided
//
// Tracer pick_ternary(bool first) {
//   Tracer x;
//   Tracer y;
//   return first ? x : y;  // not a plain name: COPIED
// }
//
// Tracer pick_branch(bool first) {
//   Tracer x;
//   Tracer y;
//   if (first) { return x; }  // a plain name: MOVED
//   return y;
// }

// --- [Slide 53] Return by reference -----------------------------------------
// Safe: the vector belongs to the caller and outlives the call.
// double& get_joint(std::vector<double>& q, std::size_t i) { return q.at(i); }

// --- [Slide 54] A search result ---------------------------------------------
// A pointer can say "nothing found" with nullptr. A reference cannot.
// double* find_value(std::vector<double>& v, double target) {
//   for (double& x : v) {
//     if (x == target) { return &x; }
//   }
//   return nullptr;  // not found
// }

// --- [Slide 55] Returning a local --- UNDEFINED BEHAVIOR --------------------
// warning: reference to local variable 'local_x' returned [-Wreturn-local-addr]
// double& tool_x() {
//   double local_x{0.42};
//   return local_x;
// }  // local_x is destroyed here

// --- [Slide 56] Returning a parameter --- UNDEFINED BEHAVIOR ----------------
// No local in sight, and it still dangles: the argument is a temporary
// std::string built for the call, and it dies at the end of that line.
// const std::string& pick_longer(const std::string& a, const std::string& b) {
//   return a.size() >= b.size() ? a : b;
// }

// #############################################################################
// SECTION: OVERLOADING AND DEFAULT ARGUMENTS
// #############################################################################

// --- [Slide 58] Function overloading ----------------------------------------
// Shares the name print_pose with [Slide 62].
// void print_pose(double x, double y) { std::cout << x << ' ' << y << '\n'; }
//
// void print_pose(double x, double y, double deg) {
//   std::cout << x << ' ' << y << ' ' << deg << '\n';
// }
//
// void print_pose(std::string_view name, double x, double y) {
//   std::cout << name << ": " << x << ' ' << y << '\n';
// }

// --- [Slide 59] Valid overloads: count, type, order -------------------------
// Any one of the three differences is enough.
// void move_joint(int id);
// void move_joint(int id, int deg);
// void move_joint(double deg);
// void move_joint(int id, double deg);
// void move_joint(double deg, int id);

// --- [Slide 59] Overloading on the return type --- DOES NOT COMPILE ---------
// error: ambiguating new declaration of 'double joint_count()'
// A call does not say which return type it wants.
// int joint_count() { return 3; }
// double joint_count() { return 3.0; }

// --- [Slide 61] Exercise 4: overload resolution -----------------------------
// int add(int a, int b) { return a + b; }
// int add(int a, float b) { return a + b; }
// int add(int a, double b) { return a + b; }

// --- [Slide 62] [Slide 63] Default arguments --------------------------------
// The default belongs in the DECLARATION, where callers can see it, and the
// definition must not repeat it. Add "= 3" to the definition below and you
// get: error: default argument given for parameter 3 of
// 'void print_pose(double, double, int, std::string_view)', even though the
// value is the same. Shares the name print_pose with [Slide 58].
// void print_pose(double x, double y, int precision = 3,
//                 std::string_view label = "tool");
//
// void print_pose(double x, double y, int precision, std::string_view label) {
//   std::cout << label << ' ' << x << ' ' << y << "  (" << precision << " dp)\n";
// }

// --- [Slide 64] Defaults against overloads ----------------------------------
// Two functions that differ only by a missing value are one function with a
// default. Two functions that do different work are overloads.
// void print_limits(double lo, double hi);        // different work
// void print_limits(double lo, double hi, int precision = 3);  // same work

// #############################################################################
// SECTION: THE CALL STACK
// #############################################################################

// --- [Slide 66] A call pushes a frame, a return pops it ---------------------
// void C() { }
// void B() { C(); }
// void A() { B(); }

// --- [Slide 74] The cost of a call ------------------------------------------
// Entering a function subtracts a size fixed at compile time from the stack
// pointer. Returning adds it back. One instruction each way.
// void g(int a) {
//   int b{a + 1};  // b lives in g's frame
//   std::cout << b << '\n';
// }                // b destroyed here
//
// void f() {
//   int x{10};     // x lives in f's frame
//   g(x);          // g's frame stacks on top of f's
// }                // x destroyed here

// --- [Slide 75] Frame addresses ---------------------------------------------
// Each address is 0x30 (48 bytes) BELOW the last: the stack grows downward.
// void descend(int depth) {
//   int local{depth};
//   std::cout << "depth " << depth << "  &local = " << &local << '\n';
//   if (depth < 3) { descend(depth + 1); }
// }

// --- [Slide 80] Exercise 5: a stack trace -----------------------------------
// Uncomment this OR [Slide 74]: they share the names f and g.
// Put a breakpoint on the line "x += y + *z;" and read the VS Code CALL STACK
// panel. gdb's bt prints the same three frames:
//   #0  f (x=@0x7fffffffc6e4: 30, y=10, z=0x7fffffffc6d8)
//   #1  in g (a=10, b=20)
//   #2  in main ()
// calls and scale are in no frame. Which Lecture 2 segment holds each one?
// constexpr int scale{2};
//
// void f(int& x, int y, int* z) {
//   static int calls{0};
//   ++calls;
//   x += y + *z;
// }
//
// int g(int a, int b) {
//   int result{};
//   result = a + b;
//   f(result, a, &b);
//   return result * scale;
// }

// --- [Slide 76] Static local variables --------------------------------------
// s is not in any frame. It sits in .bss, far below the stack address.
// int next_move_id() {
//   static int id{0};
//   return ++id;
// }
//
// void where() {
//   static int s{0};
//   int local{0};
//   std::cout << &s << ' ' << &local << '\n';
// }

// --- [Slide 78] One-time initialization -------------------------------------
// The INITIALIZER runs once. An ASSIGNMENT runs on every call.
// int count_calls() {
//   static int calls{0};  // once
//   ++calls;
//   return calls;  // 1, 2, 3
// }
//
// int count_calls_wrong() {
//   static int calls;
//   calls = 0;  // every call
//   ++calls;
//   return calls;  // always 1
// }

// --- [Slide 78] A table built once ------------------------------------------
// "building table" is printed once, however many lookups you do.
// std::vector<double> build_rad_table() {
//   std::cout << "building table\n";
//   std::vector<double> table(361);
//   for (int deg{0}; deg <= 360; ++deg) {
//     table[static_cast<std::size_t>(deg)] = deg * std::numbers::pi / 180.0;
//   }
//   return table;
// }
//
// double rad_of(int deg) {
//   static const std::vector<double> table{build_rad_table()};
//   return table.at(static_cast<std::size_t>(deg));
// }

// --- [Slide 83] [Slide 84] Recursion ----------------------------------------
// Four frames of compute_factorial are on the stack at once, each with its
// own n. Every call must move TOWARD the base case.
// long long compute_factorial(int n) {
//   if (n <= 1) {  // base case
//     return 1;
//   }
//   return n * compute_factorial(n - 1);
// }

// --- [Slide 85] Stack overflow --- UNDEFINED BEHAVIOR -----------------------
// No base case. At -O0 with the Linux default 8 MiB stack this crashed with
// "Segmentation fault" after about 520,000 calls: 8,388,608 bytes divided by
// a 16-byte frame is 524,288 frames.
// int depth{0};
//
// void dig() {
//   ++depth;
//   dig();  // no base case
// }

// --- [Slide 86] The cost of repeated work -----------------------------------
// compute_fibonacci(40) makes 331,160,281 calls and took 0.13 s at -O2.
// A loop does 40 additions in under a microsecond.
// long long compute_fibonacci(int n) {
//   if (n < 2) { return n; }
//   return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
// }

// --- [Slide 87] The same job as a loop --------------------------------------
// Uncomment this OR [Slide 83]: they share the name compute_factorial.
// long long compute_factorial(int n) {
//   long long result{1};
//   for (int i{2}; i <= n; ++i) { result *= i; }
//   return result;
// }

// --- [Slide 88] A folder tree: recursion that follows the data --------------
// namespace fs = std::filesystem;
//
// void print_tree(const fs::path& dir, int depth) {
//   for (const auto& entry : fs::directory_iterator(dir)) {
//     std::cout << std::string(2 * static_cast<std::size_t>(depth), ' ')
//               << entry.path().filename().string() << '\n';
//     if (entry.is_directory()) {
//       print_tree(entry.path(), depth + 1);
//     }
//   }
// }

// =============================================================================
// [Slide 90] Two forms of main
//
//   int main() { }
//   int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) { }
//
// The return type is always int. 0 means success; anything else is an error
// code the shell reads with "echo $?". main is the one function that may fall
// off its end: that counts as "return 0;". You may not call main yourself,
// overload it, or make it static.
// =============================================================================

/**
 * @brief Report where the tool of the three-joint planar arm ends up.
 * @param argc The number of command-line arguments, including the program name.
 * @param argv The arguments. With three of them, they are the joint angles in
 *             degrees; with none, the defaults inside are used.
 * @return 0 on success, 1 when the arguments are neither none nor three.
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // #########################################################################
    // SECTION: FUNCTIONS
    // #########################################################################

    // --- [Slide 6] Without a function: the logic is copied -------------------
    // The wrist block was copied and one q2 was never renamed. It compiles
    // with no warning, and the wrist stays at -250.
    // {
    //     double q2{-200.0};  // elbow
    //     if (q2 > max_deg) { q2 = max_deg; }
    //     if (q2 < -max_deg) { q2 = -max_deg; }
    //
    //     double q3{-250.0};  // wrist, copied
    //     if (q3 > max_deg) { q3 = max_deg; }
    //     if (q3 < -max_deg) { q2 = -max_deg; }  // the bug
    //
    //     std::cout << q2 << ' ' << q3 << '\n';  // -170 -250, not -170 -170
    // }

    // --- [Slide 6] With a function -------------------------------------------
    // {
    //     double q2{clamp_joint(-200.0)};  // elbow
    //     double q3{clamp_joint(-250.0)};  // wrist
    //     std::cout << q2 << ' ' << q3 << '\n';  // -170 -170
    // }

    // --- [Slide 10] Parameters and arguments ---------------------------------
    // Uncomment set_speed at namespace scope first.
    // {
    //     set_speed(0.5, 0.1);  // arguments: 0.5, 0.1
    // }

    // --- [Slide 12] [Slide 25] The flow of control through a call ------------
    // Uncomment one report_arm block at namespace scope first.
    // {
    //     report_arm();                   // prints "arm: 170 deg"
    //     std::cout << "exit main\n";
    // }

    // --- [Slide 13] Two functions that call each other -----------------------
    // Uncomment the [Slide 13] block at namespace scope first.
    // {
    //     move_joint(200.0);  // clamped to 170, then driven
    // }

    // --- [Slide 14] A missing definition -------------------------------------
    // {
    //     std::cout << clamp_joint(200.0) << '\n';  // 170
    // }

    // --- [Slide 26] The return statement -------------------------------------
    // Uncomment the [Slide 26] block at namespace scope first.
    // {
    //     print_range(-1.0);  // invalid
    //     print_range(2.5);   // 2.5 m
    //
    //     int sum{calculate_sum(5, 3)};  // 8
    //     std::cout << sum << '\n';
    // }

    // --- [Slide 27] Missing returns --- UNDEFINED BEHAVIOR -------------------
    // {
    //     std::cout << get_sign(5) << '\n';   // 1
    //     std::cout << get_sign(-5) << '\n';  // -1
    //     std::cout << get_sign(0) << '\n';   // whatever is left over
    // }

    // --- [Slide 28] Conversion on return -------------------------------------
    // {
    //     std::cout << truncate_value() << '\n';  // 99, the .99 is gone
    // }

    // --- [Slide 29] [[nodiscard]] --------------------------------------------
    // {
    //     clamp_joint(200.0);  // warning: ignoring return value
    //
    //     std::vector<double> v{1.0};
    //     v.empty();  // warning: empty() only ASKS, it empties nothing
    // }

    // --- [Slide 30] [Slide 31] constexpr, at compile time and at run time ----
    // Uncomment the [Slide 30] block at namespace scope first.
    // {
    //     constexpr double limit{convert_deg_to_rad(170.0)};  // at compile time
    //     std::cout << limit << '\n';                         // 2.967
    //
    //     double input{};
    //     std::cin >> input;
    //     double r{convert_deg_to_rad(input)};  // at run time
    //     std::cout << r << '\n';
    //
    //     double x{convert_deg_to_rad(170.0)};  // may run early, not required to
    //     std::cout << x << '\n';
    // }

    // --- [Slide 32] consteval -----------------------------------------------
    // Uncomment the [Slide 32] block at namespace scope first.
    // {
    //     std::array<int, steps_for(3)> plan{};  // 30 elements
    //     std::cout << plan.size() << '\n';
    //
    //     int n{3};  // holds 3, but is not constexpr
    //     // int bad{steps_for(n)};  // error: n is not a constant
    //     std::cout << n << '\n';
    // }

    // #########################################################################
    // SECTION: PASSING ARGUMENTS
    // #########################################################################

    // --- [Slide 35] Pass by value --------------------------------------------
    // Uncomment the [Slide 35] block at namespace scope first.
    // {
    //     double q2{5.0};
    //     nudge_joint(q2);
    //     std::cout << q2 << '\n';  // 5: the function changed its own copy
    // }

    // --- [Slide 36] The cost of a copy ---------------------------------------
    // Uncomment the [Slide 36] block at namespace scope first.
    // {
    //     std::vector<double> path(1'000'000, 1.0);
    //     print_average(path);  // 8 MB allocated and copied, silently
    // }

    // --- [Slide 37] Pass by reference ----------------------------------------
    // Uncomment the [Slide 37] block at namespace scope first.
    // {
    //     double q2{5.0};
    //     nudge_joint(q2);
    //     std::cout << q2 << '\n';  // 15: the caller's variable changed
    //
    //     // nudge_joint(5.0);  // error: no object to refer to
    // }

    // --- [Slide 38] A swap function ------------------------------------------
    // Uncomment one [Slide 38] block at namespace scope first.
    // {
    //     double q2{1.0};
    //     double q3{2.0};
    //     swap_deg(q2, q3);
    //     std::cout << q2 << ' ' << q3 << '\n';  // by value: 1 2. by ref: 2 1
    // }

    // --- [Slide 39] Pass by const reference ----------------------------------
    // Uncomment the [Slide 39] block at namespace scope first.
    // {
    //     std::vector<double> path(1'000'000, 1.0);
    //     print_average(path);        // no copy
    //     print_average({1.0, 2.0});  // a temporary is accepted too
    // }

    // --- [Slide 40] String parameters ----------------------------------------
    // Uncomment the [Slide 40] block at namespace scope first.
    // {
    //     std::string joint{"elbow"};
    //     log_joint(joint);    // views the string: no copy
    //     log_joint("elbow");  // views the literal: no std::string is built
    // }

    // --- [Slide 42] One parameter, three sequences ---------------------------
    // Uncomment the [Slide 41] block at namespace scope first.
    // {
    //     double c_array[]{1.0, 2.0, 3.0};
    //     std::array<double, 2> arr{4.0, 6.0};
    //     std::vector<double> vec{1.5, 2.5, 3.5, 4.5};
    //
    //     std::cout << average_angle(c_array) << '\n';  // 2
    //     std::cout << average_angle(arr) << '\n';      // 5
    //     std::cout << average_angle(vec) << '\n';      // 3
    // }

    // --- [Slide 46] Exercise 3: four calls -----------------------------------
    // Uncomment the [Slide 46] block at namespace scope first.
    // Write your answer down, then run it. Explain f3 in one sentence.
    // {
    //     int a{1};
    //     int b{1};
    //     int c{1};
    //     int d{1};
    //     f1(a);  f2(b);  f3(&c);  f4(&d);
    //     std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    // }

    // #########################################################################
    // SECTION: RETURNING VALUES
    // #########################################################################

    // --- [Slide 48] Return by value ------------------------------------------
    // {
    //     double q2{clamp_joint(200.0)};  // 170
    //     std::cout << q2 << '\n';
    // }

    // --- [Slide 49] A large result -------------------------------------------
    // Uncomment the [Slide 49] block at namespace scope first.
    // {
    //     std::vector<double> path{plan_path()};  // no copy is made
    //     std::cout << path.size() << '\n';       // 1000000
    // }

    // --- [Slide 51] Measured elision -----------------------------------------
    // Uncomment the [Slide 51] block at namespace scope first.
    // Default build prints "construct" twice: both are elided, even at -O0.
    // Rebuild with -fno-elide-constructors and make_nrvo prints
    // "construct" then "move": only the guaranteed elision survives.
    //   702w5
    //   g++ -std=c++20 -fno-elide-constructors playground/src/main.cpp -o /tmp/elide
    // {
    //     Tracer a{make_rvo()};
    //     Tracer b{make_nrvo()};
    //     (void)a;
    //     (void)b;
    // }

    // --- [Slide 52] Cases without elision ------------------------------------
    // {
    //     Tracer c{pick_ternary(true)};  // construct construct copy
    //     Tracer d{pick_branch(true)};   // construct construct move
    //
    //     Tracer e;        // already built
    //     e = make_rvo();  // construct, then move-assign
    //
    //     Tracer f{make_rvo()};  // construct only: elided
    //     (void)c; (void)d; (void)f;
    // }

    // --- [Slide 53] Return by reference --------------------------------------
    // Uncomment the [Slide 53] block at namespace scope first.
    // {
    //     std::vector<double> q{0.0, 0.5, 1.0};
    //     get_joint(q, 1) = 0.7;  // the call names q[1] itself
    //     std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1
    // }

    // --- [Slide 54] A search result ------------------------------------------
    // Uncomment the [Slide 54] block at namespace scope first.
    // {
    //     std::vector<double> q{0.0, 0.5, 1.0};
    //     double* p{find_value(q, 0.5)};
    //     if (p != nullptr) { *p = 0.7; }  // check it, every time
    //     std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1
    //
    //     std::cout << (find_value(q, 9.9) == nullptr) << '\n';  // 1: not found
    // }

    // --- [Slide 55] Returning a local --- UNDEFINED BEHAVIOR -----------------
    // Uncomment the [Slide 55] block at namespace scope first, and the two
    // -fsanitize lines in CMakeLists.txt.
    // {
    //     double& r{tool_x()};
    //     std::cout << r << '\n';  // undefined behavior
    // }

    // --- [Slide 56] Returning a parameter --- UNDEFINED BEHAVIOR -------------
    // Uncomment the [Slide 56] block at namespace scope first.
    // "camera" is not a std::string: the call builds a temporary one, and it
    // dies at the end of this line. AddressSanitizer says stack-use-after-scope.
    // {
    //     const std::string& r{pick_longer("lidar", "camera")};
    //     std::cout << r << '\n';  // undefined behavior
    // }

    // #########################################################################
    // SECTION: OVERLOADING AND DEFAULT ARGUMENTS
    // #########################################################################

    // --- [Slide 58] Function overloading -------------------------------------
    // Uncomment the [Slide 58] block at namespace scope first.
    // {
    //     print_pose(0.42, 1.17);           // the two-value version
    //     print_pose(0.42, 1.17, 30.0);     // the three-value version
    //     print_pose("wrist", 0.42, 1.17);  // the labelled version
    // }

    // --- [Slide 61] Exercise 4: overload resolution --------------------------
    // Uncomment the [Slide 61] block at namespace scope first.
    // For each line, name the version that is called and what it prints, or
    // say why it does not compile. Rank each argument with the table on
    // [Slide 60]. 'h' is 104.
    // {
    //     std::cout << add(2, 3) << '\n';        // 1
    //     std::cout << add(2, 3.5f) << '\n';     // 2
    //     std::cout << add(2.5, 3) << '\n';      // 3
    //     std::cout << add('h', false) << '\n';  // 4
    //     std::cout << add(2, 3L) << '\n';       // 5
    // }

    // --- [Slide 62] Default arguments ----------------------------------------
    // Uncomment the [Slide 62] block at namespace scope first.
    // Defaults fill from the RIGHT, and you cannot skip a middle one.
    // {
    //     print_pose(0.42, 1.17, 2, "wrist");  // 2 dp, wrist
    //     print_pose(0.42, 1.17, 2);           // 2 dp, tool
    //     print_pose(0.42, 1.17);              // 3 dp, tool
    //     // print_pose(0.42);                 // error: too few arguments
    // }

    // #########################################################################
    // SECTION: THE CALL STACK
    // #########################################################################

    // --- [Slide 66] A call pushes a frame, a return pops it ------------------
    // Uncomment the [Slide 66] block at namespace scope first, then step
    // through it in the debugger and watch the CALL STACK panel grow.
    // {
    //     A();
    // }

    // --- [Slide 74] The cost of a call ---------------------------------------
    // {
    //     f();
    // }

    // --- [Slide 75] Frame addresses ------------------------------------------
    // Each address is 48 bytes below the last. The values differ every run.
    // {
    //     descend(1);
    // }

    // --- [Slide 80] Exercise 5: a stack trace --------------------------------
    // Draw the stack at "x += y + *z;". Which variable does x name there?
    // It is NOT the x in main. Which variable does z point at?
    // {
    //     int x{10};
    //     int y{20};
    //     int z{};
    //     z = g(x, y);
    //     std::cout << z << '\n';  // 120
    // }

    // --- [Slide 76] Static local variables -----------------------------------
    // Uncomment the [Slide 76] block at namespace scope first.
    // {
    //     std::cout << next_move_id() << '\n';  // 1
    //     std::cout << next_move_id() << '\n';  // 2
    //     std::cout << next_move_id() << '\n';  // 3
    //
    //     where();  // a .bss address, then a much larger stack address
    // }

    // --- [Slide 78] One-time initialization ----------------------------------
    // {
    //     std::cout << count_calls() << count_calls() << count_calls() << '\n';
    //     std::cout << count_calls_wrong() << count_calls_wrong() << '\n';  // 11
    // }

    // --- [Slide 78] A table built once ---------------------------------------
    // "building table" is printed once, not three times.
    // {
    //     std::cout << rad_of(10) << '\n';
    //     std::cout << rad_of(20) << '\n';
    //     std::cout << rad_of(30) << '\n';
    // }

    // --- [Slide 83] Recursion ------------------------------------------------
    // {
    //     std::cout << compute_factorial(4) << '\n';  // 24
    //
    //     // A second limit, nothing to do with the stack: 21! does not fit in
    //     // a long long. Signed overflow is undefined behavior, from Lecture 2.
    //     std::cout << compute_factorial(21) << '\n';  // -4249290049419214848
    // }

    // --- [Slide 85] Stack overflow --- UNDEFINED BEHAVIOR --------------------
    // This CRASHES. That is the point. Run it last.
    // {
    //     dig();
    //     std::cout << depth << '\n';  // never reached
    // }

    // --- [Slide 86] The cost of repeated work --------------------------------
    // Time it: 702build && time 702run week5
    // {
    //     std::cout << compute_fibonacci(40) << '\n';  // 102334155
    // }

    // --- [Slide 88] A folder tree --------------------------------------------
    // Uncomment the [Slide 88] block at namespace scope first.
    // Run it from the week5 folder: 702w5 && 702run week5
    // {
    //     print_tree(fs::current_path(), 0);
    // }

    // #########################################################################
    // SECTION: THE main FUNCTION
    // #########################################################################

    // --- [Slide 91] Command-line arguments -----------------------------------
    // 702run week5 30 -45 60
    // {
    //     std::cout << "Number of arguments: " << argc << '\n';
    //     for (int i{0}; i < argc; ++i) {
    //         std::cout << "argv[" << i << "]: " << argv[i] << '\n';
    //     }
    //
    //     // argv[argc] is always a null pointer.
    //     std::cout << (argv[argc] == nullptr) << '\n';  // 1
    //
    //     // Two pointers are an iterator range, as Lecture 4 showed. Copy
    //     // them into something safer before you use them.
    //     std::vector<std::string_view> args(argv, argv + argc);
    //     std::cout << args.size() << '\n';
    // }

    return 0;
}
