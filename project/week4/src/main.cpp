/**
 * @file main.cpp
 * @brief Every code snippet from the L4 slides (The Standard Library and Its
 *        Containers), switched off, so you can try them one at a time.
 *
 * @details Build target: @c week4. This file stands alone: it includes no
 * project header and is linked with nothing else.
 *
 * @par How to use it
 * Every snippet sits between @c "#if 0" and @c "#endif", so the compiler skips
 * it. To try one, change its @c "#if 0" to @c "#if 1", build, run, then set it
 * back to @c "#if 0" and move on. Only that one line changes: the explanation
 * around a block is ordinary comments, which stay comments whatever you do.
 *
 * @par Tags
 * @c "[Slide N]" is the frame number printed in the top-left corner of the
 * slide. @c "[Appendix: Title]" is a frame in the appendix, after the summary,
 * named by the title in the appendix index.
 *
 * @par Where the snippets live
 * Snippets that are whole functions or types live at namespace scope, above
 * @c main(). Snippets that are statements live inside @c main(), each in its
 * own @c { } block so the same variable name can be reused from one to the
 * next. Where a block needs both, the comment says which other block to
 * enable with it.
 *
 * @note Blocks marked "DOES NOT COMPILE" are on the slides to show you an
 *       error. Enable them on purpose, read the message, set them back.
 * @warning Blocks marked "UNDEFINED BEHAVIOR" are the point of the lecture,
 *          not accidents. They may crash, print garbage, print the right
 *          answer, or appear to work. Build them with AddressSanitizer, which
 *          names the bug: uncomment the two @c -fsanitize lines in
 *          @c project/week4/CMakeLists.txt.
 * @note Every number in the comments was measured on the course machine,
 *       g++ 13.3 with libstdc++ and @c -std=c++20, on 2026-09-28. Where the
 *       standard leaves a choice to the implementation (capacities, the SSO
 *       size, the bucket count), the comment says so. Addresses differ on
 *       every run; only their order and spacing are the point.
 */

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

// =============================================================================
// NAMESPACE SCOPE
//
// A function or a type cannot be defined inside main(), so every snippet
// that is one lives here. Enable it together with the matching block inside
// main().
// =============================================================================

// #############################################################################
// SECTION: ARRAYS
// #############################################################################

// --- [Slide 22] sizeof after decay ------------------------------------------
// GCC warns here under -Wall: "'sizeof' on array function parameter
// 'joint_deg' will return size of 'int*'". That warning is the lesson.
// Shares the name report with [Slide 85], but not the parameters, so the two
// are overloads and can be enabled together.
#if 0
void report(int joint_deg[6]) {  // the 6 is ignored: this is int*
    std::cout << sizeof(joint_deg) << '\n';                         // 8: pointer
    std::cout << sizeof(joint_deg) / sizeof(joint_deg[0]) << '\n';  // 2
}
#endif

// #############################################################################
// SECTION: VECTORS
// #############################################################################

// --- [Slide 53] reserve: a stand-in for a driver read -----------------------
#if 0
int read() { return 42; }
#endif

// --- [Slide 55] Waypoint: a type you wrote ----------------------------------
// A plain aggregate. Lecture 6 covers declaring these; today just use it.
#if 0
struct Waypoint {
    int x;
    int y;
};
#endif

// --- [Slide 58] A predicate for std::erase_if -------------------------------
// A predicate answers yes or no about one element.
#if 0
bool is_stale(int reading) { return reading == 2; }
#endif

// #############################################################################
// SECTION: STRINGS
// #############################################################################

// --- [Slide 75] String views: one parameter for every kind of string --------
#if 0
void log(std::string_view msg) {
    std::cout << msg << " (" << msg.size() << " chars)\n";
}
#endif

// --- [Slide 76] Dangling views ----------------------------------------------
// The text is longer than 15 characters, so it is on the heap.
#if 0
std::string_view bad() {
    std::string local{"a-long-name-on-the-heap"};
    return local;  // local is destroyed here; the view is left dangling
}
#endif

// #############################################################################
// SECTION: MAPS
// #############################################################################

// --- [Slide 85] Worked example: a stand-in for a report function ------------
// Shares the name report with [Slide 22]: see there.
#if 0
void report(const std::string& name, double value) {
    std::cout << name << ": " << value << '\n';
}
#endif

/**
 * @brief Runs whichever snippets are switched on. With none, it does nothing.
 * @return 0.
 */
