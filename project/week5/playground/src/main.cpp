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
//   [Appendix: Title] is a frame in the appendix, after the summary. Those
//   frames print no number, so they are named by the title at their top.
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
// Shares the name clamp_joint with [Slide 28] and [Slide 42].
// double clamp_joint(double deg) {
//   if (deg > max_deg) { return max_deg; }
//   if (deg < -max_deg) { return -max_deg; }
//   return deg;
// }

// --- [Slide 7] The four parts of a function ---------------------------------
// Shares the name convert_deg_to_rad with [Slide 8] and
// [Appendix: constexpr Function].
// double convert_deg_to_rad(double deg) {
//   return deg * std::numbers::pi / 180.0;  // C++20, <numbers>
// }

// --- [Slide 8] Function header against function body ------------------------
// Shares the name convert_deg_to_rad with [Slide 7] and
// [Appendix: constexpr Function].
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

// --- [Slide 13] [Slide 14] Two functions that call each other ---------------
// No order works without a declaration, because each one needs the other.
// [Slide 14] puts every declaration at the top instead; either way compiles.
// Uncomment a clamp_joint block ([Slide 6], [Slide 28] or [Slide 42]) too.
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

// --- [Slide 15] A missing definition ----------------------------------------
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

// --- [Slide 28] [[nodiscard]] -----------------------------------------------
// Shares the name clamp_joint with [Slide 6] and [Slide 42].
// [[nodiscard]] double clamp_joint(double deg) {
//   return std::clamp(deg, -max_deg, max_deg);
// }

// --- [Appendix: Conversion on Return] ---------------------------------------
// Silent under -Wall -Wextra -pedantic-errors. Add -Wconversion to see:
//   warning: conversion from 'double' to 'int' may change value
// int truncate_value() {
//   double value{99.99};
//   return value;  // converted to int: 99
// }

// --- [Appendix: constexpr Function] ----------------------------------------
// constexpr ALLOWS the compiler to run the call while compiling. It does not
// require it. Shares the name convert_deg_to_rad with [Slide 7] and [Slide 8].
// constexpr double convert_deg_to_rad(double deg) {
//   return deg * std::numbers::pi / 180.0;
// }

// --- [Appendix: consteval Function] ----------------------------------------
// consteval REQUIRES it. Every call needs constant arguments.
// consteval int steps_for(int deg) { return deg * 10; }  // C++20

// #############################################################################
// SECTION: PASSING ARGUMENTS
// #############################################################################

// --- [Slide 31] Pass by value -----------------------------------------------
// The parameter is a new object, initialized from the argument, exactly as
// if you had written double deg{q2}.
// void nudge_joint(double deg) {
//   deg += 10.0;  // changes the copy
// }

// --- [Slide 32] The cost of a copy ------------------------------------------
// Shares the name average_angle with [Slide 35] and [Slide 37].
// double average_angle(std::vector<double> angles) {  // a copy
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   return sum / static_cast<double>(angles.size());
// }

// --- [Slide 33] Pass by reference -------------------------------------------
// Uncomment this OR [Slide 31]: with both, nudge_joint(q2) is ambiguous.
// void nudge_joint(double& deg) {
//   deg += 10.0;  // changes the caller's q2
// }

// --- [Slide 34] A swap function: by value, then by reference ----------------
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

// --- [Slide 35] Pass by const reference -------------------------------------
// No copy, and the function may only read. This is the default for anything
// bigger than a few words. Shares the name average_angle with [Slide 32] and
// [Slide 37]. Uncomment the push_back line to see the slide's error:
//   error: passing 'const std::vector<double>' as 'this' argument discards qualifiers
// double average_angle(const std::vector<double>& angles) {  // no copy
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   // angles.push_back(0.0);  // error: discards qualifiers
//   return sum / static_cast<double>(angles.size());
// }

// --- [Slide 36] String parameters -------------------------------------------
// A std::string_view accepts a std::string or a literal, and copies neither.
// void log_joint(std::string_view name) {  // C++17, <string_view>
//   std::cout << name << '\n';
// }

