/**
 * @file snippets.cpp
 * @brief Every code snippet from the L5 slides (Functions), switched off, so
 *        you can try them one at a time.
 *
 * @details Build target: @c week5_snippets. This file stands alone: it
 * includes no project header and is linked with nothing else, so any block can
 * be switched on without clashing with anything.
 *
 * The arm program split into headers and source files is in
 * @c ../../arm_demo/ (build target @c week5_arm_demo). The Header Files
 * slides, Exercise 1 and the Documenting Functions section use it.
 *
 * @par How to use it
 * Every snippet sits between @c "#if 0" and @c "#endif", so the compiler skips
 * it. To try one, change its @c "#if 0" to @c "#if 1", build, run, then set it
 * back to @c "#if 0" and move on. Only that one line changes: the explanation
 * around a block is ordinary comments, which stay comments whatever you do.
 *
 * @par Tags
 * @c "[Slide N]" is the frame number printed in the top-left corner of the
 * slide. @c "[Appendix: Title]" is a frame in the appendix, after the summary.
 * Those frames print no number, so they are named by the title at their top.
 *
 * @par Where the snippets live
 * Snippets that are whole functions live at namespace scope, above @c main().
 * Snippets that are statements live inside @c main(), each in its own @c { }
 * block so the same variable name can be reused from one to the next. Where a
 * block needs both, the comment says which other block to enable with it.
 *
 * @note Some snippets share a name with another snippet, because the slides
 *       show the same function written two ways. The comment on each says
 *       which other block it clashes with. Enable one of them at a time.
 * @note Blocks marked "DOES NOT COMPILE" are on the slides to show you an
 *       error. Enable them on purpose, read the message, set them back.
 * @warning Blocks marked "UNDEFINED BEHAVIOR" are the point of the lecture,
 *          not accidents. They may crash, print garbage, or appear to work.
 *          Build them with AddressSanitizer, which names the bug: uncomment
 *          the two @c -fsanitize lines in @c project/week5/CMakeLists.txt.
 * @note Every number in the comments was measured on the course machine,
 *       g++ 13.3 with @c -std=c++20, on 2026-09-26. Addresses differ on every
 *       run and on every machine; only their order is the point.
 */

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

/// The joint limit of the arm, in degrees. Several snippets clamp a joint to
/// it, so the constant is here once rather than inside each of them.
constexpr double max_deg{170.0};

// =============================================================================
// NAMESPACE SCOPE
//
// A function cannot be defined inside another function, so every snippet that
// is a whole function lives here. Enable it together with the matching
// block inside main().
// =============================================================================

// #############################################################################
// SECTION: FUNCTIONS
// #############################################################################

// --- [Slide 7] With a function: the logic is written once -------------------
// Shares the name clamp_joint with [Slide 33] and [Slide 49].
#if 0
double clamp_joint(double deg) {
  if (deg > max_deg) { return max_deg; }
  if (deg < -max_deg) { return -max_deg; }
  return deg;
}
#endif

// --- [Slide 8] The four parts of a function ---------------------------------
// Shares the name convert_deg_to_rad with [Slide 9] and
// [Appendix: constexpr Function].
#if 0
double convert_deg_to_rad(double deg) {
  return deg * std::numbers::pi / 180.0;  // C++20, <numbers>
}
#endif

// --- [Slide 9] Function header against function body ------------------------
// Shares the name convert_deg_to_rad with [Slide 8] and
// [Appendix: constexpr Function].
#if 0
constexpr double convert_deg_to_rad(double deg)  // the header
{
  return deg * std::numbers::pi / 180.0;         // the body
}
#endif

// --- [Slide 10] The signature includes the namespace ------------------------
// Inside a namespace, so this one does NOT collide. The signature is
// robot::convert_deg_to_rad(double): the name, the namespace, and the
// parameter types. That is what the LINKER uses, and what shows up in
// "undefined reference to `robot::convert_deg_to_rad(double)'".
#if 0
namespace robot {
constexpr double convert_deg_to_rad(double deg) {
  return deg * std::numbers::pi / 180.0;
}
}  // namespace robot
#endif

// --- [Slide 11] Parameters and arguments ------------------------------------
#if 0
void print_velocities(double linear, double angular) {  // parameters
  std::cout << linear << ' ' << angular << '\n';
}
#endif

// --- [Slide 13] Declaration order: the broken version -----------------------
// DOES NOT COMPILE: 'print_limits' was not declared in this scope.
#if 0
void report_arm() {
  std::cout << "arm: ";
  print_limits();  // not seen yet
}

void print_limits() { std::cout << "170 deg\n"; }
#endif

// --- [Slide 13] Declaration order: the fix ----------------------------------
// One declaration above both definitions. Enable this OR the broken
// version above, not both.
#if 0
void print_limits();  // the promise

void report_arm() {
  std::cout << "arm: ";
  print_limits();  // OK
}

void print_limits() { std::cout << "170 deg\n"; }
#endif