int main() {
    // #########################################################################
    // SECTION: THE TYPE OF EVERY SIZE
    // #########################################################################

    // --- [Slide 4] std::size_t is unsigned -----------------------------------
    // The loop is left commented: it never ends. -Wall says so:
    // "comparison of unsigned expression in '>= 0' is always true".
#if 0
    {
        std::size_t n{3};
        std::cout << n - 5 << '\n';  // not -2, but 18446744073709551614

        // for (std::size_t i{n - 1}; i >= 0; --i) { }  // never ends
    }
#endif

    // #########################################################################
    // SECTION: TIME COMPLEXITY
    // #########################################################################

    // --- [Slide 7] Example 1: one loop, O(n) ---------------------------------
    // stmt is a counter, so you can see how many times it ran.
#if 0
    {
        int n{1000};
        long count{0};
        for (int i{0}; i < n; ++i)
            ++count;  // stmt
        std::cout << count << '\n';  // 1000: exactly n. Double n and it doubles: 2000
    }
#endif

    // --- [Slide 8] Example 2: two nested loops, O(n^2) -----------------------
#if 0
    {
        int n{1000};
        long count{0};
        for (int i{0}; i < n; ++i)
            for (int j{0}; j < i; ++j)
                ++count;  // stmt
        std::cout << count << '\n';  // 499500: n(n-1)/2, about half of n^2.
                                     // Double n and it quadruples: 1999000
    }
#endif

    // #########################################################################
    // SECTION: THE STANDARD LIBRARY AND THE STL
    // #########################################################################

    // --- [Slide 14] One interface, every container ---------------------------
#if 0
    {
        std::vector<int> c{10, 20, 30};
        std::cout << c.size() << ' ' << c.empty() << '\n';       // 3 0
        std::cout << c[1] << ' ' << c.at(1) << '\n';             // 20 20
        std::cout << c.front() << ' ' << c.back() << '\n';       // 10 30
        std::cout << *c.data() << ' ' << *c.begin() << '\n';     // 10 10
    }
#endif

    // #########################################################################
    // SECTION: ARRAYS
    // #########################################################################

    // --- [Slide 16] Two kinds of array ---------------------------------------
    // The slide names both joint_deg; here they need two names.
#if 0
    {
        int c_deg[6]{};               // inherited from C
        std::array<int, 6> s_deg{};   // std::array, since C++11

        std::cout << sizeof(c_deg) << ' ' << sizeof(s_deg) << '\n';  // 24 24
        std::cout << s_deg.size() << '\n';  // 6: a member the C array lacks
    }
#endif

    // --- [Slide 17] C-style array initialization --- UNDEFINED BEHAVIOR -----
    // Case 1 reads uninitialized memory. AddressSanitizer does not catch this
    // one; the compiler does: "'a' is used uninitialized" under -Wall.
#if 0
    {
        int a[6];              // 1. garbage: six uninitialized values
        int b[6]{};            // 2. all six are 0
        int c[6]{10, 20, 30};  // 3. 10 20 30 0 0 0: the rest are zeroed
        int d[]{10, 20, 30};   // 4. size deduced: d has 3 elements

        std::cout << a[0] << '\n';  // UB: reading before writing
        std::cout << b[0] << ' ' << c[3] << ' ' << std::size(d) << '\n';  // 0 0 3
    }
#endif

    // --- [Slide 18] Contiguity -----------------------------------------------
#if 0
    {
        int joint_deg[6]{10, 20, 30};

        std::cout << &joint_deg[0] << '\n';  // 0x7ffd...a10
        std::cout << &joint_deg[1] << '\n';  // 0x7ffd...a14 <- 4 bytes on
        std::cout << &joint_deg[2] << '\n';  // 0x7ffd...a18 <- 8 bytes on
    }
#endif

    // --- [Slide 19] Array decay, and where it does not happen ----------------
#if 0
    {
        int joint_deg[6]{};
        std::cout << joint_deg << '\n';  // decays: 0x7ffd...a10
        int* p{joint_deg};               // decays: p holds &joint_deg[0]

        // Where it does NOT happen:
        std::cout << sizeof(joint_deg) << '\n';  // 24: the array
        std::cout << &joint_deg << '\n';         // the whole array, int(*)[6]
        int (&r)[6]{joint_deg};                  // a reference keeps the length
        std::cout << sizeof(r) << ' ' << *p << '\n';  // 24 0
    }
#endif

    // --- [Slide 20] Pointer arithmetic on an array ---------------------------
#if 0
    {
        int joint_deg[6]{10, 20, 30};
        int* p{joint_deg};  // decay: p is &joint_deg[0], 0x7ffd...a10

        std::cout << p + 1 << '\n';         // 0x7ffd...a14: one element on, 4 bytes
        std::cout << *(p + 2) << '\n';      // 30: step two elements, then read
        std::cout << joint_deg[2] << '\n';  // 30: the same two steps, with brackets
    }
#endif

    // --- [Slide 21] One block, read two ways ---------------------------------
#if 0
    {
        int joint_deg[6]{10, 20, 30};
        std::cout << &joint_deg[2] << '\n';  // 0x7ffd...a18
        std::cout << joint_deg[2] << '\n';   // 30
    }
#endif

    // --- [Slide 22] sizeof after decay ---------------------------------------
    // Enable the [Slide 22] report() at namespace scope first.
#if 0
    {
        int joint_deg[6]{};
        std::cout << sizeof(joint_deg) << '\n';                         // 24: array
        std::cout << sizeof(joint_deg) / sizeof(joint_deg[0]) << '\n';  // 6
        report(joint_deg);                                              // 8, then 2
    }
#endif

    // --- [Slide 23] Array length ---------------------------------------------
    // The last line is left commented: it does not compile, which is the point.
    // error: no matching function for call to 'size(int*&)'
    // The sizeof(p) line compiles, but GCC warns under -Wall: "division
    // 'sizeof (int*) / sizeof (int)' does not compute the number of array
    // elements".
#if 0
    {
        int c_deg[6]{};
        std::array<int, 6> s_deg{};
        int* p{c_deg};

        std::cout << sizeof(c_deg) / sizeof(c_deg[0]) << '\n';              // 6
        std::cout << std::size(c_deg) << ' ' << std::size(s_deg) << '\n';  // 6 6
        std::cout << s_deg.size() << ' ' << std::ssize(c_deg) << '\n';     // 6 6
        std::cout << sizeof(p) / sizeof(p[0]) << '\n';  // 2: compiles, wrong answer
        // std::cout << std::size(p) << '\n';  // DOES NOT COMPILE
    }
#endif

    // --- [Slide 24] Arrays sized at run time --- DOES NOT COMPILE ------------
    // A variable length array. GCC accepts it as an extension, but the course
    // build passes -pedantic-errors, so this block stops the build with
    // "error: ISO C++ forbids variable length array 'scan'". With plain
    // -Wpedantic it would only warn. std::array<int, n> never compiles at all:
    // its length is a template argument.
#if 0
    {
        int n{};
        std::cin >> n;
        int scan[n];  // compiles on GCC. Not C++.
        scan[0] = 1;
        std::cout << scan[0] << '\n';
        // std::array<int, n> s;  // DOES NOT COMPILE: n is not a constant
    }
#endif

    // --- [Slide 25] std::array -----------------------------------------------
#if 0
    {
        std::array<int, 6> joint_deg{10, 20, 30};

        std::cout << joint_deg.size() << '\n';  // 6
        std::cout << joint_deg.at(1) << '\n';   // 20, range-checked
        joint_deg.fill(0);                      // every element becomes 0
        std::cout << joint_deg[1] << '\n';      // 0
    }
#endif

    // --- [Slide 26] Reading the angle brackets -------------------------------
    // The two are different types, so one cannot be assigned to the other.
#if 0
    {
        std::array<int, 6> joint_deg{};     // 6 ints
        std::array<double, 6> torque_nm{};  // 6 doubles: a different type

        // joint_deg = torque_nm;  // DOES NOT COMPILE: no match for 'operator='
        std::cout << joint_deg.size() << ' ' << torque_nm.size() << '\n';  // 6 6
    }
#endif

    // --- [Slide 27] std::array initialization --- UNDEFINED BEHAVIOR --------
    // Case 1 is an aggregate with no constructor: garbage until braces zero it.
    // As on [Slide 17], the sanitizer says nothing here. Read the warnings.
#if 0
    {
        std::array<int, 6> a;              // 1. garbage: no constructor runs
        std::array<int, 6> b{};            // 2. all six are 0: the braces do it
        std::array<int, 6> c{10, 20, 30};  // 3. 10 20 30 0 0 0
        std::array d{10, 20, 30};          // 4. deduces std::array<int, 3>

        std::cout << a[0] << '\n';  // UB: reading before writing
        std::cout << b[0] << ' ' << c[3] << ' ' << d.size() << '\n';  // 0 0 3
    }
#endif

    // --- [Slide 28] Visiting every element -----------------------------------
    // Three ways to write the loop variable.
#if 0
    {
        std::array<double, 3> ranges_m{3.0, 1.0, 4.0};

        for (const auto& r : ranges_m) {  // read-only, no copy: the default
            std::cout << r << ' ';        // 3 1 4
        }
        std::cout << '\n';

        for (auto r : ranges_m) {  // a copy: changes nothing
            r = 0.0;
            (void)r;  // silences "variable 'r' set but not used", which is the point
        }
        std::cout << ranges_m[0] << '\n';  // 3

        for (auto& r : ranges_m) {  // the element itself
            r = 0.0;
        }
        std::cout << ranges_m[0] << '\n';  // 0
    }
#endif

    // --- [Slide 29] Access and modification --- UNDEFINED BEHAVIOR ----------
    // As written, the write to joint_deg[9] lands outside the array and the
    // program crashed: "Segmentation fault". The sanitizer names it:
    // stack-buffer-overflow. Comment that line out and .at(9) throws instead:
    // "terminate called after throwing an instance of 'std::out_of_range'".
    // Comment out both to reach the last two.
#if 0
    {
        std::array<int, 6> joint_deg{};

        joint_deg[9] = 1;           // UB: no check, no complaint
        joint_deg.at(9) = 1;        // throws std::out_of_range
        int* p{joint_deg.data()};   // address of element 0: p[2] is joint_deg[2]
        std::cout << p[2] << '\n';  // 0
    }
#endif

    // --- [Slide 31] Multidimensional: declaration ----------------------------
    // The slide names both grid; here the std::array one is sgrid.
    // std::array<std::array<int, 4>, 3>: read it inside out.
#if 0
    {
        int grid[3][4]{};
        std::array<std::array<int, 4>, 3> sgrid{};

        grid[1][2] = 1;
        sgrid[1][2] = 1;

        std::cout << sizeof(grid) << ' ' << sizeof(sgrid) << '\n';    // 48 48
        std::cout << std::size(grid) << ' ' << sgrid.size() << '\n';  // 3 3: rows
        std::cout << sgrid.size() * sgrid[0].size() << '\n';          // 12: cells
    }
#endif

    // --- [Slide 32] Row-major order ------------------------------------------
#if 0
    {
        int grid[3][4]{{0, 0, 1, -1}, {0, 0, 1, -1}, {1, 0, 0, 0}};

        int* flat{&grid[0][0]};                // the whole grid is one block of 12
        std::cout << flat[1 * 4 + 2] << '\n';  // 1: grid[1][2] is element 6
        std::cout << grid[1][2] << '\n';       // 1: the same cell
    }
#endif

    // --- [Slide 33] [Slide 34] With and against the layout -------------------
    // On a 4000 x 4000 grid: 1.8 ms with the layout, 19 ms against it.
#if 0
    {
        int grid[3][4]{};

        for (int i{0}; i < 3; ++i)      // rows
            for (int j{0}; j < 4; ++j)  // columns: the last index inside
                grid[i][j] = 0;

        for (int j{0}; j < 4; ++j)      // columns
            for (int i{0}; i < 3; ++i)  // rows: every step jumps a whole row
                grid[i][j] = 0;

        std::cout << grid[2][3] << '\n';  // 0
    }
#endif

    // #########################################################################
    // SECTION: ITERATORS
    // #########################################################################

    // --- [Slide 36] A pointer is already an iterator -------------------------
#if 0
    {
        int joint_deg[6]{10, 20, 30, 40, 50, 60};

        int* it{&joint_deg[0]};    // start at the first element
        ++it;                      // step forward by sizeof(int)
        std::cout << *it << '\n';  // 20
    }
#endif

    // --- [Slide 37] [Slide 38] Begin and end, one past the end ---------------
#if 0
    {
        std::array<int, 6> joint_deg{10, 20, 30, 40, 50, 60};

        // the first element, and one past the last element
        std::array<int, 6>::iterator first{joint_deg.begin()};
        std::array<int, 6>::iterator last{joint_deg.end()};

        std::cout << last - first << '\n';  // 6, the same as size()
        std::cout << *first << '\n';        // 10
        // std::cout << *last << '\n';      // UB: end() is a position, not an element
    }
#endif

    // --- [Slide 39] Writing the loop -----------------------------------------
#if 0
    {
        std::array<double, 3> ranges_m{3.55, 1.28, 4.12};

        // 1. by index
        for (std::size_t i{0}; i < ranges_m.size(); ++i)
            std::cout << ranges_m[i] << ' ';
        std::cout << '\n';

        // 2. by iterator
        for (auto it{ranges_m.begin()}; it != ranges_m.end(); ++it)
            std::cout << *it << ' ';
        std::cout << '\n';

        // 3. range-based
        for (const auto& r : ranges_m)
            std::cout << r << ' ';
        std::cout << '\n';  // all three: 3.55 1.28 4.12
    }
#endif

    // --- [Slide 40] Exercise 1: writing the loop -----------------------------
    // Trace iter on paper first: three prints, four tests of iter != end().
    // end() is compared but never dereferenced.
#if 0
    {
        std::array<double, 3> ranges_m{3.55, 1.28, 4.12};

        // 2. by iterator
        for (auto iter{ranges_m.begin()}; iter != ranges_m.end(); ++iter)
            std::cout << *iter << ' ';
        std::cout << '\n';  // 3.55 1.28 4.12
    }
#endif

    // --- [Slide 41] Const iterators --- DOES NOT COMPILE ---------------------
    // error: assignment of read-only location '* ct'
    // Delete the *ct line to build it.
#if 0
    {
        std::array<int, 6> joint_deg{};

        auto it{joint_deg.begin()};   // int*: may write through it
        auto ct{joint_deg.cbegin()};  // int const*: may not

        *it = 90;  // fine
        *ct = 90;  // error: assignment of read-only location
    }
#endif

    // #########################################################################
    // SECTION: VECTORS
    // #########################################################################

    // --- [Slide 43] A vector in memory ---------------------------------------
#if 0
    {
        std::vector<int> ranges_m{10, 20, 30};
        ranges_m.reserve(6);
        std::cout << ranges_m.capacity() << '\n';  // 6
        std::cout << sizeof(ranges_m) << '\n';     // 24: three pointers

        std::vector<int> many(1'000'000);
        std::cout << sizeof(many) << '\n';  // 24, whatever it holds
    }
#endif

    // --- [Slide 44] Who gives the memory back --------------------------------
#if 0
    {
        std::vector<int> empty;
        std::cout << empty.capacity() << ' ' << empty.data() << '\n';  // 0 0: nothing owned

        {
            std::vector<int> scoped(1000, 7);
            std::cout << scoped.size() << '\n';  // 1000
        }  // the destructor frees the block here, on every way out
    }
#endif

    // --- [Slide 45] The pointer to the block ---------------------------------
#if 0
    {
        std::vector<int> ranges_m{10, 20, 30};
        int* p{ranges_m.data()};  // == &ranges_m[0]
        std::cout << p << ' ' << &ranges_m[0] << '\n';  // the same heap address: 0x6311...2b0
        std::cout << *(p + 1) << '\n';                  // 20: pointer arithmetic
    }
#endif

    // --- [Slide 46] Vector initialization ------------------------------------
#if 0
    {
        std::vector<int> a;        // empty
        std::vector<int> a2{};     // the same: empty, capacity 0
        std::vector<int> b(3, 7);  // 7 7 7: three copies of 7
        std::vector<int> c{3, 7};  // 3 7: two elements
        std::vector<int> d(3);     // 0 0 0: three value-initialized
        std::vector<int> e{c};     // a copy of c

        std::cout << a.size() << ' ' << a2.capacity() << ' ' << b.size() << ' '
                  << c.size() << ' ' << d.size() << ' ' << e.size() << '\n';  // 0 0 3 2 3 2
    }
#endif

    // --- [Slide 47] A grid of vectors ----------------------------------------
#if 0
    {
        std::size_t rows{3};  // from the map header, at run time
        std::size_t cols{4};
        std::vector<std::vector<int>> nested(rows, std::vector<int>(cols, 0));
        nested[1][2] = 7;  // reads like the 2D array

        // four allocations: the rows are separate blocks
        std::cout << nested[0].data() << ' ' << nested[1].data() << '\n';

        std::vector<int> flat(rows * cols, 0);  // one block, row-major
        flat[1 * cols + 2] = 7;
        std::cout << flat[6] << '\n';  // 7
    }
#endif

    // --- [Slide 48] Size and capacity --- UNDEFINED BEHAVIOR at the end ------
#if 0
    {
        std::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);

        std::cout << v.size() << '/' << v.capacity() << '\n';  // 3/4: room for one more
        std::cout << v[3] << '\n';  // UB: spare room is not an element
    }