// --- [Slide 37] std::span ---------------------------------------------------
// A view of a contiguous sequence: a pointer and a count, 16 bytes, whether
// the sequence holds 2 elements or 2 million.
// [Appendix: One Parameter, Three Sequences] passes it three kinds of
// sequence. Shares the name average_angle with [Slide 32] and [Slide 35].
// double average_angle(std::span<const double> angles) {  // C++20, <span>
//   double sum{0.0};
//   for (double a : angles) { sum += a; }
//   return sum / static_cast<double>(angles.size());
// }

// --- [Slide 38] Pass by pointer ---------------------------------------------
// The caller passes an address, and nullptr means "no object". Check it before
// you dereference it. This overload does not clash with [Slide 31] or
// [Slide 33]: &q2 is a double*, so nudge_joint(&q2) can only pick this one.
// void nudge_joint(double* p) {  // double* p{&q2};
//   if (p != nullptr) {
//     *p += 10.0;  // changes q2
//   }
// }

// --- [Slide 40] Exercise 3: four calls --------------------------------------
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

// --- [Slide 42] Return by value ---------------------------------------------
// Shares the name clamp_joint with [Slide 6] and [Slide 28]. [Slide 11] shows
// the same function split into a declaration and a definition.
// On x86-64 a small result such as a double comes back in the register xmm0.
// double clamp_joint(double deg) {
//   return std::clamp(deg, -max_deg, max_deg);
// }

// --- [Slide 43] A large result ----------------------------------------------
// No copy is made: the vector is built directly in the caller's variable.
// [Slide 44] names the rule: copy elision.
// std::vector<double> plan_path() {
//   std::vector<double> angles(1'000'000);
//   // ... fill it, one step per millisecond ...
//   return angles;  // copy a million doubles? no.
// }

// --- [Appendix: Measured Elision] -------------------------------------------
// &v is the vector object. v.data() is its heap block. Compare the two lines
// that main() prints:
//   same object, same block   elided (the default, even at -O0)
//   new object, same block    moved
//   new block as well         copied
// Rebuild with -fno-elide-constructors and the object changes but the block
// does not: NRVO is off, and the named local is moved instead.
//   702w5
//   g++ -std=c++20 -fno-elide-constructors playground/src/main.cpp -o /tmp/elide
// std::vector<double> make_path() {
//   std::vector<double> v(1000);
//   std::cout << &v << ' ' << v.data() << '\n';
//   return v;  // a named local
// }

// --- [Appendix: Cases without Elision] Two candidates -----------------------
// Uncomment one pick at a time: they share a name. With two candidates NRVO
// is not done, and the implicit move needs a plain name. The first line of
// each prints x's heap block, so main() can tell a copy from a move. That
// line is not on the slide.
// std::vector<double> pick(bool first) {
//   std::vector<double> x(1000);
//   std::vector<double> y(1000);
//   std::cout << x.data() << '\n';
//   return first ? x : y;  // copy
// }
//
// std::vector<double> pick(bool first) {
//   std::vector<double> x(1000);
//   std::vector<double> y(1000);
//   std::cout << x.data() << '\n';
//   if (first) { return x; }  // move
//   return y;
// }

// --- [Slide 45] Return by reference -----------------------------------------
// Safe: the vector belongs to the caller and outlives the call.
// double& get_joint(std::vector<double>& q, std::size_t i) { return q.at(i); }

// --- [Slide 46] A search result ---------------------------------------------
// A pointer can say "nothing found" with nullptr. A reference cannot.
// double* find_value(std::vector<double>& v, double target) {
//   for (double& x : v) {
//     if (x == target) { return &x; }
//   }
//   return nullptr;  // not found
// }

// --- [Slide 47] Returning a local --- UNDEFINED BEHAVIOR --------------------
// warning: reference to local variable 'local_x' returned [-Wreturn-local-addr]
// double& tool_x() {
//   double local_x{0.42};
//   return local_x;
// }  // local_x is destroyed here

// --- [Slide 48] Returning a parameter --- UNDEFINED BEHAVIOR ----------------
// No local in sight, and it still dangles: the argument is a temporary
// std::string built for the call, and it dies at the end of that line.
// const std::string& pick_longer(const std::string& a, const std::string& b) {
//   return a.size() >= b.size() ? a : b;
// }

// #############################################################################
// SECTION: OVERLOADING AND DEFAULT ARGUMENTS
// #############################################################################