// --- [Slide 14] [Slide 15] Two functions that call each other ---------------
// No order works without a declaration, because each one needs the other.
// [Slide 15] puts every declaration at the top instead; either way compiles.
// Enable a clamp_joint block ([Slide 7], [Slide 33] or [Slide 49]) too.
#if 0
void retry_move(double deg);  // the declaration that breaks the cycle

void move_joint(double deg) {
  if (deg > max_deg) {
    retry_move(deg);
    return;
  }
  std::cout << "driving to " << deg << '\n';
}

void retry_move(double deg) { move_joint(clamp_joint(deg)); }
#endif

// --- [Slide 16] A missing definition ----------------------------------------
// Enable this AND the call block in main(), with no clamp_joint block
// enabled, and you get the slide's link error. The two stages, one at a
// time:
//   702w5
//   g++ -std=c++20 -c playground/src/snippets.cpp -o /tmp/main.o  # compiles
//   g++ /tmp/main.o -o /tmp/week5                             # undefined reference
#if 0
double clamp_joint(double deg);  // promised, never delivered
#endif

// --- [Slide 29] The flow of control through a call --------------------------
// Enable this OR the [Slide 13] blocks: they share print_limits.
#if 0
void print_limits() { std::cout << "170 deg\n"; }

void report_arm() {
  std::cout << "arm: ";
  print_limits();
}
#endif

// --- [Slide 30] The return statement ----------------------------------------
#if 0
void print_range(double m) {
  if (m < 0.0) {
    std::cout << "invalid\n";
    return;  // leave early
  }
  std::cout << m << " m\n";
}  // returns here otherwise

int calculate_sum(int a, int b) {
  int result{a + b};
  return result;
}
#endif

// --- [Slide 31] Missing returns --- UNDEFINED BEHAVIOR ----------------------
// get_sign(0) falls off the end. GCC warns even without -Wall:
//   warning: control reaches end of non-void function [-Wreturn-type]
#if 0
int get_sign(int number) {
  if (number > 0) {
    return 1;
  } else if (number < 0) {
    return -1;
  }
}  // number == 0 falls off the end
#endif

// --- [Slide 32] Conversion on return ----------------------------------------
// Silent under -Wall -Wextra -pedantic-errors. Add -Wconversion to see:
//   warning: conversion from 'double' to 'int' may change value
#if 0
int truncate_value() {
  double value{99.99};
  return value;  // converted to int: 99
}
#endif

// --- [Slide 33] [[nodiscard]] -----------------------------------------------
// Shares the name clamp_joint with [Slide 7] and [Slide 49].
#if 0
[[nodiscard]] double clamp_joint(double deg) {
  return std::clamp(deg, -max_deg, max_deg);
}
#endif

// --- [Appendix: constexpr Function] -----------------------------------------
// constexpr ALLOWS the compiler to run the call while compiling. It does not
// require it. Shares the name convert_deg_to_rad with [Slide 8] and [Slide 9].
#if 0
constexpr double convert_deg_to_rad(double deg) {
  return deg * std::numbers::pi / 180.0;
}
#endif

// --- [Appendix: consteval Function] -----------------------------------------
// consteval REQUIRES it. Every call needs constant arguments.
#if 0
consteval int steps_for(int deg) { return deg * 10; }  // C++20
#endif

// #############################################################################
// SECTION: PASSING ARGUMENTS
// #############################################################################

// --- [Slide 36] Pass by value -----------------------------------------------
// The parameter is a new object, initialized from the argument, exactly as
// if you had written double deg{q2}.
#if 0
void nudge_joint(double deg) {
  deg += 10.0;  // changes the copy
}
#endif

// --- [Slide 37] The cost of a copy ------------------------------------------
// Shares the name average_angle with [Slide 40] and [Slide 42].
#if 0
double average_angle(std::vector<double> angles) {  // a copy
  double sum{0.0};
  for (double a : angles) { sum += a; }
  return sum / static_cast<double>(angles.size());
}
#endif

// --- [Slide 38] Pass by reference -------------------------------------------
// Enable this OR [Slide 36]: with both, nudge_joint(q2) is ambiguous.
#if 0
void nudge_joint(double& deg) {
  deg += 10.0;  // changes the caller's q2
}
#endif

// --- [Slide 39] A swap function: by value, then by reference ----------------
// Enable one at a time: together, swap_deg(q2, q3) is ambiguous.
#if 0
void swap_deg(double a, double b) {  // swaps two copies: does nothing
  double tmp{a};
  a = b;
  b = tmp;
}
#endif

#if 0
void swap_deg(double& a, double& b) {  // swaps the caller's variables
  double tmp{a};
  a = b;
  b = tmp;
}
#endif

// --- [Slide 40] Pass by const reference -------------------------------------
// No copy, and the function may only read. This is the default for anything
// bigger than a few words. Shares the name average_angle with [Slide 37] and
// [Slide 42]. Enable the push_back line to see the slide's error:
//   error: passing 'const std::vector<double>' as 'this' argument discards qualifiers
#if 0
double average_angle(const std::vector<double>& angles) {  // no copy
  double sum{0.0};
  for (double a : angles) { sum += a; }
  // angles.push_back(0.0);  // error: discards qualifiers
  return sum / static_cast<double>(angles.size());
}
#endif