#endif

    // --- [Slide 51] [Slide 52] Exercise 2: trace size and capacity -----------
    // Write the six lines down before you run it. [Slide 52] is the answer.
#if 0
    {
        std::vector<int> readings;

        for (int i{0}; i < 6; ++i) {
            readings.push_back(i);
            std::cout << readings.size() << "/" << readings.capacity() << '\n';
        }
        // 1/1 2/2 3/4 4/4 5/8 6/8: four reallocations, at pushes 0, 1, 2 and 4
    }
#endif

    // --- [Slide 53] reserve --------------------------------------------------
    // Enable the [Slide 53] read() at namespace scope first.
    // Delete the reserve line to see the other count.
#if 0
    {
        std::vector<int> scan;
        scan.reserve(1080);  // one allocation, now: 0/1080
        const int* block{scan.data()};
        int moves{0};
        for (int i{0}; i < 1080; ++i) {
            scan.push_back(read());  // zero reallocations
            if (scan.data() != block) {
                block = scan.data();
                ++moves;
            }
        }
        std::cout << moves << ' ' << scan.capacity() << '\n';  // 0 1080
        // Without the reserve: 12 reallocations, capacity 2048.
    }
#endif

    // --- [Slide 54] Capacity given back --------------------------------------
    // shrink_to_fit is a request. libstdc++ honors it; the standard does not
    // promise that it will.