// --- [Slide 50] Function overloading ----------------------------------------
// Shares the name print_pose with [Slide 54].
// void print_pose(double x, double y) { std::cout << x << ' ' << y << '\n'; }
//
// void print_pose(double x, double y, double deg) {
//   std::cout << x << ' ' << y << ' ' << deg << '\n';
// }
//
// void print_pose(std::string_view name, double x, double y) {
//   std::cout << name << ": " << x << ' ' << y << '\n';
// }

// --- [Slide 51] Valid overloads: count, type, order -------------------------
// Any one of the three differences is enough.
// void move_joint(int id);
// void move_joint(int id, int deg);
// void move_joint(double deg);
// void move_joint(int id, double deg);
// void move_joint(double deg, int id);

// --- [Slide 51] Overloading on the return type --- DOES NOT COMPILE ---------
// error: ambiguating new declaration of 'double joint_count()'
// A call does not say which return type it wants.
// int joint_count() { return 3; }
// double joint_count() { return 3.0; }

// --- [Slide 53] Exercise 4: overload resolution -----------------------------
// int add(int a, int b) { return a + b; }
// int add(int a, float b) { return a + b; }
// int add(int a, double b) { return a + b; }

// --- [Slide 54] [Slide 55] Default arguments --------------------------------
// The default belongs in the DECLARATION, where callers can see it, and the
// definition must not repeat it. Add "= 3" to the definition below and you
// get: error: default argument given for parameter 3 of
// 'void print_pose(double, double, int, std::string_view)', even though the
// value is the same. Shares the name print_pose with [Slide 50].
// void print_pose(double x, double y, int precision = 3,
//                 std::string_view label = "tool");
//
// void print_pose(double x, double y, int precision, std::string_view label) {
//   std::cout << label << ' ' << x << ' ' << y << "  (" << precision << " dp)\n";
// }

// --- [Slide 56] Defaults versus overloads -----------------------------------
// Two functions that differ only by a missing value are one function with a
// default. Uncomment one of the two, not both: with both, print_pose(x, y)
// is ambiguous. Declarations only, so there is nothing to run.
//
// Two functions:
// void print_pose(double x, double y);
// void print_pose(double x, double y, int precision);
//
// One function:
// void print_pose(double x, double y, int precision = 3);

// #############################################################################
// SECTION: THE CALL STACK
// #############################################################################

// --- [Slide 58] A call pushes a frame, a return pops it ---------------------
// The appendix walks through the same program one call at a time, from
// [Appendix: Step 0: Inside main()] on, with a local in A and in B:
// void B() { int b{2}; C(); } and void A() { int a{1}; B(); }.
// void C() { }
// void B() { C(); }
// void A() { B(); }

// --- [Appendix: The Cost of a Call] -----------------------------------------
// Entering a function subtracts a size fixed at compile time from the stack
// pointer. Returning adds it back. One instruction each way.
// Uncomment this OR [Slide 62]: they share the names f and g.
// void g(int a) {
//   int b{a + 1};  // b lives in g's frame
//   std::cout << b << '\n';
// }                // b destroyed here
//
// void f() {
//   int x{10};     // x lives in f's frame
//   g(x);          // g's frame stacks on top of f's
// }                // x destroyed here

// --- [Appendix: Frame Addresses] --------------------------------------------
// Each address is 0x30 (48 bytes) BELOW the last: the stack grows downward.
// void descend(int depth) {
//   int local{depth};
//   std::cout << "depth " << depth << "  &local = " << &local << '\n';
//   if (depth < 3) { descend(depth + 1); }
// }

// --- [Slide 59] Static local variables --------------------------------------
// s is not in any frame. It sits in .bss, far below the stack address.
// [Slide 61] sums up which segment holds what.
// [Appendix: Five Common Uses] lists where a static local is worth it.
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

// --- [Slide 60] One-time initialization -------------------------------------
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

// --- [Slide 60] A table built once ------------------------------------------
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

// --- [Slide 62] Exercise 5: a stack trace -----------------------------------
// Uncomment this OR [Appendix: The Cost of a Call]: they share the names f
// and g. Put a breakpoint on the line "x += y + *z;" and read the VS Code
// CALL STACK panel. gdb's bt prints the same three frames:
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