// --- [Slide 41] String parameters -------------------------------------------
// A std::string_view accepts a std::string or a literal, and copies neither.
#if 0
void log_joint(std::string_view name) {  // C++17, <string_view>
  std::cout << name << '\n';
}
#endif

// --- [Slide 42] std::span ---------------------------------------------------
// A view of a contiguous sequence: a pointer and a count, 16 bytes, whether
// the sequence holds 2 elements or 2 million. [Slide 43] passes it three
// kinds of sequence. Shares the name average_angle with [Slide 37] and
// [Slide 40].
#if 0
double average_angle(std::span<const double> angles) {  // C++20, <span>
  double sum{0.0};
  for (double a : angles) { sum += a; }
  return sum / static_cast<double>(angles.size());
}
#endif

// --- [Slide 45] Pass by pointer ---------------------------------------------
// The caller passes an address, and nullptr means "no object". Check it before
// you dereference it. This overload does not clash with [Slide 36] or
// [Slide 38]: &q2 is a double*, so nudge_joint(&q2) can only pick this one.
#if 0
void nudge_joint(double* p) {  // double* p{&q2};
  if (p != nullptr) {
    *p += 10.0;  // changes q2
  }
}
#endif

// --- [Slide 47] Exercise 2: four calls --------------------------------------
// Write your answer down before you run it. GCC gives away one of the four:
// "parameter 'x' set but not used" on f1, because f1 changes nothing the
// caller can see.
#if 0
void f1(int x) { x = 99; }
void f2(int& x) { x = 99; }
void f3(int* p) { p = nullptr; }
void f4(int* p) { *p = 99; }
#endif

// #############################################################################
// SECTION: RETURNING VALUES
// #############################################################################

// --- [Slide 49] Return by value ---------------------------------------------
// Shares the name clamp_joint with [Slide 7] and [Slide 33]. [Slide 12] shows
// the same function split into a declaration and a definition.
// On x86-64 a small result such as a double comes back in the register xmm0.
#if 0
double clamp_joint(double deg) {
  return std::clamp(deg, -max_deg, max_deg);
}
#endif

// --- [Slide 50] A large result ----------------------------------------------
// No copy is made: the vector is built directly in the caller's variable.
// [Slide 51] names the rule: copy elision.
#if 0
std::vector<double> plan_path() {
  std::vector<double> angles(1'000'000);
  // One step per millisecond: sweep the joint from 0 to 90 degrees over
  // 1000 seconds.
  const double last{static_cast<double>(angles.size() - 1)};
  for (std::size_t i{0}; i < angles.size(); ++i) {
    angles[i] = 90.0 * static_cast<double>(i) / last;
  }
  return angles;  // copy a million doubles? no.
}
#endif

// --- [Slide 52] Measured elision --------------------------------------------
// &v is the vector object. v.data() is its heap block. Compare the two lines
// that main() prints:
//   same object, same block   elided (the default, even at -O0)
//   new object, same block    moved
//   new block as well         copied
// Rebuild with -fno-elide-constructors and the object changes but the block
// does not: NRVO is off, and the named local is moved instead.
//   702w5
//   g++ -std=c++20 -fno-elide-constructors playground/src/snippets.cpp -o /tmp/elide
#if 0
std::vector<double> make_path() {
  std::vector<double> v(1000);
  std::cout << &v << ' ' << v.data() << '\n';
  return v;  // a named local
}
#endif

// --- [Appendix: Cases without Elision] Two candidates -----------------------
// Enable one pick at a time: they share a name. With two candidates NRVO
// is not done, and the implicit move needs a plain name. The first line of
// each prints x's heap block, so main() can tell a copy from a move. That
// line is not on the slide.
#if 0
std::vector<double> pick(bool first) {
  std::vector<double> x(1000);
  std::vector<double> y(1000);
  std::cout << x.data() << '\n';
  return first ? x : y;  // copy
}
#endif

#if 0
std::vector<double> pick(bool first) {
  std::vector<double> x(1000);
  std::vector<double> y(1000);
  std::cout << x.data() << '\n';
  if (first) { return x; }  // move
  return y;
}
#endif

// --- [Slide 53] Return by reference -----------------------------------------
// Safe: the vector belongs to the caller and outlives the call.
#if 0
double& get_joint(std::vector<double>& q, std::size_t i) { return q.at(i); }
#endif

// --- [Slide 54] Returning a local --- UNDEFINED BEHAVIOR --------------------
// warning: reference to local variable 'local_x' returned [-Wreturn-local-addr]
#if 0
double& tool_x() {
  double local_x{0.42};
  return local_x;
}  // local_x is destroyed here
#endif

// --- [Slide 55] Returning a parameter --- UNDEFINED BEHAVIOR ----------------
// No local in sight, and it still dangles: the argument is a temporary
// std::string built for the call, and it dies at the end of that line.
#if 0
const std::string& pick_longer(const std::string& a, const std::string& b) {
  return a.size() >= b.size() ? a : b;
}
#endif

