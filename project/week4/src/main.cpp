// =============================================================================
// ENPM702 - Introductory Robot Programming
// Week 4 playground - L4: The Standard Library and Its Containers
//
// HOW TO USE THIS FILE
//   Every snippet below is copied from a slide and is COMMENTED OUT.
//   Uncomment one block, build, run, then comment it back and move on.
//   In VS Code: select the block and press Ctrl+/ (Cmd+/ on macOS).
//
//   [Slide N] is the frame number printed in the TOP-LEFT corner of the slide.
//
//   Each snippet lives in its own { } block, so the same variable name can be
//   reused from one snippet to the next without a redeclaration error.
//
// THREE THINGS TO EXPECT
//   1. Blocks marked "DOES NOT COMPILE" are on the slides to show you an error.
//      Uncomment them on purpose, read the message, then comment them back.
//   2. Blocks marked "UNDEFINED BEHAVIOR" are the point of the lecture, not
//      accidents. They may crash, print garbage, print the right answer, or
//      appear to work. Build them with AddressSanitizer, which names the bug:
//      uncomment the two -fsanitize lines in project/week4/CMakeLists.txt.
//   3. Every number in the comments was measured on the course machine,
//      GCC 13 with libstdc++. Where the standard leaves a choice to the
//      implementation (capacities, the SSO size), the comment says so.
// =============================================================================

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

// =============================================================================
// NAMESPACE SCOPE
//
// A few snippets cannot live inside main(): a function, and a type with the
// functions that take it. Uncomment these together with the matching block
// inside main().
// =============================================================================

// --- [Slide 22] sizeof after decay ------------------------------------------
// void report(int joint_deg[6]) {  // the 6 is ignored: this is int*
//     std::cout << sizeof(joint_deg) << '\n';                         // 8: pointer
//     std::cout << sizeof(joint_deg) / sizeof(joint_deg[0]) << '\n';  // 2
// }

// --- [Slide 53] reserve: a stand-in for a driver read ------------------------
// int read() { return 42; }

// --- [Slide 55] [Slide 89] Waypoint: a type you wrote --------------------------
// A plain aggregate. Lecture 6 covers declaring these; today just use it.
// struct Waypoint {
//     int x;
//     int y;
// };
//
// --- [Slide 89] a predicate: yes or no about one element ----------------------
// bool is_far(const Waypoint& w) { return w.x * w.x + w.y * w.y > 25.0; }

// --- [Slide 58] a predicate for std::erase_if -------------------------------
// bool is_stale(int reading) { return reading == 2; }

// --- [Slide 72] string_view: a parameter, and a bug ----------------------------
// void log(std::string_view msg) { std::cout << msg << '\n'; }
//
// std::string_view bad() {
//     std::string local{"a-long-name-on-the-heap"};
//     return local;  // the string dies here; the view outlives it
// }

// --- [Slide 81] a stand-in for a report function -------------------------------
// void report(const std::string& name, double value) {
//     std::cout << name << ": " << value << '\n';
// }