// --- [Slide 63] [Slide 64] Recursion ----------------------------------------
// Four frames of compute_factorial are on the stack at once, each with its
// own n. Every call must move TOWARD the base case.
// long long compute_factorial(int n) {
//   if (n <= 1) {  // base case
//     return 1;
//   }
//   return n * compute_factorial(n - 1);
// }

// --- [Slide 65] Stack overflow --- UNDEFINED BEHAVIOR -----------------------
// No base case. At -O0 with the Linux default 8 MiB stack this crashed with
// "Segmentation fault" after about 520,000 calls: 8,388,608 bytes divided by
// a 16-byte frame is 524,288 frames.
// int depth{0};
//
// void dig() {
//   ++depth;
//   dig();  // no base case
// }

// --- [Slide 66] Recursion versus a loop -------------------------------------
// A walk along a sequence is a loop: the depth would be its length, and
// every call would cost a frame. A loop reuses one.
// double sum_readings(std::span<const double> readings) {
//   double total{0.0};
//   for (double r : readings) {
//     total += r;
//   }
//   return total;
// }

// --- [Appendix: The Costs of Recursion] -------------------------------------
// compute_fibonacci(40) makes 331,160,281 calls and took 0.13 s at -O2.
// A loop does 40 additions in under a microsecond.
// long long compute_fibonacci(int n) {
//   if (n < 2) { return n; }
//   return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
// }