// --- [Slide 56] A search result ---------------------------------------------
// A pointer can say "nothing found" with nullptr. A reference cannot.
#if 0
double* find_value(std::vector<double>& v, double target) {
  for (double& x : v) {
    if (x == target) { return &x; }
  }
  return nullptr;  // not found
}
#endif

// --- [Slide 57] Returning the address of a local --- UNDEFINED BEHAVIOR -----
// Enable this OR [Slide 54]: they share the name tool_x.
// warning: address of local variable 'local_x' returned [-Wreturn-local-addr]
// GCC returns a null pointer in place of the dead address, so this crashed
// with SIGSEGV (exit code 139) at -O0 and -O2.
#if 0
double* tool_x() {
  double local_x{0.42};
  return &local_x;
}  // local_x is destroyed here
#endif

// #############################################################################
// SECTION: FUNCTION OVERLOADING
// #############################################################################

// --- [Slide 59] Function overloading ----------------------------------------
// Shares the name print_pose with [Slide 64].
#if 0
void print_pose(double x, double y) { std::cout << x << ' ' << y << '\n'; }

void print_pose(double x, double y, double deg) {
  std::cout << x << ' ' << y << ' ' << deg << '\n';
}

void print_pose(std::string_view name, double x, double y) {
  std::cout << name << ": " << x << ' ' << y << '\n';
}
#endif

// --- [Slide 60] Valid overloads: count, type, order -------------------------
// Any one of the three differences is enough.
#if 0
void move_joint(int id);
void move_joint(int id, int deg);
void move_joint(double deg);
void move_joint(int id, double deg);
void move_joint(double deg, int id);
#endif

// --- [Slide 60] Overloading on the return type --- DOES NOT COMPILE ---------
// error: ambiguating new declaration of 'double joint_count()'
// A call does not say which return type it wants.
#if 0
int joint_count() { return 3; }
double joint_count() { return 3.0; }
#endif

// --- [Slide 62] Exercise 3: overload resolution -----------------------------
#if 0
int add(int a, int b) { return a + b; }
int add(int a, float b) { return a + b; }
int add(int a, double b) { return a + b; }
#endif

// #############################################################################
// SECTION: DEFAULT ARGUMENTS
// #############################################################################

// --- [Slide 64] [Slide 66] Default arguments --------------------------------
// The default belongs in the DECLARATION, where callers can see it, and the
// definition must not repeat it. Add "= 3" to the definition below and you
// get: error: default argument given for parameter 3 of
// 'void print_pose(double, double, int, std::string_view)', even though the
// value is the same. Shares the name print_pose with [Slide 59].
#if 0
void print_pose(double x, double y, int precision = 3,
                std::string_view label = "tool");

void print_pose(double x, double y, int precision, std::string_view label) {
  std::cout << label << ' ' << x << ' ' << y << "  (" << precision << " dp)\n";
}
#endif

// --- [Slide 67] Defaults versus overloads -----------------------------------
// Two functions that differ only by a missing value are one function with a
// default. Enable one of the two, not both: with both, print_pose(x, y)
// is ambiguous. Declarations only, so there is nothing to run.
//
// Two functions:
#if 0
void print_pose(double x, double y);
void print_pose(double x, double y, int precision);
#endif

// One function:
#if 0
void print_pose(double x, double y, int precision = 3);
#endif

// #############################################################################
// SECTION: STATIC LOCAL VARIABLES
// #############################################################################

// --- [Slide 69] Lifetime and scope ------------------------------------------
// id is not on the stack. It sits in .bss, because its initializer is 0.
// A static with no initializer, such as count below, is zero-initialized and
// also sits in .bss. [Appendix: Five Common Uses] lists where a static local
// is worth it.
#if 0
int next_move_id() {
  static int id{0};
  return ++id;
}

int next_count() {
  static int count;  // no initializer: starts at 0
  return ++count;
}
#endif

// --- [Slide 70] One-time initialization -------------------------------------
// The INITIALIZER runs once. An ASSIGNMENT runs on every call.
#if 0
int count_calls() {
  static int calls{0};  // once
  ++calls;
  return calls;  // 1, 2, 3
}

int count_calls_wrong() {
  static int calls;
  calls = 0;  // every call
  ++calls;
  return calls;  // always 1
}
#endif

// --- [Appendix: Five Common Uses] A table built once ------------------------
// "building table" is printed once, however many lookups you do.
#if 0
std::vector<double> build_rad_table() {
  std::cout << "building table\n";
  std::vector<double> table(361);
  for (int deg{0}; deg <= 360; ++deg) {
    table[static_cast<std::size_t>(deg)] = deg * std::numbers::pi / 180.0;
  }
  return table;
}

double rad_of(int deg) {
  static const std::vector<double> table{build_rad_table()};
  return table.at(static_cast<std::size_t>(deg));
}
#endif

// #############################################################################
// SECTION: THE CALL STACK
// #############################################################################

// --- [Slide 72] A call pushes a frame, a return pops it ---------------------
// The appendix walks through the same program one call at a time, from
// [Appendix: Step 0: Inside main()] on, with a local in A and in B:
// void B() { int b{2}; C(); } and void A() { int a{1}; B(); }.
#if 0
void C() { }
void B() { C(); }
void A() { B(); }
#endif