#if 0
    {
        std::vector<int> v;
        for (int i{0}; i < 1000; ++i)
            v.push_back(i);
        std::cout << v.size() << '/' << v.capacity() << '\n';  // 1000/1024
        v.shrink_to_fit();
        std::cout << v.size() << '/' << v.capacity() << '\n';  // 1000/1000: 24 slots returned
    }
#endif

    // --- [Slide 55] push_back and emplace_back -------------------------------
    // Enable struct Waypoint at namespace scope first.
#if 0
    {
        std::vector<Waypoint> path;

        path.push_back(Waypoint{1, 2});  // build a Waypoint, then move it in
        path.emplace_back(3, 4);         // build it directly in the vector
        std::cout << path.size() << ' ' << path[1].x << '\n';  // 2 3
    }
#endif

    // --- [Slide 56] insert and emplace ---------------------------------------
#if 0
    {
        std::vector<int> v{10, 20, 30};

        v.insert(v.begin() + 1, 15);  // 10 15 20 30
        v.emplace(v.begin(), 5);      // 5 10 15 20 30
        for (const auto& x : v)
            std::cout << x << ' ';
        std::cout << '\n';
    }
#endif

    // --- [Slide 57] Deletion -------------------------------------------------
#if 0
    {
        std::vector<int> v{10, 20, 30, 40, 50};

        v.pop_back();                     // 10 20 30 40: O(1), returns nothing
        auto it{v.erase(v.begin() + 1)};  // 10 30 40: it now points at 30
        std::cout << *it << '\n';         // 30
        v.erase(v.begin(), v.begin() + 2);                      // 40: erase a range
        std::cout << v.size() << '/' << v.capacity() << '\n';  // 1/5
        v.clear();                                              // empty
        std::cout << v.size() << '/' << v.capacity() << '\n';  // 0/5: the block stays
    }
