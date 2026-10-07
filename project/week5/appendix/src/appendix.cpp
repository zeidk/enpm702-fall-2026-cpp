/**
 * @file appendix.cpp
 * @brief L5, Functions: the code of every appendix frame, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week5_appendix.
 *
 * @code
 * 702build week5_appendix
 * 702run week5_appendix        # every appendix frame, in order
 * 702run week5_appendix iv     # only [Appendix iv]; 702run week5_appendix 4 works too
 * @endcode
 *
 * The appendix frames are numbered i, ii, iii, ... in their top-left corner,
 * and @c "[Appendix iv]" is that number. Each frame's code is in its own
 * namespace, with a @c run() that does what the frame does in @c main(). The
 * table at the bottom of this file lists them, and run_slides() in
 * @c ../../common/slides.cpp runs them.
 *
 * Code that does not compile is commented out where its frame shows it:
 * uncomment it, build, and you get the frame's error. The code with undefined
 * behavior, [Appendix xix], is in @c ../../undefined/undefined.cpp, built with
 * the sanitizers. A full run skips it; @c "702run week5_appendix xix" runs it.
 *
 * @note Every number in the comments was measured with g++ 13.3,
 *       @c -std=c++20, on 2026-10-06. Addresses differ on every run and on
 *       every machine; only which ones are equal is the point.
 */
#include <array>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <numbers>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include "slides.hpp"
#include "undefined.hpp"

// [Appendix ii] constexpr Function
namespace constexpr_function {
// The constexpr Function frame.
constexpr double convert_deg_to_rad(double deg) {
  return deg * std::numbers::pi / 180.0;
}

void run() {
  constexpr double limit{convert_deg_to_rad(170.0)};  // at compile time
  std::cout << "limit: " << limit << '\n';             // 2.96706
}
}  // namespace constexpr_function

// [Appendix iii] The Same Function at Run Time
namespace same_function_run_time {
using constexpr_function::convert_deg_to_rad;

void run() {
  // The slide reads input with std::cin >> input;. A string stream stands in
  // for the keyboard here, so the program does not stop and wait.
  std::istringstream keyboard{"90"};
  double input{};
  keyboard >> input;
  double r{convert_deg_to_rad(input)};  // at run time
  double x{convert_deg_to_rad(170.0)};  // may run early, not required to
  std::cout << "input: " << input << ", r: " << r << ", x: " << x << '\n';  // 90 1.5708 2.96706
}
}  // namespace same_function_run_time

// [Appendix iv] consteval Function (C++20)
namespace consteval_function {
// The consteval Function (C++20) frame.
consteval int steps_for(int deg) { return deg * 10; }

void run() {
  std::array<int, steps_for(3)> plan{};  // 30 elements
  std::cout << "plan.size(): " << plan.size() << '\n';  // 30
  int n{3};
  // Does not compile: error: the value of 'n' is not usable in a constant expression
  // int bad{steps_for(n)};  // error: n is not a constant
  std::cout << "n: " << n << '\n';  // 3
}
}  // namespace consteval_function

// [Appendix v] Cases without Elision
namespace cases_without_elision {
// make_path from [Slide 53] Measured Elision, in week5_playground.
std::vector<double> make_path() {
  std::vector<double> v(1000);
  std::cout << "  inside make_path, &v: " << &v << ", v.data(): " << v.data() << '\n';
  return v;  // a named local
}

// Two Candidates. The line that prints x.data() is not on the slide: it shows
// where x's elements are, so the caller can tell a copy from a move.
namespace ternary {
std::vector<double> pick(bool first) {
  std::vector<double> x(1000);
  std::vector<double> y(1000);
  std::cout << "  inside pick, x.data(): " << x.data() << '\n';
  return first ? x : y;  // copy
}
}  // namespace ternary

namespace if_return {
std::vector<double> pick(bool first) {
  std::vector<double> x(1000);
  std::vector<double> y(1000);
  std::cout << "  inside pick, x.data(): " << x.data() << '\n';
  if (first) { return x; }  // move
  return y;
}
}  // namespace if_return

void run() {
  std::cout << "first ? x : y\n";
  std::vector<double> c{ternary::pick(true)};
  std::cout << "  c.data(): " << c.data() << '\n';  // a different address: copied

  std::cout << "if (first) { return x; }\n";
  std::vector<double> m{if_return::pick(true)};
  std::cout << "  m.data(): " << m.data() << '\n';  // the same address: moved

  // Assignment
  std::cout << "e = make_path();\n";
  // e exists: move-assign
  std::vector<double> e;
  e = make_path();
  std::cout << "  &e: " << &e << ", e.data(): " << e.data()
            << '\n';  // &e differs from &v, data() is the same: moved

  std::cout << "std::vector<double> f{make_path()};\n";
  // f is new: elided
  std::vector<double> f{make_path()};
  std::cout << "  &f: " << &f << ", f.data(): " << f.data() << '\n';  // both the same: elided
}
}  // namespace cases_without_elision

// [Appendix viii] Step 0: Inside main()
namespace call_steps {
// The program of Steps 0 to 6. The two lines that print a and b are not on
// the slides: without them g++ warns that a and b are unused.
void C() { }
void B() { int b{2}; C(); std::cout << "  in B, b: " << b << '\n'; }  // 2
void A() { int a{1}; B(); std::cout << "  in A, a: " << a << '\n'; }  // 1

void run() { A(); }
}  // namespace call_steps