// --- [Appendix: The Cost of a Call] -----------------------------------------
// Entering a function subtracts a size fixed at compile time from the stack
// pointer. Returning adds it back. One instruction each way.
// Enable this OR [Slide 73]: they share the names f and g.
#if 0
void g(int a) {
  int b{a + 1};  // b lives in g's frame
  std::cout << b << '\n';
}                // b destroyed here

void f() {
  int x{10};     // x lives in f's frame
  g(x);          // g's frame stacks on top of f's
}                // x destroyed here
#endif

// --- [Appendix: Frame Addresses] --------------------------------------------
// Each address is 0x30 (48 bytes) BELOW the last: the stack grows downward.
#if 0
void descend(int depth) {
  int local{depth};
  std::cout << "depth " << depth << "  &local = " << &local << '\n';
  if (depth < 3) { descend(depth + 1); }
}
#endif

// --- [Slide 73] Exercise 4: a stack trace -----------------------------------
// Enable this OR [Appendix: The Cost of a Call]: they share the names f
// and g. Put a breakpoint on the line "x += y + *z;" and read the VS Code
// CALL STACK panel. gdb's bt prints the same three frames:
//   #0  f (x=@0x7fffffffc6e4: 30, y=10, z=0x7fffffffc6d8)
//   #1  in g (a=10, b=20)
//   #2  in main ()
// calls and scale are in no frame. Which Lecture 2 segment holds each one?
#if 0
constexpr int scale{2};

int e() {
    static int calls{0};
    ++calls;
    return calls;
}

void f(int &x, int y, int *z) {
    int first{e()};
    int second{e()};
    x += (y + *z) * (first + second);
}

int g(int a, int b) {
    int result{};
    result = a + b;
    f(result, a, &b);
    return result * scale;
}
#endif

// --- [Appendix: Recursion] [Appendix: Frames of a Recursive Call] -----------
// Four frames of compute_factorial are on the stack at once, each with its
// own n. Every call must move TOWARD the base case.
#if 0
long long compute_factorial(int n) {
  if (n <= 1) {  // base case
    return 1;
  }
  return n * compute_factorial(n - 1);
}
#endif

// --- [Appendix: Stack Overflow] --- UNDEFINED BEHAVIOR ----------------------
// No base case. At -O0 with the Linux default 8 MiB stack this crashed with
// "Segmentation fault" after about 520,000 calls: 8,388,608 bytes divided by
// a 16-byte frame is 524,288 frames.
#if 0
int depth{0};

void dig() {
  ++depth;
  dig();  // no base case
}
#endif

// --- [Appendix: Recursion versus a Loop] ------------------------------------
// A walk along a sequence is a loop: the depth would be its length, and
// every call would cost a frame. A loop reuses one.
#if 0
double sum_readings(std::span<const double> readings) {
  double total{0.0};
  for (double r : readings) {
    total += r;
  }
  return total;
}
#endif

// --- [Appendix: The Costs of Recursion] -------------------------------------
// compute_fibonacci(40) makes 331,160,281 calls and took 0.13 s at -O2.
// A loop does 40 additions in under a microsecond.
#if 0
long long compute_fibonacci(int n) {
  if (n < 2) { return n; }
  return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
}
#endif

// --- [Appendix: A Folder Tree] recursion that follows the data --------------
#if 0
namespace fs = std::filesystem;

void print_tree(const fs::path& dir, int depth) {
  for (const auto& entry : fs::directory_iterator(dir)) {
    std::cout << std::string(2 * static_cast<std::size_t>(depth), ' ')
              << entry.path().filename().string() << '\n';
    if (entry.is_directory()) {
      print_tree(entry.path(), depth + 1);
    }
  }
}
#endif

// =============================================================================
// [Slide 75] Two forms of main
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
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    // #########################################################################
    // SECTION: FUNCTIONS
    // #########################################################################

    // --- [Slide 7] Without a function: the logic is copied -------------------
    // The wrist block was copied and one q2 was never renamed. It compiles
    // with no warning, and the wrist stays at -250.
#if 0
    {
        double q2{-200.0};  // elbow
        if (q2 > max_deg) { q2 = max_deg; }
        if (q2 < -max_deg) { q2 = -max_deg; }

        double q3{-250.0};  // wrist, copied
        if (q3 > max_deg) { q3 = max_deg; }
        if (q3 < -max_deg) { q2 = -max_deg; }  // the bug

        std::cout << q2 << ' ' << q3 << '\n';  // -170 -250, not -170 -170
    }
#endif

    // --- [Slide 7] With a function -------------------------------------------
#if 0
    {
        double q2{clamp_joint(-200.0)};  // elbow
        double q3{clamp_joint(-250.0)};  // wrist
        std::cout << q2 << ' ' << q3 << '\n';  // -170 -170
    }
#endif

    // --- [Slide 11] Parameters and arguments ---------------------------------
    // Enable print_velocities at namespace scope first.
#if 0
    {
        print_velocities(0.5, 0.1);  // arguments: 0.5, 0.1
    }