#endif

    // --- [Slide 58] Erasing by value -----------------------------------------
    // Enable is_stale() at namespace scope first.
#if 0
    {
        std::vector<int> v{1, 2, 3, 2, 5, 2};

        // the erase-remove idiom: you will meet it in older code
        // v.erase(std::remove(v.begin(), v.end(), 2), v.end());

        std::cout << std::erase(v, 2) << '\n';  // 3: returns how many were removed
        for (const auto& x : v)
            std::cout << x << ' ';  // 1 3 5
        std::cout << '\n';

        v = {1, 2, 3, 2, 5, 2};
        std::erase_if(v, is_stale);
        std::cout << v.size() << '\n';  // 3
    }
#endif

    // --- [Slide 60] Iterator invalidation --- UNDEFINED BEHAVIOR -------------
    // Lecture 3's dangling pointer, renamed. With the sanitizer:
    // ERROR: AddressSanitizer: heap-use-after-free.
    // Fix: ranges_m.reserve(4) before taking the pointer.
#if 0
    {
        std::vector<int> ranges_m{10, 20, 30};
        int* first{&ranges_m[0]};
        ranges_m.push_back(40);         // reallocates: the old block is freed
        std::cout << *first << '\n';    // reads freed memory
    }
#endif

    // #########################################################################
    // SECTION: STRINGS
    // #########################################################################

    // --- [Slide 62] C-strings ------------------------------------------------