// --- [Appendix: A Folder Tree] recursion that follows the data --------------
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
// [Slide 68] Two forms of main
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

    // --- [Slide 15] A missing definition -------------------------------------
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

    // --- [Slide 28] [[nodiscard]] --------------------------------------------
    // {
    //     clamp_joint(200.0);  // warning: ignoring return value
    //
    //     std::vector<double> v{1.0};
    //     v.empty();  // warning: empty() only ASKS, it empties nothing
    // }

    // --- [Appendix: Conversion on Return] ------------------------------------
    // {
    //     std::cout << truncate_value() << '\n';  // 99, the .99 is gone
    // }

    // --- [Appendix: constexpr Function] -------------------------------------
    // Uncomment the constexpr block at namespace scope first.
    // [Appendix: The Same Function at Run Time] is the second half.
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

    // --- [Appendix: consteval Function] -------------------------------------
    // Uncomment the consteval block at namespace scope first.
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

    // --- [Slide 31] Pass by value --------------------------------------------
    // Uncomment the [Slide 31] block at namespace scope first.
    // {
    //     double q2{5.0};
    //     nudge_joint(q2);
    //     std::cout << q2 << '\n';  // 5: the function changed its own copy
    // }

    // --- [Slide 32] The cost of a copy ---------------------------------------
    // Uncomment the [Slide 32] block at namespace scope first.
    // {
    //     std::vector<double> path(1'000'000, 1.0);
    //     std::cout << average_angle(path) << '\n';  // 8 MB allocated and copied
    // }

    // --- [Slide 33] Pass by reference ----------------------------------------
    // Uncomment the [Slide 33] block at namespace scope first.
    // {
    //     double q2{5.0};
    //     nudge_joint(q2);
    //     std::cout << q2 << '\n';  // 15: the caller's variable changed
    //
    //     // nudge_joint(5.0);  // error: no object to refer to
    // }

    // --- [Slide 34] A swap function ------------------------------------------
    // Uncomment one [Slide 34] block at namespace scope first.
    // {
    //     double q2{1.0};
    //     double q3{2.0};
    //     swap_deg(q2, q3);
    //     std::cout << q2 << ' ' << q3 << '\n';  // by value: 1 2. by ref: 2 1
    // }

    // --- [Slide 35] Pass by const reference ----------------------------------
    // Uncomment the [Slide 35] block at namespace scope first.
    // {
    //     std::vector<double> path(1'000'000, 1.0);
    //     std::cout << average_angle(path) << '\n';        // no copy
    //     std::cout << average_angle({1.0, 2.0}) << '\n';  // a temporary is accepted too
    // }

    // --- [Slide 36] String parameters ----------------------------------------
    // Uncomment the [Slide 36] block at namespace scope first.
    // {
    //     std::string joint{"elbow"};
    //     log_joint(joint);    // views the string: no copy
    //     log_joint("elbow");  // views the literal: no std::string is built
    // }

    // --- [Appendix: One Parameter, Three Sequences] --------------------------
    // Uncomment the [Slide 37] block at namespace scope first.
    // [Appendix: A Span in Memory] draws what angles holds for arr.
    // {
    //     double c_array[]{1.0, 2.0, 3.0};
    //     std::array<double, 2> arr{4.0, 6.0};
    //     std::vector<double> vec{1.5, 2.5, 3.5, 4.5};
    //
    //     std::cout << average_angle(c_array) << '\n';  // 2
    //     std::cout << average_angle(arr) << '\n';      // 5
    //     std::cout << average_angle(vec) << '\n';      // 3
    // }

    // --- [Slide 38] Pass by pointer ------------------------------------------
    // Uncomment the [Slide 38] block at namespace scope first.
    // {
    //     double q2{5.0};
    //     nudge_joint(&q2);
    //     std::cout << q2 << '\n';  // 15
    //
    //     nudge_joint(nullptr);  // does nothing
    // }

    // --- [Slide 40] Exercise 3: four calls -----------------------------------
    // Uncomment the [Slide 40] block at namespace scope first.
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

    // --- [Slide 42] Return by value ------------------------------------------
    // {
    //     double q2{clamp_joint(200.0)};  // 170
    //     std::cout << q2 << '\n';
    // }

    // --- [Slide 43] A large result -------------------------------------------
    // Uncomment the [Slide 43] block at namespace scope first.
    // {
    //     std::vector<double> path{plan_path()};  // no copy is made
    //     std::cout << path.size() << '\n';       // 1000000
    // }

    // --- [Appendix: Measured Elision] ----------------------------------------
    // Uncomment make_path at namespace scope first.
    // Default build: the two lines are identical. With -fno-elide-constructors
    // the first address differs and the second does not: a move.
    // {
    //     std::vector<double> path{make_path()};
    //     std::cout << &path << ' ' << path.data() << '\n';
    // }

    // --- [Appendix: Cases without Elision] -----------------------------------
    // Uncomment one pick, and make_path, at namespace scope first.
    // The ternary pick prints two DIFFERENT blocks: a copy. The if pick prints
    // the SAME block twice: a move.
    // {
    //     std::vector<double> c{pick(true)};
    //     std::cout << c.data() << '\n';
    //
    //     std::vector<double> e;  // e exists: move-assign
    //     e = make_path();
    //
    //     std::vector<double> f{make_path()};  // f is new: elided
    // }

    // --- [Slide 45] Return by reference --------------------------------------
    // Uncomment the [Slide 45] block at namespace scope first.
    // {
    //     std::vector<double> q{0.0, 0.5, 1.0};
    //     get_joint(q, 1) = 0.7;  // the call names q[1] itself
    //     std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1
    // }

    // --- [Slide 46] A search result ------------------------------------------
    // Uncomment the [Slide 46] block at namespace scope first.
    // {
    //     std::vector<double> q{0.0, 0.5, 1.0};
    //     double* p{find_value(q, 0.5)};
    //     if (p != nullptr) { *p = 0.7; }  // check it, every time
    //     std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1
    //
    //     std::cout << (find_value(q, 9.9) == nullptr) << '\n';  // 1: not found
    // }

    // --- [Slide 47] Returning a local --- UNDEFINED BEHAVIOR -----------------
    // Uncomment the [Slide 47] block at namespace scope first, and the two
    // -fsanitize lines in CMakeLists.txt.
    // {
    //     double& r{tool_x()};
    //     std::cout << r << '\n';  // undefined behavior
    // }

    // --- [Slide 48] Returning a parameter --- UNDEFINED BEHAVIOR -------------
    // Uncomment the [Slide 48] block at namespace scope first.
    // "camera" is not a std::string: the call builds a temporary one, and it
    // dies at the end of this line. AddressSanitizer says stack-use-after-scope.
    // {
    //     const std::string& r{pick_longer("lidar", "camera")};
    //     std::cout << r << '\n';  // undefined behavior
    // }

    // #########################################################################
    // SECTION: OVERLOADING AND DEFAULT ARGUMENTS
    // #########################################################################

    // --- [Slide 50] Function overloading -------------------------------------
    // Uncomment the [Slide 50] block at namespace scope first.
    // {
    //     print_pose(0.42, 1.17);           // the two-value version
    //     print_pose(0.42, 1.17, 30.0);     // the three-value version
    //     print_pose("wrist", 0.42, 1.17);  // the labelled version
    // }

    // --- [Slide 53] Exercise 4: overload resolution --------------------------
    // Uncomment the [Slide 53] block at namespace scope first.
    // For each line, name the version that is called and what it prints, or
    // say why it does not compile. Rank each argument with the table on
    // [Slide 52]. 'h' is 104.
    // {
    //     std::cout << add(2, 3) << '\n';        // 1
    //     std::cout << add(2, 3.5f) << '\n';     // 2
    //     std::cout << add(2.5, 3) << '\n';      // 3
    //     std::cout << add('h', false) << '\n';  // 4
    //     std::cout << add(2, 3L) << '\n';       // 5
    // }

    // --- [Slide 54] Default arguments ----------------------------------------
    // Uncomment the [Slide 54] block at namespace scope first.
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

    // --- [Slide 58] A call pushes a frame, a return pops it ------------------
    // Uncomment the [Slide 58] block at namespace scope first, then step
    // through it in the debugger and watch the CALL STACK panel grow.
    // {
    //     A();
    // }

    // --- [Appendix: The Cost of a Call] --------------------------------------
    // {
    //     f();
    // }

    // --- [Appendix: Frame Addresses] -----------------------------------------
    // Each address is 48 bytes below the last. The values differ every run.
    // {
    //     descend(1);
    // }

    // --- [Slide 59] Static local variables -----------------------------------
    // Uncomment the [Slide 59] block at namespace scope first.
    // {
    //     std::cout << next_move_id() << '\n';  // 1
    //     std::cout << next_move_id() << '\n';  // 2
    //     std::cout << next_move_id() << '\n';  // 3
    //
    //     where();  // a .bss address, then a much larger stack address
    // }

    // --- [Slide 60] One-time initialization ----------------------------------
    // {
    //     std::cout << count_calls() << count_calls() << count_calls() << '\n';
    //     std::cout << count_calls_wrong() << count_calls_wrong() << '\n';  // 11
    // }

    // --- [Slide 60] A table built once ---------------------------------------
    // "building table" is printed once, not three times.
    // {
    //     std::cout << rad_of(10) << '\n';
    //     std::cout << rad_of(20) << '\n';
    //     std::cout << rad_of(30) << '\n';
    // }

    // --- [Slide 62] Exercise 5: a stack trace --------------------------------
    // Draw the stack at "x += y + *z;". Which variable does x name there?
    // It is NOT the x in main. Which variable does z point at?
    // {
    //     int x{10};
    //     int y{20};
    //     int z{};
    //     z = g(x, y);
    //     std::cout << z << '\n';  // 120
    // }

    // --- [Slide 63] [Slide 64] Recursion -------------------------------------
    // {
    //     std::cout << compute_factorial(4) << '\n';  // 24
    //
    //     // [Slide 65]: a second limit, nothing to do with the stack. 21! does
    //     // not fit in a long long. Signed overflow is undefined behavior, from
    //     // Lecture 2.
    //     std::cout << compute_factorial(21) << '\n';  // -4249290049419214848
    // }

    // --- [Slide 65] Stack overflow --- UNDEFINED BEHAVIOR --------------------
    // This CRASHES. That is the point. Run it last.
    // {
    //     dig();
    //     std::cout << depth << '\n';  // never reached
    // }

    // --- [Slide 66] Recursion versus a loop ----------------------------------
    // Uncomment the [Slide 66] block at namespace scope first.
    // {
    //     std::vector<double> readings{0.5, 1.5, 2.0};
    //     std::cout << sum_readings(readings) << '\n';  // 4
    // }

    // --- [Appendix: The Costs of Recursion] ----------------------------------
    // Time it: 702build && time 702run week5
    // {
    //     std::cout << compute_fibonacci(40) << '\n';  // 102334155
    // }

    // --- [Appendix: A Folder Tree] -------------------------------------------
    // Uncomment the print_tree block at namespace scope first.
    // Run it from the week5 folder: 702w5 && 702run week5
    // {
    //     print_tree(fs::current_path(), 0);
    // }

    // #########################################################################
    // SECTION: THE main FUNCTION
    // #########################################################################

    // --- [Slide 69] Command-line arguments -----------------------------------
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