#endif

    // --- [Slide 13] [Slide 29] The flow of control through a call ------------
    // Enable one report_arm block at namespace scope first.
#if 0
    {
        report_arm();                   // prints "arm: 170 deg"
        std::cout << "exit main\n";
    }
#endif

    // --- [Slide 14] Two functions that call each other -----------------------
    // Enable the [Slide 14] block at namespace scope first.
#if 0
    {
        move_joint(200.0);  // clamped to 170, then driven
    }
#endif

    // --- [Slide 16] A missing definition -------------------------------------
#if 0
    {
        std::cout << clamp_joint(200.0) << '\n';  // 170
    }
#endif

    // --- [Slide 30] The return statement -------------------------------------
    // Enable the [Slide 30] block at namespace scope first.
#if 0
    {
        print_range(-1.0);  // invalid
        print_range(2.5);   // 2.5 m

        int sum{calculate_sum(5, 3)};  // 8
        std::cout << sum << '\n';
    }
#endif

    // --- [Slide 31] Missing returns --- UNDEFINED BEHAVIOR -------------------
#if 0
    {
        std::cout << get_sign(5) << '\n';   // 1
        std::cout << get_sign(-5) << '\n';  // -1
        std::cout << get_sign(0) << '\n';   // whatever is left over
    }
#endif

    // --- [Slide 32] Conversion on return -------------------------------------
#if 0
    {
        std::cout << truncate_value() << '\n';  // 99, the .99 is gone
    }
#endif

    // --- [Slide 33] [[nodiscard]] --------------------------------------------
#if 0
    {
        clamp_joint(200.0);  // warning: ignoring return value

        std::vector<double> v{1.0};
        v.empty();  // warning: empty() only ASKS, it empties nothing
    }
#endif

    // --- [Appendix: constexpr Function] --------------------------------------
    // Enable the constexpr block at namespace scope first.
    // [Appendix: The Same Function at Run Time] is the second half.
#if 0
    {
        constexpr double limit{convert_deg_to_rad(170.0)};  // at compile time
        std::cout << limit << '\n';                         // 2.967

        double input{};
        std::cin >> input;
        double r{convert_deg_to_rad(input)};  // at run time
        std::cout << r << '\n';

        double x{convert_deg_to_rad(170.0)};  // may run early, not required to
        std::cout << x << '\n';
    }
#endif

    // --- [Appendix: consteval Function] --------------------------------------
    // Enable the consteval block at namespace scope first.
#if 0
    {
        std::array<int, steps_for(3)> plan{};  // 30 elements
        std::cout << plan.size() << '\n';

        int n{3};  // holds 3, but is not constexpr
        // int bad{steps_for(n)};  // error: n is not a constant
        std::cout << n << '\n';
    }
#endif

    // #########################################################################
    // SECTION: PASSING ARGUMENTS
    // #########################################################################

    // --- [Slide 36] Pass by value --------------------------------------------
    // Enable the [Slide 36] block at namespace scope first.
#if 0
    {
        double q2{5.0};
        nudge_joint(q2);
        std::cout << q2 << '\n';  // 5: the function changed its own copy
    }
#endif

    // --- [Slide 37] The cost of a copy ---------------------------------------
    // Enable the [Slide 37] block at namespace scope first.
#if 0
    {
        std::vector<double> path(1'000'000, 1.0);
        std::cout << average_angle(path) << '\n';  // 8 MB allocated and copied
    }
#endif

    // --- [Slide 38] Pass by reference ----------------------------------------
    // Enable the [Slide 38] block at namespace scope first.
#if 0
    {
        double q2{5.0};
        nudge_joint(q2);
        std::cout << q2 << '\n';  // 15: the caller's variable changed

        // nudge_joint(5.0);  // error: no object to refer to
    }
#endif

    // --- [Slide 39] A swap function ------------------------------------------
    // Enable one [Slide 39] block at namespace scope first.
#if 0
    {
        double q2{1.0};
        double q3{2.0};
        swap_deg(q2, q3);
        std::cout << q2 << ' ' << q3 << '\n';  // by value: 1 2. by ref: 2 1
    }
#endif

    // --- [Slide 40] Pass by const reference ----------------------------------
    // Enable the [Slide 40] block at namespace scope first.
#if 0
    {
        std::vector<double> path(1'000'000, 1.0);
        std::cout << average_angle(path) << '\n';        // no copy
        std::cout << average_angle({1.0, 2.0}) << '\n';  // a temporary is accepted too
    }
#endif

    // --- [Slide 41] String parameters ----------------------------------------
    // Enable the [Slide 41] block at namespace scope first.
#if 0
    {
        std::string joint{"elbow"};
        log_joint(joint);    // views the string: no copy
        log_joint("elbow");  // views the literal: no std::string is built
    }
#endif

    // --- [Slide 43] One parameter, three sequences ---------------------------
    // Enable the [Slide 42] block at namespace scope first.
    // [Slide 44] draws what angles holds for arr.