#if 0
    {
        char lidar[]{"OS1-64"};           // sizeof 7: your array, with the '\0'
        char camera[]{"BFS-U3-51S5"};     // sizeof 12: a longer array
        const char* radar{"ARS 408-21"};  // sizeof 8: the pointer
        auto imu{"BMI088"};               // sizeof 8: auto gives you the pointer

        std::cout << sizeof(lidar) << ' ' << sizeof(camera) << ' ' << sizeof(radar) << ' '
                  << sizeof(imu) << '\n';  // 7 12 8 8
    }
#endif

    // --- [Slide 63] const char*: immutable --- DOES NOT COMPILE --------------
    // error: assignment of read-only location '* radar'
    // Delete the radar[0] line to see the two addresses.
#if 0
    {
        const char* radar{"ARS 408-21"};  // pointer: stack
                                          // "ARS 408-21": .rodata
        std::cout << &radar << ' ' << static_cast<const void*>(radar) << '\n';
        // the pointer is on the stack (0x7ffe...); the characters are not (0x5...)

        radar = "ESR 2.5";  // moves the pointer
                            // "ARS 408-21" is untouched
        std::cout << radar << '\n';  // ESR 2.5

        radar[0] = 'a';  // error: assignment of read-only location
    }
#endif

    // --- [Slide 64] C-array: mutable -----------------------------------------
#if 0
    {
        char radar[]{"ARS 408-21"};  // a copy on the stack
        std::cout << sizeof(radar) << '\n';       // 11: array, terminator included
        std::cout << std::strlen(radar) << '\n';  // 10: walks to '\0' on each call

        radar[0] = 'a';               // allowed: this copy is yours
        std::cout << radar << '\n';  // aRS 408-21
    }
#endif

    // --- [Slide 65] The s suffix ---------------------------------------------
#if 0
    {
        using namespace std::literals;  // needed for the s suffix

        auto imu1{"BMI088"};   // const char*: just a pointer
        auto imu2{"BMI088"s};  // std::string

        std::cout << std::strlen(imu1) << '\n';  // 6: the '\0' is not counted
        std::cout << imu2.size() << '\n';        // 6: stored, O(1)

        imu2 += "-XS1";  // the string owns its characters, so it can grow
        std::cout << imu2 << ' ' << imu2.size() << '\n';  // BMI088-XS1 10
        std::cout << (imu2 == "BMI088-XS1") << '\n';      // 1: compares the characters
    }
#endif

    // --- [Slide 66] std::string, the same interface as vector ----------------
    // Appending one character at a time takes the capacity through
    // 15, 30, 60, 120 on libstdc++.
#if 0
    {
        std::string topic{"/robot/scan"};

        std::cout << topic.size() << ' ' << topic.empty() << ' ' << topic.capacity() << '\n';  // 11 0 15
        std::cout << topic[0] << topic.at(0) << topic.front() << topic.back() << '\n';  // ///n
        topic.push_back('s');
        topic.reserve(64);
        for (char c : topic)
            std::cout << c;  // /robot/scans
        std::cout << '\n';

        std::string s;
        std::size_t last_cap{s.capacity()};
        for (int i{0}; i < 100; ++i) {
            s.push_back('x');
            if (s.capacity() != last_cap) {
                last_cap = s.capacity();
                std::cout << last_cap << ' ';  // 30 60 120
            }
        }
        std::cout << '\n';
    }
#endif

    // --- [Slide 67] [Slide 68] [Slide 69] [Slide 70] SSO, then the heap ------
    // 15 characters fit inside the object: that number belongs to libstdc++.
    // data() shows where the characters are: inside imu, or on the heap.
#if 0
    {
        std::cout << sizeof(std::string) << '\n';  // 32 bytes, whatever the text is

        std::string imu{"BMI088"};  // [Slide 68]
        std::cout << imu.size() << ' ' << imu.capacity() << '\n';  // 6 15
        std::cout << &imu << ' ' << static_cast<const void*>(imu.data()) << '\n';  // 16 bytes apart

        imu = "ICM-42688-P";  // [Slide 69]
        std::cout << imu.size() << ' ' << imu.capacity() << '\n';  // 11 15

        imu = "Xsens MTi-630 AHRS";  // [Slide 70] On the heap
        std::cout << imu.size() << ' ' << imu.capacity() << '\n';  // 18 30
        std::cout << &imu << ' ' << static_cast<const void*>(imu.data()) << '\n';  // far apart
    }
#endif

    // --- [Slide 71] Common operations ----------------------------------------
#if 0
    {
        std::string topic{"/robot"};

        topic += "/scan";              // append in place
        topic.append("/filtered");     // the same thing, spelled out
        topic.insert(0, "/tf");        // insert at a position
        topic.erase(0, 3);             // erase 3 characters from index 0
        topic.replace(0, 6, "/base");  // replace a range with new text
        std::cout << topic << '\n';    // /base/scan/filtered
    }