// [Appendix ix] Step 1: The Call to A()
namespace step_1 {
void run() { call_steps::A(); }  // the same program as Step 0
}  // namespace step_1

// [Appendix x] Step 2: The Call to B()
namespace step_2 {
void run() { call_steps::A(); }
}  // namespace step_2

// [Appendix xi] Step 3: The Call to C()
namespace step_3 {
void run() { call_steps::A(); }
}  // namespace step_3

// [Appendix xii] Step 4: The Return from C()
namespace step_4 {
void run() { call_steps::A(); }
}  // namespace step_4

// [Appendix xiii] Step 5: The Return from B()
namespace step_5 {
void run() { call_steps::A(); }
}  // namespace step_5

// [Appendix xiv] Step 6: The Return from A()
namespace step_6 {
void run() { call_steps::A(); }
}  // namespace step_6

// [Appendix xv] The Cost of a Call
namespace cost_of_call {
void g(int a) {
  int b{a + 1};  // b lives in g's frame
  std::cout << "b: " << b << '\n';  // 11; not on the slide: without it b is unused
}                // b destroyed here
void f() {
  int x{10};     // x lives in f's frame
  g(x);          // g's frame stacks on top of f's
}                // x destroyed here

void run() { f(); }
}  // namespace cost_of_call

// [Appendix xvi] Frame Addresses
namespace frame_addresses {
void descend(int depth) {
  int local{depth};
  std::cout << "depth " << depth << "  &local = " << &local << '\n';
  if (depth < 3) { descend(depth + 1); }
}

void run() {
  descend(1);  // each address 0x30, 48 bytes, below the last
}
}  // namespace frame_addresses

// [Appendix xvii] Recursive Function
namespace recursion {
long long compute_factorial(int n) {
  if (n <= 1) {        // base case
    return 1;
  }
  return n * compute_factorial(n - 1);
}

void run() {
  long long r{compute_factorial(4)};
  std::cout << "r: " << r << '\n';  // 24
}
}  // namespace recursion

// [Appendix xix] Stack Overflow: undefined behavior, so the code is in
// ../../undefined/undefined.cpp, built with the sanitizers.

// [Appendix xx] Recursion versus a Loop
namespace recursion_or_loop {
double sum_readings(std::span<const double> readings) {
  double total{0.0};
  for (double r : readings) {
    total += r;
  }
  return total;
}

void run() {
  std::vector<double> readings{0.5, 1.5, 2.0};
  std::cout << "sum_readings(readings): " << sum_readings(readings) << '\n';  // 4
}
}  // namespace recursion_or_loop

// [Appendix xxi] The Costs of Recursion
namespace costs_of_recursion {
long long compute_fibonacci(int n) {
  if (n < 2) { return n; }
  return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
}

void run() {
  const auto start{std::chrono::steady_clock::now()};
  long long result{compute_fibonacci(40)};  // 331,160,281 calls
  const std::chrono::duration<double> took{std::chrono::steady_clock::now() - start};
  std::cout << "compute_fibonacci(40): " << result << '\n';  // 102334155
  std::cout << "took: " << took.count() << " s\n";  // depends on the machine and the -O level
}
}  // namespace costs_of_recursion

// [Appendix xxii] A Folder Tree
namespace folder_tree {
namespace fs = std::filesystem;
void print_tree(const fs::path& dir, int depth) {
  for (const auto& entry : fs::directory_iterator(dir)) {
    std::cout << std::string(2 * depth, ' ')
      << entry.path().filename().string() << '\n';
    if (entry.is_directory()) {
      print_tree(entry.path(), depth + 1);
    }
  }
}

void run() {
  // The slide lists arm_demo. Once Doxygen has run there, arm_demo holds
  // thousands of generated files, so this lists appendix, the folder of this
  // file, instead. __FILE__ is this file's path, as the compiler saw it.
  const fs::path appendix{fs::path{__FILE__}.parent_path().parent_path()};
  if (!fs::is_directory(appendix)) {
    std::cout << "folder not found: " << appendix << '\n';
    return;
  }
  std::cout << appendix.filename().string() << '\n';
  print_tree(appendix, 1);  // the order is the file system's, not sorted
}
}  // namespace folder_tree

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (4 is iv), its
  // title, and the function that runs it.
  const std::vector<Slide> slides{
      {2, "constexpr Function", constexpr_function::run},
      {3, "The Same Function at Run Time", same_function_run_time::run},
      {4, "consteval Function (C++20)", consteval_function::run},
      {5, "Cases without Elision", cases_without_elision::run},
      {8, "Step 0: Inside main()", call_steps::run},
      {9, "Step 1: The Call to A()", step_1::run},
      {10, "Step 2: The Call to B()", step_2::run},
      {11, "Step 3: The Call to C()", step_3::run},
      {12, "Step 4: The Return from C()", step_4::run},
      {13, "Step 5: The Return from B()", step_5::run},
      {14, "Step 6: The Return from A()", step_6::run},
      {15, "The Cost of a Call", cost_of_call::run},
      {16, "Frame Addresses", frame_addresses::run},
      {17, "Recursive Function", recursion::run},
      {19, "Stack Overflow", undefined::stack_overflow::run, true},
      {20, "Recursion versus a Loop", recursion_or_loop::run},
      {21, "The Costs of Recursion", costs_of_recursion::run},
      {22, "A Folder Tree", folder_tree::run},
  };
  return run_slides(slides, argc, argv, Part::appendix);
}