#if 0
    {
        double c_array[]{1.0, 2.0, 3.0};
        std::array<double, 2> arr{4.0, 6.0};
        std::vector<double> vec{1.5, 2.5, 3.5, 4.5};

        std::cout << average_angle(c_array) << '\n';  // 2
        std::cout << average_angle(arr) << '\n';      // 5
        std::cout << average_angle(vec) << '\n';      // 3
    }
#endif

    // --- [Slide 45] Pass by pointer ------------------------------------------
    // Enable the [Slide 45] block at namespace scope first.
#if 0
    {
        double q2{5.0};
        nudge_joint(&q2);
        std::cout << q2 << '\n';  // 15

        nudge_joint(nullptr);  // does nothing
    }
#endif

    // --- [Slide 47] Exercise 2: four calls -----------------------------------
    // Enable the [Slide 47] block at namespace scope first.
    // Write your answer down, then run it. Explain f3 in one sentence.
#if 0
    {
        int a{1};
        int b{1};
        int c{1};
        int d{1};
        f1(a);  f2(b);  f3(&c);  f4(&d);
        std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    }
#endif

    // #########################################################################
    // SECTION: RETURNING VALUES
    // #########################################################################

    // --- [Slide 49] Return by value ------------------------------------------
#if 0
    {
        double q2{clamp_joint(200.0)};  // 170
        std::cout << q2 << '\n';
    }
#endif

    // --- [Slide 50] A large result -------------------------------------------
    // Enable the [Slide 50] block at namespace scope first.
#if 0
    {
        std::vector<double> path{plan_path()};  // no copy is made
        std::cout << path.size() << '\n';       // 1000000
        std::cout << path.front() << ' ' << path[path.size() / 2] << ' '
                  << path.back() << '\n';       // 0 45 90
    }
#endif

    // --- [Slide 52] Measured elision -----------------------------------------
    // Enable make_path at namespace scope first.
    // Default build: the two lines are identical. With -fno-elide-constructors
    // the first address differs and the second does not: a move.
#if 0
    {
        std::vector<double> path{make_path()};
        std::cout << &path << ' ' << path.data() << '\n';
    }
#endif

    // --- [Appendix: Cases without Elision] -----------------------------------
    // Enable one pick, and make_path, at namespace scope first.
    // The ternary pick prints two DIFFERENT blocks: a copy. The if pick prints
    // the SAME block twice: a move.
#if 0
    {
        std::vector<double> c{pick(true)};
        std::cout << c.data() << '\n';

        std::vector<double> e;  // e exists: move-assign
        e = make_path();

        std::vector<double> f{make_path()};  // f is new: elided
    }
#endif

    // --- [Slide 53] Return by reference --------------------------------------
    // Enable the [Slide 53] block at namespace scope first.
#if 0
    {
        std::vector<double> q{0.0, 0.5, 1.0};
        get_joint(q, 1) = 0.7;  // the call names q[1] itself
        std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1
    }
#endif

    // --- [Slide 54] Returning a local --- UNDEFINED BEHAVIOR -----------------
    // Enable the [Slide 54] block at namespace scope first, and the two
    // -fsanitize lines in CMakeLists.txt.
#if 0
    {
        double& r{tool_x()};
        std::cout << r << '\n';  // undefined behavior
    }
#endif

    // --- [Slide 55] Returning a parameter --- UNDEFINED BEHAVIOR -------------
    // Enable the [Slide 55] block at namespace scope first.
    // "camera" is not a std::string: the call builds a temporary one, and it
    // dies at the end of this line. AddressSanitizer says stack-use-after-scope.
#if 0
    {
        const std::string& r{pick_longer("lidar", "camera")};
        std::cout << r << '\n';  // undefined behavior
    }
#endif

    // --- [Slide 56] A search result ------------------------------------------
    // Enable the [Slide 56] block at namespace scope first.
#if 0
    {
        std::vector<double> q{0.0, 0.5, 1.0};
        double* p{find_value(q, 0.5)};
        if (p != nullptr) { *p = 0.7; }  // check it, every time
        std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << '\n';  // 0 0.7 1

        std::cout << (find_value(q, 9.9) == nullptr) << '\n';  // 1: not found
    }
#endif

    // --- [Slide 57] Returning the address of a local --- UNDEFINED BEHAVIOR ---
    // Enable the [Slide 57] block at namespace scope first.
#if 0
    {
        double* p{tool_x()};
        std::cout << *p << '\n';  // undefined behavior: crashed here
    }
#endif

    // #########################################################################
    // SECTION: FUNCTION OVERLOADING
    // #########################################################################

    // --- [Slide 59] Function overloading -------------------------------------
    // Enable the [Slide 59] block at namespace scope first.
#if 0
    {
        print_pose(0.42, 1.17);           // the two-value version
        print_pose(0.42, 1.17, 30.0);     // the three-value version
        print_pose("wrist", 0.42, 1.17);  // the labelled version
    }
#endif

    // --- [Slide 62] Exercise 3: overload resolution --------------------------
    // Enable the [Slide 62] block at namespace scope first.
    // For each line, name the version that is called and what it prints, or
    // say why it does not compile. Rank each argument with the table on
    // [Slide 61]. 'h' is 104.