int main() {

    // #########################################################################
    // SECTION: THE TYPE OF EVERY SIZE, TIME COMPLEXITY
    // #########################################################################

    // --- [Slide 4] std::size_t is unsigned -----------------------------------
    // {
    //     std::size_t n{3};
    //     std::cout << n - 5 << '\n';  // not -2, but 18446744073709551614
    //
    //     // for (std::size_t i{n - 1}; i >= 0; --i) { }   // never ends
    //     // -Wall: comparison of unsigned expression >= 0 is always true
    // }

    // --- [Slide 7] Example 1: one loop, O(n) -------------------------------------
    // {
    //     int n{1000};
    //     long count{0};
    //     for (int i{0}; i < n; ++i)
    //         ++count;  // stmt
    //     std::cout << count << '\n';  // 1000: exactly n. Double n and it doubles: 2000
    // }

    // --- [Slide 8] Example 2: two nested loops, O(n^2) ---------------------------
    // {
    //     int n{1000};
    //     long count{0};
    //     for (int i{0}; i < n; ++i)
    //         for (int j{0}; j < i; ++j)
    //             ++count;  // stmt
    //     std::cout << count << '\n';  // 499500: n(n-1)/2, about half of n^2.
    //                                  // Double n and it quadruples: 1999000
    // }

    // #########################################################################
    // SECTION: CONTAINERS
    // #########################################################################

    // --- [Slide 14] One interface, every container ---------------------------
    // {
    //     std::vector<int> c{10, 20, 30};
    //     std::cout << c.size() << ' ' << c.empty() << '\n';  // 3 0
    //     std::cout << c[1] << ' ' << c.at(1) << '\n';       // 20 20
    //     std::cout << c.front() << ' ' << c.back() << '\n';  // 10 30
    //     std::cout << *c.data() << ' ' << *c.begin() << '\n';  // 10 10
    // }

    // #########################################################################
    // SECTION: ARRAYS
    // #########################################################################

    // --- [Slide 16] Two kinds of array ---------------------------------------
    // {
    //     int c_deg[6]{};
    //     std::array<int, 6> s_deg{};
    //
    //     std::cout << sizeof(c_deg) << ' ' << sizeof(s_deg) << '\n';  // 24 24
    //     std::cout << s_deg.size() << '\n';  // 6: a member the C array lacks
    //     s_deg.fill(0);
    // }

    // --- [Slide 17] C-style array initialization --- UNDEFINED BEHAVIOR ----
    // Case 1 reads uninitialized memory. Build with the sanitizer to see it.
    // {
    //     int a[6];            // 1. garbage: six uninitialized values
    //     int b[6]{};          // 2. all six are 0
    //     int c[6]{10, 20, 30};// 3. 10 20 30 0 0 0  -- the rest are zeroed
    //     int d[]{10, 20, 30}; // 4. size deduced: d has 3 elements
    //
    //     std::cout << a[0] << '\n';  // UB: reading before writing
    //     std::cout << b[0] << ' ' << c[3] << ' ' << std::size(d) << '\n';  // 0 0 3
    // }

    // --- [Slide 18] Contiguity -----------------------------------------------
    // {
    //     int joint_deg[6]{10, 20, 30};
    //
    //     std::cout << &joint_deg[0] << '\n';  // 0x7ffd...a10
    //     std::cout << &joint_deg[1] << '\n';  // 0x7ffd...a14 <- 4 bytes on
    //     std::cout << &joint_deg[2] << '\n';  // 0x7ffd...a18 <- 8 bytes on
    // }

    // --- [Slide 19] Array decay ----------------------------------------------
    // {
    //     int joint_deg[6]{};
    //     std::cout << joint_deg << '\n';  // decays: 0x7ffd...a10
    //     int* p{joint_deg};               // decays: p holds &joint_deg[0]
    //
    //     // Where it does NOT happen:
    //     std::cout << sizeof(joint_deg) << '\n';  // 24: the array
    //     std::cout << &joint_deg << '\n';         // the whole array, int(*)[6]
    //     int (&r)[6]{joint_deg};                  // a reference keeps the length
    //     std::cout << sizeof(r) << ' ' << *p << '\n';  // 24 0
    // }

    // --- [Slide 20] Pointer arithmetic on an array ---------------------------
    // {
    //     int joint_deg[6]{10, 20, 30};
    //     int* p{joint_deg};  // decay: p is &joint_deg[0]
    //
    //     std::cout << p + 1 << '\n';         // one element on, 4 bytes
    //     std::cout << *(p + 2) << '\n';      // 30: step two elements, then read
    //     std::cout << joint_deg[2] << '\n';  // 30: the same two steps, with brackets
    // }

    // --- [Slide 21] One block, read two ways ---------------------------------
    // {
    //     int joint_deg[6]{10, 20, 30};
    //     std::cout << &joint_deg[2] << '\n';  // 0x7ffd...a18
    //     std::cout << joint_deg[2] << '\n';   // 30
    // }

    // --- [Slide 22] sizeof after decay ----------------------------------------
    // Uncomment report() at namespace scope first.
    // {
    //     int joint_deg[6]{};
    //     std::cout << sizeof(joint_deg) << '\n';                         // 24: array
    //     std::cout << sizeof(joint_deg) / sizeof(joint_deg[0]) << '\n';  // 6
    //     report(joint_deg);                                              // 8, then 2
    // }

    // --- [Slide 23] Array length, three ways ----------------------------------
    // {
    //     int c_deg[6]{};
    //     std::array<int, 6> s_deg{};
    //     int* p{c_deg};
    //
    //     std::cout << sizeof(c_deg) / sizeof(c_deg[0]) << '\n';  // 6
    //     std::cout << std::size(c_deg) << ' ' << std::size(s_deg) << '\n';  // 6 6
    //     std::cout << s_deg.size() << ' ' << std::ssize(c_deg) << '\n';     // 6 6
    //     std::cout << sizeof(p) / sizeof(p[0]) << '\n';  // 2: compiles, wrong answer
    //     // std::cout << std::size(p) << '\n';  // DOES NOT COMPILE: no size(int*&)
    // }

    // --- [Slide 24] Arrays sized at run time --- DOES NOT COMPILE ------------
    // A variable length array. GCC accepts it as an extension, but the course
    // build passes -pedantic-errors, so this block stops the build with
    // "error: ISO C++ forbids variable length array 'scan'". With plain
    // -Wpedantic it would only warn. std::array<int, n> never compiles at all:
    // its length is a template argument.
    // {
    //     int n{};
    //     std::cin >> n;
    //     int scan[n];  // compiles on GCC. Not C++.
    //     scan[0] = 1;
    //     std::cout << scan[0] << '\n';
    //     // std::array<int, n> s;  // DOES NOT COMPILE: n is not a constant
    // }

    // --- [Slide 25] std::array ------------------------------------------------
    // {
    //     std::array<int, 6> joint_deg{10, 20, 30};
    //
    //     std::cout << joint_deg.size() << '\n';   // 6
    //     std::cout << joint_deg.at(1) << '\n';    // 20, range-checked
    //     joint_deg.fill(0);                       // every element becomes 0
    //     std::cout << joint_deg[1] << '\n';       // 0
    // }

    // --- [Slide 26] Reading the angle brackets -------------------------------
    // {
    //     std::array<int, 6> joint_deg{};      // 6 ints
    //     std::array<double, 6> torque_nm{};   // 6 doubles: a different type
    //     std::cout << typeid(joint_deg).name() << '\n';
    //     std::cout << typeid(torque_nm).name() << '\n';  // not the same
    // }

    // --- [Slide 27] std::array initialization --- UNDEFINED BEHAVIOR -------
    // Case 1 is an aggregate with no constructor: garbage until braces zero it.
    // {
    //     std::array<int, 6> a;                // 1. garbage: no constructor runs
    //     std::array<int, 6> b{};              // 2. all six are 0: the braces do it
    //     std::array<int, 6> c{10, 20, 30};    // 3. 10 20 30 0 0 0
    //     std::array d{10, 20, 30};            // 4. deduces std::array<int, 3>
    //
    //     std::cout << a[0] << '\n';  // UB: reading before writing
    //     std::cout << b[0] << ' ' << c[3] << ' ' << d.size() << '\n';  // 0 0 3
    // }

    // --- [Slide 28] Visiting every element -------------------------------------
    // {
    //     std::array<double, 3> ranges_m{3.0, 1.0, 4.0};
    //
    //     for (const auto& r : ranges_m) {  // read-only, no copy: the default
    //         std::cout << r << ' ';
    //     }
    //     std::cout << '\n';
    //
    //     for (auto r : ranges_m) { r = 0.0; (void)r; }  // a copy: changes nothing
    //     std::cout << ranges_m[0] << '\n';        // 3
    //     for (auto& r : ranges_m) r = 0.0;        // the element itself
    //     std::cout << ranges_m[0] << '\n';        // 0
    // }

    // --- [Slide 29] Access and modification --- UNDEFINED BEHAVIOR ---------
    // {
    //     std::array<int, 6> joint_deg{};
    //
    //     joint_deg[9] = 1;         // UB: no check, no complaint
    //     joint_deg.at(9) = 1;      // throws std::out_of_range and ends the program
    //     int* p{joint_deg.data()}; // address of element 0: p[2] is joint_deg[2]
    //     std::cout << p[2] << '\n';
    // }

    // --- [Slide 31] Multidimensional: declaration ------------------------------
    // {
    //     int grid[3][4]{};
    //     std::array<std::array<int, 4>, 3> sgrid{};  // read it inside out
    //
    //     grid[1][2] = 1;
    //     sgrid[1][2] = 1;
    //
    //     std::cout << sizeof(grid) << ' ' << sizeof(sgrid) << '\n';   // 48 48
    //     std::cout << std::size(grid) << ' ' << sgrid.size() << '\n';  // 3 3: rows
    //     std::cout << sgrid.size() * sgrid[0].size() << '\n';          // 12: cells
    // }

    // --- [Slide 32] Row-major order --------------------------------------------
    // {
    //     int grid[3][4]{{0, 0, 1, -1}, {0, 0, 1, -1}, {1, 0, 0, 0}};
    //
    //     int* flat{&grid[0][0]};  // the whole grid is one block of 12
    //     std::cout << flat[1 * 4 + 2] << '\n';  // 1: grid[1][2] is element 6
    //     std::cout << grid[1][2] << '\n';       // 1: the same cell
    // }

    // --- [Slide 33] [Slide 34] With and against the layout ---------------------
    // {
    //     int grid[3][4]{};
    //
    //     for (int i{0}; i < 3; ++i)      // rows
    //         for (int j{0}; j < 4; ++j)  // columns: the last index inside
    //             grid[i][j] = 0;
    //
    //     for (int j{0}; j < 4; ++j)      // columns
    //         for (int i{0}; i < 3; ++i)  // rows: every step jumps a whole row
    //             grid[i][j] = 0;
    //
    //     std::cout << grid[2][3] << '\n';
    //     // On a 4000 x 4000 grid: 1.8 ms with the layout, 19 ms against it.
    // }

    // #########################################################################
    // SECTION: ITERATORS
    // #########################################################################

    // --- [Slide 36] A pointer is already an iterator --------------------------
    // {
    //     int joint_deg[6]{10, 20, 30, 40, 50, 60};
    //
    //     int* it{&joint_deg[0]};   // start at the first element
    //     ++it;                     // step forward by sizeof(int)
    //     std::cout << *it << '\n'; // 20
    // }

    // --- [Slide 37] Begin and end ----------------------------------------------
    // {
    //     std::array<int, 6> joint_deg{10, 20, 30, 40, 50, 60};
    //
    //     // the first element, and one past the last element
    //     std::array<int, 6>::iterator first{joint_deg.begin()};
    //     std::array<int, 6>::iterator last{joint_deg.end()};
    //
    //     std::cout << last - first << '\n';  // 6, the same as size()
    //     std::cout << *first << '\n';        // 10
    //     // std::cout << *last << '\n';      // UB: end() is a position, not an element
    // }

    // --- [Slide 39] Writing the loop, three ways -------------------------------
    // {
    //     std::array<double, 3> ranges_m{3.55, 1.28, 4.12};
    //
    //     // 1. by index
    //     for (std::size_t i{0}; i < ranges_m.size(); ++i)
    //         std::cout << ranges_m[i] << ' ';
    //     std::cout << '\n';
    //
    //     // 2. by iterator
    //     for (auto it{ranges_m.begin()}; it != ranges_m.end(); ++it)
    //         std::cout << *it << ' ';
    //     std::cout << '\n';
    //
    //     // 3. range-based: prefer this one
    //     for (const auto& r : ranges_m)
    //         std::cout << r << ' ';
    //     std::cout << '\n';
    // }

    // --- [Slide 40] Exercise 1: trace iter -------------------------------------
    // Three prints, four tests; end() is compared but never dereferenced.
    // {
    //     std::array<double, 3> ranges_m{3.55, 1.28, 4.12};
    //
    //     for (auto iter{ranges_m.begin()}; iter != ranges_m.end(); ++iter)
    //         std::cout << *iter << ' ';
    //     std::cout << '\n';
    // }

    // --- [Slide 41] Const iterators --- DOES NOT COMPILE ----------------------
    // {
    //     std::array<int, 6> joint_deg{};
    //
    //     auto it{joint_deg.begin()};    // int* -- may write through it
    //     auto ct{joint_deg.cbegin()};   // int const* -- may not
    //
    //     *it = 90;  // fine
    //     *ct = 90;  // error: assignment of read-only location
    //     std::cout << typeid(it).name() << ' ' << typeid(ct).name() << '\n';  // Pi PKi
    // }

    // #########################################################################
    // SECTION: VECTORS
    // #########################################################################

    // --- [Slide 43] A vector in memory ----------------------------------------
    // {
    //     std::vector<int> ranges_m{10, 20, 30};
    //     ranges_m.reserve(6);
    //     std::cout << ranges_m.capacity() << '\n';   // 6
    //     std::cout << sizeof(ranges_m) << '\n';      // 24: three pointers
    //
    //     std::vector<int> many(1'000'000);
    //     std::cout << sizeof(many) << '\n';          // 24, whatever it holds
    // }

    // --- [Slide 44] Who gives the memory back ---------------------------------
    // {
    //     std::vector<int> empty;
    //     std::cout << empty.capacity() << ' ' << empty.data() << '\n';  // 0 0: nothing owned
    //
    //     {
    //         std::vector<int> scoped(1000, 7);
    //         std::cout << scoped.size() << '\n';
    //     }  // the destructor frees the block here, on every way out
    // }

    // --- [Slide 45] The pointer to the block ----------------------------------
    // {
    //     std::vector<int> ranges_m{10, 20, 30};
    //     int* p{ranges_m.data()};  // == &ranges_m[0]
    //     std::cout << p << ' ' << &ranges_m[0] << '\n';  // the same address
    //     std::cout << *(p + 1) << '\n';                  // 20: pointer arithmetic
    // }

    // --- [Slide 46] Vector initialization -------------------------------------
    // {
    //     std::vector<int> a;       // empty
    //     std::vector<int> a2{};    // the same: empty, capacity 0
    //     std::vector<int> b(3, 7); // 7 7 7  -- three copies of 7
    //     std::vector<int> c{3, 7}; // 3 7    -- two elements
    //     std::vector<int> d(3);    // 0 0 0  -- three value-initialized
    //     std::vector<int> e{c};    // a copy of c
    //
    //     std::cout << a.size() << ' ' << a2.capacity() << ' ' << b.size() << ' '
    //               << c.size() << ' ' << d.size() << ' ' << e.size() << '\n';  // 0 0 3 2 3 2
    // }

    // --- [Slide 47] A grid of vectors ------------------------------------------
    // {
    //     std::size_t rows{3};   // from the map header, at run time
    //     std::size_t cols{4};
    //     std::vector<std::vector<int>> nested(rows, std::vector<int>(cols, 0));
    //     nested[1][2] = 7;      // reads like the 2D array
    //
    //     // four allocations: the rows are separate blocks
    //     std::cout << nested[0].data() << ' ' << nested[1].data() << '\n';
    //
    //     std::vector<int> flat(rows * cols, 0);  // one block, row-major
    //     flat[1 * cols + 2] = 7;
    //     std::cout << flat[6] << '\n';  // 7
    // }

    // --- [Slide 48] Size and capacity --- UNDEFINED BEHAVIOR at the end -----
    // {
    //     std::vector<int> v;
    //     v.push_back(1);
    //     v.push_back(2);
    //     v.push_back(3);
    //
    //     std::cout << v.size() << '/' << v.capacity() << '\n';  // 3/4
    //     std::cout << v[3] << '\n';  // UB: spare room is not an element
    // }

    // --- [Slide 51] [Slide 52] Exercise 2: trace size and capacity ----------------
    // {
    //     std::vector<int> readings;
    //
    //     for (int i{0}; i < 6; ++i) {
    //         readings.push_back(i);
    //         std::cout << readings.size() << "/" << readings.capacity() << '\n';
    //     }
    //     // 1/1 2/2 3/4 4/4 5/8 6/8: four reallocations, at pushes 0, 1, 2 and 4
    // }

    // --- [Slide 53] reserve ----------------------------------------------------
    // Uncomment read() at namespace scope first.
    // {
    //     std::vector<int> scan;
    //     scan.reserve(1080);  // one allocation, now: 0/1080
    //     const int* block{scan.data()};
    //     int moves{0};
    //     for (int i{0}; i < 1080; ++i) {
    //         scan.push_back(read());
    //         if (scan.data() != block) { block = scan.data(); ++moves; }
    //     }
    //     std::cout << moves << ' ' << scan.capacity() << '\n';  // 0 1080
    //     // Without the reserve: 12 reallocations, 2047 moves, capacity 2048.
    // }

    // --- [Slide 54] shrink_to_fit --------------------------------------------
    // {
    //     std::vector<int> v;
    //     for (int i{0}; i < 1000; ++i)
    //         v.push_back(i);
    //     std::cout << v.size() << '/' << v.capacity() << '\n';  // 1000/1024
    //     v.shrink_to_fit();
    //     std::cout << v.size() << '/' << v.capacity() << '\n';  // 1000/1000 on libstdc++
    // }

    // --- [Slide 55] push_back and emplace_back --------------------------------
    // Uncomment struct Waypoint at namespace scope first.
    // {
    //     std::vector<Waypoint> path;
    //
    //     path.push_back(Waypoint{1, 2});  // build a Waypoint, then move it in
    //     path.emplace_back(3, 4);         // build it directly in the vector
    //     std::cout << path.size() << ' ' << path[1].x << '\n';  // 2 3
    // }

    // --- [Slide 56] insert and emplace -----------------------------------------
    // {
    //     std::vector<int> v{10, 20, 30};
    //
    //     v.insert(v.begin() + 1, 15);   // 10 15 20 30
    //     v.emplace(v.begin(), 5);       // 5 10 15 20 30
    //     for (const auto& x : v) std::cout << x << ' ';
    //     std::cout << '\n';
    // }

    // --- [Slide 57] Deletion --------------------------------------------------
    // {
    //     std::vector<int> v{10, 20, 30, 40, 50};
    //
    //     v.pop_back();                          // 10 20 30 40  -- O(1), returns nothing
    //     auto it{v.erase(v.begin() + 1)};       // 10 30 40     -- it now points at 30
    //     std::cout << *it << '\n';              // 30
    //     v.erase(v.begin(), v.begin() + 2);     // 40           -- erase a range
    //     std::cout << v.size() << '/' << v.capacity() << '\n';  // 1/5
    //     v.clear();                             // empty
    //     std::cout << v.size() << '/' << v.capacity() << '\n';  // 0/5: the block stays
    // }

    // --- [Slide 58] Erasing by value ------------------------------------------
    // Uncomment is_stale() at namespace scope first.
    // {
    //     std::vector<int> v{1, 2, 3, 2, 5, 2};
    //
    //     // the erase-remove idiom -- you will meet it in older code
    //     // v.erase(std::remove(v.begin(), v.end(), 2), v.end());
    //
    //     std::cout << std::erase(v, 2) << '\n';  // 3: how many were removed
    //     for (const auto& x : v) std::cout << x << ' ';   // 1 3 5
    //     std::cout << '\n';
    //
    //     v = {1, 2, 3, 2, 5, 2};
    //     std::erase_if(v, is_stale);
    //     std::cout << v.size() << '\n';  // 3
    // }

    // --- [Slide 60] Iterator invalidation --- UNDEFINED BEHAVIOR -------------
    // Build with the sanitizer: ERROR: AddressSanitizer: heap-use-after-free.
    // {
    //     std::vector<int> ranges_m{10, 20, 30};
    //     int* first{&ranges_m[0]};
    //     ranges_m.push_back(40);          // reallocates: the old block is freed
    //     std::cout << *first << '\n';     // reads freed memory
    //     // Fix: ranges_m.reserve(4) before taking the pointer.
    // }

    // #########################################################################
    // SECTION: STRINGS
    // #########################################################################

    // --- [Slide 62] C-strings: what auto deduces ------------------------------
    // {
    //     auto name{"John Doe"};
    //     std::cout << typeid(name).name() << '\n';  // PKc: char const*
    //     std::cout << std::strlen(name) << '\n';    // 8: walks to the '\0'
    //
    //     using namespace std::literals;
    //     auto name2{"John Doe"s};                   // std::string
    //     std::cout << name2.size() << '\n';         // 8, stored: O(1)
    // }

    // --- [Slide 63] Where the characters live --- DOES NOT COMPILE ------------
    // {
    //     auto name{"John Doe"};
    //     std::cout << &name << ' ' << static_cast<const void*>(name) << '\n';
    //     // the pointer is on the stack (0x7ffe...); the chars are in .rodata (0x6...)
    //     name = "Jane Doe";  // repoints name. "John Doe" is untouched
    //     std::cout << name << '\n';
    //     name[0] = 'j';      // error: assignment of read-only location
    // }

    // --- [Slide 64] The terminator ---------------------------------------------
    // {
    //     char name[]{"lidar"};   // a copy on the stack, 6 bytes
    //     std::cout << sizeof(name) << '\n';        // 6: array, terminator included
    //     std::cout << std::strlen(name) << '\n';   // 5: walks to '\0' on each call
    //     name[0] = 'L';                            // allowed: this copy is yours
    //     std::cout << name << '\n';                // Lidar
    // }

    // --- [Slide 65] std::string, the same interface as vector -----------------
    // {
    //     std::string topic{"/robot/scan"};
    //
    //     std::cout << topic.size() << ' ' << topic.empty() << ' ' << topic.capacity() << '\n';
    //     std::cout << topic[0] << topic.at(0) << topic.front() << topic.back() << '\n';
    //     topic.push_back('!');
    //     topic.reserve(64);
    //     for (char c : topic) std::cout << c;
    //     std::cout << '\n';
    //
    //     // appending one character at a time: capacity 15, 30, 60, 120
    //     std::string s;
    //     std::size_t last_cap{s.capacity()};
    //     for (int i{0}; i < 100; ++i) {
    //         s.push_back('x');
    //         if (s.capacity() != last_cap) { last_cap = s.capacity(); std::cout << last_cap << ' '; }
    //     }
    //     std::cout << '\n';
    // }

    // --- [Slide 66] [Slide 67] [Slide 68] Small string optimization ------------
    // {
    //     std::cout << sizeof(std::string) << '\n';  // 32, whatever the text is
    //     std::string s{"imu"};
    //     std::string l{"a-very-long-sensor-name"};
    //     std::cout << s.capacity() << ' ' << l.capacity() << '\n';  // 15 23: 15 belongs to libstdc++
    // }

    // --- [Slide 69] Common operations -------------------------------------------
    // {
    //     std::string topic{"/robot"};
    //
    //     topic += "/scan";              // append in place
    //     topic.append("/filtered");     // the same thing, spelled out
    //     topic.insert(0, "/tf");        // insert at a position
    //     topic.erase(0, 3);             // erase 3 characters from index 0
    //     topic.replace(0, 6, "/base");  // replace a range with new text
    //     std::cout << topic << '\n';    // /base/scan/filtered
    // }

    // --- [Slide 70] Searching ---------------------------------------------------
    // {
    //     std::string topic{"/robot/scan"};
    //
    //     std::cout << topic.find("scan") << '\n';   // 7: index where it starts
    //     std::cout << topic.find("imu") << '\n';    // npos: 18446744073709551615
    //     std::cout << topic.substr(7) << '\n';      // scan
    //
    //     if (topic.find("imu") != std::string::npos) std::cout << "found\n";
    //     else std::cout << "not found\n";
    // }

    // --- [Slide 71] Input: the getline trap --------------------------------------
    // Type 7, Enter, /robot/scan, Enter. Without std::ws the topic is EMPTY.
    // {
    //     int id{};
    //     std::string topic;
    //
    //     std::cin >> id;                  // reads 7, leaves the newline in the buffer
    //     std::cin >> std::ws;             // the fix: eat the whitespace first
    //     std::getline(std::cin, topic);   // reads the whole next line
    //     std::cout << id << " [" << topic << "]\n";
    // }

    // --- [Slide 72] String views --- UNDEFINED BEHAVIOR in bad() -------------
    // Uncomment log() and bad() at namespace scope first.
    // {
    //     std::string owned{"/robot/scan"};
    //     log(owned);           // a std::string
    //     log("a literal");     // a literal: no temporary string is built
    //
    //     std::string_view dangling{bad()};  // the view outlives the string
    //     std::cout << dangling << '\n';     // UB: ASan reports stack-use-after-return
    // }

    // #########################################################################
    // SECTION: MAPS
    // #########################################################################

    // --- [Slide 74] Keyed lookup -----------------------------------------------
    // {
    //     std::vector<std::pair<std::string, double>> sensors{{"lidar", 0.25}, {"imu", 0.01}};
    //     for (const auto& s : sensors)  // O(n), every lookup
    //         if (s.first == "lidar") std::cout << s.second << '\n';
    //
    //     std::map<std::string, double> keyed{{"lidar", 0.25}, {"imu", 0.01}};
    //     std::cout << keyed.at("lidar") << '\n';  // 0.25 -- O(log n)
    // }

    // --- [Slide 75] Ordered maps -----------------------------------------------
    // {
    //     std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};
    //
    //     for (const auto& [name, period] : sensors)
    //         std::cout << name << ' ';  // imu lidar   -- sorted, not the order written
    //     std::cout << '\n';
    // }

    // --- [Slide 76] Insertion into a map -----------------------------------------
    // {
    //     std::map<std::string, double> sensors{{"imu", 0.01}};
    //
    //     auto r{sensors.insert({"imu", 99.0})};  // key exists: nothing changes
    //     std::cout << r.second << ' ' << sensors.at("imu") << '\n';  // 0 0.01: not inserted
    //
    //     auto s{sensors.insert_or_assign("imu", 99.0)};  // overwrites
    //     std::cout << s.second << ' ' << sensors.at("imu") << '\n';  // 0 99: not inserted, but assigned
    // }

    // --- [Slide 77] Subscript inserts -------------------------------------------
    // {
    //     std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};
    //
    //     double period{sensors["camera"]};  // "camera" is not in the map: {"camera", 0} is inserted
    //     std::cout << period << ' ' << sensors.size() << '\n';  // 0 3: a READ made the map bigger
    // }

    // --- [Slide 78] The four right tools -----------------------------------------
    // None of these four insert anything.
    // {
    //     std::map<std::string, double> m{{"lidar", 0.25}};
    //
    //     std::cout << m.count("imu") << ' ' << m.contains("imu") << '\n';  // 0 0
    //     if (auto it{m.find("imu")}; it != m.end()) std::cout << it->second << '\n';
    //     else std::cout << "no imu\n";
    //     // m.at("imu");  // throws std::out_of_range: "map::at"
    //     std::cout << m.size() << '\n';  // still 1
    // }

    // --- [Slide 79] Iterating a map ---------------------------------------------
    // {
    //     std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};
    //
    //     for (const auto& entry : sensors)  // the long way
    //         std::cout << entry.first << ' ' << entry.second << '\n';
    //
    //     for (const auto& [name, period] : sensors)  // C++17, readable
    //         std::cout << name << ' ' << period << '\n';
    // }

    // --- [Slide 80] Unordered maps ---------------------------------------------
    // {
    //     std::unordered_map<std::string, double> sensors{
    //         {"camera", 0.05}, {"imu", 0.01}, {"lidar", 0.25}};
    //
    //     for (const auto& [name, period] : sensors)
    //         std::cout << name << ' ';  // lidar imu camera   -- no order promised
    //     std::cout << '\n';
    //     std::cout << sensors.bucket_count() << ' ' << sensors.load_factor() << '\n';  // 13 0.23
    // }

    // --- [Slide 81] Worked example: a sensor registry ----------------------------
    // Uncomment report(const std::string&, double) at namespace scope first.
    // {
    //     std::unordered_map<std::string, std::vector<double>> readings;
    //
    //     readings["lidar"].push_back(2.31);  // insert if absent
    //     readings["lidar"].push_back(2.28);
    //     readings["imu"].push_back(0.04);
    //
    //     if (auto it{readings.find("gps")}; it != readings.end())
    //         report(it->first, it->second.back());
    //     else
    //         std::cout << "no gps readings yet\n";
    //
    //     for (const auto& [name, values] : readings)
    //         std::cout << name << ": " << values.size() << " samples\n";
    // }

    // #########################################################################
    // SECTION: STL ALGORITHMS
    // #########################################################################

    // --- [Slide 87] Algorithms take iterators ------------------------------------
    // {
    //     std::string topic{"/robot/scan"};
    //     int arr[]{3, 9, 4};
    //     std::vector<int> v{5, 1, 8};
    //
    //     auto pos{std::find(topic.begin(), topic.end(), 's')};   // a std::string
    //     std::cout << pos - topic.begin() << '\n';                // 7
    //     std::cout << *std::max_element(std::begin(arr), std::end(arr)) << '\n';  // 9: a C array
    //     std::cout << *std::max_element(v.begin(), v.end()) << '\n';              // 8: a std::vector
    // }

    // --- [Slide 88] The ones you will use ---------------------------------------
    // {
    //     std::vector<double> ranges_m{2.31, 0.42, 5.00, 0.18, 1.75};
    //
    //     std::sort(ranges_m.begin(), ranges_m.end());                     // 0.18 0.42 1.75 2.31 5
    //     auto hit{std::find(ranges_m.begin(), ranges_m.end(), 5.00)};    // iterator, or end()
    //     std::cout << (hit != ranges_m.end()) << '\n';                    // 1
    //     std::cout << std::count(ranges_m.begin(), ranges_m.end(), 5.00) << '\n';   // 1
    //     std::cout << *std::min_element(ranges_m.begin(), ranges_m.end()) << '\n';  // 0.18
    //     std::cout << std::accumulate(ranges_m.begin(), ranges_m.end(), 0.0) << '\n';  // 9.66
    //     std::cout << std::accumulate(ranges_m.begin(), ranges_m.end(), 0) << '\n';    // 8: watch the seed
    //     std::ranges::sort(ranges_m);  // C++20: one argument instead of two
    // }

    // --- [Slide 89] Predicates ---------------------------------------------------
    // Uncomment struct Waypoint and is_far() at namespace scope first.
    // {
    //     std::vector<Waypoint> path{{1, 2}, {3, 4}, {6, 0}, {0, 1}, {5, 5}};
    //
    //     std::cout << std::count_if(path.begin(), path.end(), is_far) << '\n';  // 2
    //     auto far{std::find_if(path.begin(), path.end(), is_far)};              // -> (6, 0)
    //     std::cout << far - path.begin() << '\n';                               // index 2
    // }

    return 0;
}