#endif

    // --- [Slide 72] Searching ------------------------------------------------
#if 0
    {
        std::string topic{"/robot/scan"};

        std::cout << topic.find("scan") << '\n';  // 7: index where it starts
        std::cout << topic.find("imu") << '\n';   // npos: 18446744073709551615
        std::cout << topic.substr(7) << '\n';     // scan

        if (topic.find("imu") != std::string::npos)
            std::cout << "found\n";
        else
            std::cout << "not found\n";  // not found
    }
#endif

    // --- [Slide 73] Input: the getline trap ----------------------------------
    // Type 7 and Enter. The program never waits for the mission.
#if 0
    {
        int robot_id{};
        std::string mission;  // a whole line, spaces included

        std::cout << "Robot ID: ";
        std::cin >> robot_id;  // 7 and Enter: reads 7, the '\n' stays
        std::cout << "Mission: ";
        std::getline(std::cin, mission);  // meets '\n': mission is ""
        std::cout << '[' << mission << "]\n";  // []
    }
#endif

    // --- [Slide 74] Skipping whitespace with std::ws -------------------------
    // Type 7, Enter, inspect loading dock B, Enter.
#if 0
    {
        int robot_id{};
        std::string mission;

        std::cout << "Robot ID: ";
        std::cin >> robot_id;  // 7
        std::cout << "Mission: ";
        std::getline(std::cin >> std::ws, mission);  // "inspect loading dock B"
        std::cout << '[' << mission << "]\n";        // [inspect loading dock B]
    }
#endif

    // --- [Slide 75] String views ---------------------------------------------
    // Enable log() at namespace scope first.
#if 0
    {
        std::string status{"lidar ready"};
        const char* model{"OS1-64"};

        log(status);              // std::string: lidar ready (11 chars)
        log(model);               // const char*: OS1-64 (6 chars)
        log("radar ARS 408-21");  // literal: radar ARS 408-21 (16 chars)
    }
#endif

    // --- [Slide 76] Dangling views --- UNDEFINED BEHAVIOR --------------------
    // Enable bad() at namespace scope first. With the sanitizer:
    // ERROR: AddressSanitizer: heap-use-after-free.
#if 0
    {
        std::string_view v{bad()};
        std::cout << v << '\n';  // undefined behavior
    }
#endif

    // #########################################################################
    // SECTION: MAPS
    // #########################################################################

    // --- [Slide 78] Keyed lookup ---------------------------------------------
#if 0
    {
        std::vector<std::pair<std::string, double>> sensors{{"lidar", 0.25}, {"imu", 0.01}};

        for (const auto& s : sensors)  // O(n), every lookup
            if (s.first == "lidar")
                std::cout << s.second << '\n';  // 0.25

        std::map<std::string, double> keyed{{"lidar", 0.25}, {"imu", 0.01}};
        std::cout << keyed.at("lidar") << '\n';  // 0.25: O(log n)
    }
#endif

    // --- [Slide 79] Ordered maps ---------------------------------------------
#if 0
    {
        std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};

        for (const auto& [name, period] : sensors)
            std::cout << name << ' ';  // imu lidar: sorted, not the order written
        std::cout << '\n';
    }
#endif

    // --- [Slide 80] Insertion ------------------------------------------------
#if 0
    {
        std::map<std::string, double> sensors{{"imu", 0.01}};

        auto r{sensors.insert({"imu", 99.0})};  // key exists: nothing changes
        std::cout << r.second << ' ' << sensors.at("imu") << '\n';  // 0 0.01: not inserted

        auto s{sensors.insert_or_assign("imu", 99.0)};  // overwrites
        std::cout << s.second << ' ' << sensors.at("imu") << '\n';  // 0 99: not inserted, but assigned
    }
#endif

    // --- [Slide 81] Subscript inserts ----------------------------------------
#if 0
    {
        std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};

        double period{sensors["camera"]};  // "camera" is not in the map
                                           // {"camera", 0} is inserted
        std::cout << period << ' ' << sensors.size() << '\n';  // 0 3: a READ made the map bigger
    }
#endif

    // --- [Slide 82] The four right tools -------------------------------------
    // None of these four insert anything.
#if 0
    {
        std::map<std::string, double> m{{"lidar", 0.25}};

        std::cout << m.count("imu") << ' ' << m.contains("imu") << '\n';  // 0 0
        if (auto it{m.find("imu")}; it != m.end())
            std::cout << it->second << '\n';
        else
            std::cout << "no imu\n";  // no imu
        // m.at("imu");  // throws std::out_of_range: "map::at"
        std::cout << m.size() << '\n';  // still 1
    }
#endif

    // --- [Slide 83] Iterating a map ------------------------------------------
    // *sensors.begin() is a std::pair<const std::string, double>.
#if 0
    {
        std::map<std::string, double> sensors{{"lidar", 0.25}, {"imu", 0.01}};

        for (const auto& entry : sensors)  // the long way
            std::cout << entry.first << ' ' << entry.second << '\n';

        for (const auto& [name, period] : sensors)  // C++17, readable
            std::cout << name << ' ' << period << '\n';
        // both print: imu 0.01, then lidar 0.25
    }
#endif

    // --- [Slide 84] Unordered maps -------------------------------------------
    // The order and the bucket count belong to libstdc++; nothing promises them.