#if 0
    {
        float f{3.5};
        long n{3};
        unsigned int u{3};

        std::cout << add(2, 3) << '\n';        // 1
        std::cout << add(2, f) << '\n';        // 2
        std::cout << add(2.5, 3) << '\n';      // 3
        std::cout << add('h', false) << '\n';  // 4
        std::cout << add(2, n) << '\n';        // 5
        std::cout << add(2, u) << '\n';        // 6
    }
#endif

    // #########################################################################
    // SECTION: DEFAULT ARGUMENTS
    // #########################################################################

    // --- [Slide 64] Default arguments ----------------------------------------
    // Enable the [Slide 64] block at namespace scope first.
    // Defaults fill from the RIGHT, and you cannot skip a middle one.
#if 0
    {
        print_pose(0.42, 1.17, 2, "wrist");  // 2 dp, wrist
        print_pose(0.42, 1.17, 2);           // 2 dp, tool
        print_pose(0.42, 1.17);              // 3 dp, tool
        // print_pose(0.42);                 // error: too few arguments
    }
#endif

    // #########################################################################
    // SECTION: STATIC LOCAL VARIABLES
    // #########################################################################

    // --- [Slide 69] Lifetime and scope ---------------------------------------
    // Enable the [Slide 69] block at namespace scope first.
#if 0
    {
        std::cout << next_move_id() << '\n';  // 1
        std::cout << next_move_id() << '\n';  // 2
        std::cout << next_move_id() << '\n';  // 3

        std::cout << next_count() << '\n';  // 1: count started at 0
    }
#endif

    // --- [Slide 70] One-time initialization ----------------------------------
#if 0
    {
        std::cout << count_calls() << count_calls() << count_calls() << '\n';
        std::cout << count_calls_wrong() << count_calls_wrong() << '\n';  // 11
    }
#endif

    // --- [Appendix: Five Common Uses] A table built once ---------------------
    // "building table" is printed once, not three times.
#if 0
    {
        std::cout << rad_of(10) << '\n';
        std::cout << rad_of(20) << '\n';
        std::cout << rad_of(30) << '\n';
    }
#endif

    // #########################################################################
    // SECTION: THE CALL STACK
    // #########################################################################

    // --- [Slide 72] A call pushes a frame, a return pops it ------------------
    // Enable the [Slide 72] block at namespace scope first, then step
    // through it in the debugger and watch the CALL STACK panel grow.
#if 0
    {
        A();
    }
#endif

    // --- [Appendix: The Cost of a Call] --------------------------------------
#if 0
    {
        f();
    }
#endif

    // --- [Appendix: Frame Addresses] -----------------------------------------
    // Each address is 48 bytes below the last. The values differ every run.
#if 0
    {
        descend(1);
    }
#endif

    // --- [Slide 73] Exercise 4: a stack trace --------------------------------
    // Draw the stack at "x += y + *z;". Which variable does x name there?
    // It is NOT the x in main. Which variable does z point at?
#if 0
    {
        int x{10};
        int y{20};
        int z{};
        z = g(x, y);
        std::cout << z << '\n'; // 120
    }
#endif

    // --- [Appendix: Recursion] [Appendix: Frames of a Recursive Call] --------
#if 0
    {
        std::cout << compute_factorial(4) << '\n';  // 24

        // [Appendix: Stack Overflow]: a second limit, nothing to do with
        // the stack. 21! does not fit in a long long. Signed overflow is
        // undefined behavior, from Lecture 2.
        std::cout << compute_factorial(21) << '\n';  // -4249290049419214848
    }
#endif

    // --- [Appendix: Stack Overflow] --- UNDEFINED BEHAVIOR -------------------
    // This CRASHES. That is the point. Run it last.
#if 0
    {
        dig();
        std::cout << depth << '\n';  // never reached
    }
#endif

    // --- [Appendix: Recursion versus a Loop] ---------------------------------
    // Enable sum_readings at namespace scope first.
#if 0
    {
        std::vector<double> readings{0.5, 1.5, 2.0};
        std::cout << sum_readings(readings) << '\n';  // 4
    }
#endif

    // --- [Appendix: The Costs of Recursion] ----------------------------------
    // Time it from build/project/week5: time ./week5_snippets
#if 0
    {
        std::cout << compute_fibonacci(40) << '\n';  // 102334155
    }
#endif

    // --- [Appendix: A Folder Tree] -------------------------------------------
    // Enable the print_tree block at namespace scope first.
    // Run it from project/week5 so it lists that folder:
    //   702w5 && ../../build/project/week5/week5_snippets
#if 0
    {
        print_tree(fs::current_path(), 0);
    }
#endif

    // #########################################################################
    // SECTION: THE main FUNCTION
    // #########################################################################

    // --- [Slide 76] Command-line arguments -----------------------------------
    // From build/project/week5: ./week5_snippets 30 -45 60
#if 0
    {
        std::cout << "Number of arguments: " << argc << '\n';

        for (int i{0}; i < argc; ++i) {
            std::cout << "argv[" << i << "]: " << argv[i] << '\n';
        }
    }
#endif

    return 0;
}