#if 0
    {
        std::unordered_map<std::string, double> sensors{
            {"camera", 0.05}, {"imu", 0.01}, {"lidar", 0.25}};

        for (const auto& [name, period] : sensors)
            std::cout << name << ' ';  // lidar imu camera: no order promised
        std::cout << '\n';
        std::cout << sensors.bucket_count() << ' ' << sensors.load_factor() << '\n';  // 13 0.230769
    }
#endif

    // --- [Slide 85] Worked example -------------------------------------------
    // Enable the [Slide 85] report() at namespace scope first.
#if 0
    {
        std::unordered_map<std::string, std::vector<double>> readings;

        readings["lidar"].push_back(2.31);  // insert if absent
        readings["lidar"].push_back(2.28);
        readings["imu"].push_back(0.04);
        // readings["gps"].push_back(0.322);

        if (auto it{readings.find("gps")}; it != readings.end())
            report(it->first, it->second.back());
        else
            std::cout << "no gps readings yet\n";  // no gps readings yet

        for (const auto& [name, values] : readings)
            std::cout << name << ": " << values.size() << " samples\n";  // imu: 1, lidar: 2
    }
#endif

    // #########################################################################
    // SECTION: STL ALGORITHMS
    // #########################################################################

    // --- [Slide 89] Algorithms take iterators --------------------------------
    // std::find(first, last, value) is the shape.
#if 0
    {
        std::string topic{"/robot/scan"};
        int arr[]{3, 9, 4};
        std::vector<int> v{5, 1, 8};

        auto pos{std::find(topic.begin(), topic.end(), 's')};  // a std::string
        std::cout << pos - topic.begin() << '\n';               // 7
        std::cout << *std::max_element(std::begin(arr), std::end(arr)) << '\n';  // 9: a C array
        std::cout << *std::max_element(v.begin(), v.end()) << '\n';              // 8: a std::vector
    }
#endif

    // --- [Slide 90] The ones you will use ------------------------------------
#if 0
    {
        std::vector<double> ranges_m{2.31, 0.42, 5.00, 0.18, 1.75};

        std::sort(ranges_m.begin(), ranges_m.end());                   // 0.18 0.42 1.75 2.31 5
        auto hit{std::find(ranges_m.begin(), ranges_m.end(), 5.00)};  // iterator, or end()
        std::cout << (hit != ranges_m.end()) << '\n';                  // 1
        std::cout << std::count(ranges_m.begin(), ranges_m.end(), 5.00) << '\n';   // 1
        std::cout << *std::min_element(ranges_m.begin(), ranges_m.end()) << '\n';  // 0.18
        std::cout << std::accumulate(ranges_m.begin(), ranges_m.end(), 0.0) << '\n';  // 9.66
        std::cout << std::accumulate(ranges_m.begin(), ranges_m.end(), 0) << '\n';    // 8: watch the seed
        std::ranges::sort(ranges_m);  // C++20: one argument instead of two
    }
#endif

    // #########################################################################
    // APPENDIX
    // #########################################################################

    // --- [Appendix: Three C-String Traps] 1. The terminator goes missing -----
    // UNDEFINED BEHAVIOR. With the sanitizer:
    // ERROR: AddressSanitizer: stack-buffer-overflow.
#if 0
    {
        char buf[5]{'l', 'i', 'd', 'a', 'r'};  // five characters, no '\0'
        std::cout << std::strlen(buf) << '\n';  // runs off the end: printed 5, by luck
    }
#endif

    // --- [Appendix: Three C-String Traps] 2. Writing through a literal -------
    // UNDEFINED BEHAVIOR. The slide has no code; this is what it describes.
    // On Linux it ends with "Segmentation fault", and the sanitizer reports
    // SEGV on unknown address.
#if 0
    {
        char* p{const_cast<char*>("lidar")};  // the const is cast away
        p[0] = 'L';                           // writes to read-only memory
        std::cout << p << '\n';               // never reached
    }
#endif

    // --- [Appendix: Three C-String Traps] 3. Comparing with == ---------------
    // GCC warns: "comparison between two arrays is deprecated in C++20".
#if 0
    {
        char a[]{"scan"};
        char b[]{"scan"};
        std::cout << (a == b) << '\n';                  // 0: compares addresses, not text
        std::cout << (std::strcmp(a, b) == 0) << '\n';  // 1: the comparison you wanted
    }
#endif

    // --- [Appendix: Printing a char Pointer] ---------------------------------
#if 0
    {
        const char* name{"lidar"};
        int value{42};

        std::cout << name << '\n';                            // lidar: the text
        std::cout << static_cast<const void*>(name) << '\n';  // 0x560e...07c: the address
        std::cout << &value << '\n';                          // 0x7ffe...4d0: the address
    }
#endif

    // --- [Appendix: The C-String Functions] ----------------------------------
    // UNDEFINED BEHAVIOR on the print. GCC warns: "output truncated copying 6
    // bytes from a string of length 10" [-Wstringop-truncation]. Without the
    // sanitizer it printed lidar_ by luck; with it: stack-buffer-overflow.
#if 0
    {
        char dst[6];
        std::strncpy(dst, "lidar_scan", 6);  // dst holds l i d a r _
                                             // and no terminator at all
        std::cout << dst << '\n';            // runs off the end
    }
#endif

    return 0;
}
