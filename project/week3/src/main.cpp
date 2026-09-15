// =============================================================================
// ENPM702 - Introductory Robot Programming
// Week 3 playground - L3: Pointers and Memory Management
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
//      appear to work. Run them under Valgrind, which is the only way to see
//      what actually happened:
//          valgrind --leak-check=full ./build/project/week3/week3
//   3. Build in Debug. Valgrind can only name the LINE that leaked when -g is
//      there, and Debug is the variant that adds it. [Slide 6]
// =============================================================================

#include <cstddef>
#include <functional>
#include <iostream>
#include <typeinfo>
#include <vector>

// =============================================================================
// NAMESPACE SCOPE
//
// A couple of snippets cannot live inside main(): a type and the function
// declarations that take it. Uncomment these together with the matching
// [Slide 71] block inside main().
// =============================================================================

// --- [Slide 71] Where references are actually used ---------------------------
// A stand-in for a real class. Classes are Lecture 8.
// struct Battery {
//     int charge_pct{0};
// };
//
// void charge(Battery& pack);        // may modify the caller's battery
// void report(const Battery& pack);  // reads it, copies nothing

int main() {

    // #########################################################################
    // SECTION: POINTERS
    // #########################################################################

    // --- [Slide 10] What a pointer is -----------------------------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};

    //     std::cout << "altitude_ptr holds   " << altitude_ptr << '\n';
    //     std::cout << "it lives at          " << &altitude_ptr << '\n';
    //     std::cout << "*altitude_ptr is     " << *altitude_ptr << '\n';
    // }
    //
    // Two different addresses are in play: the one the pointer HOLDS, and the
    // one the pointer LIVES AT. They have nothing to do with each other.

    // --- [Slide 14] Declaring a pointer ---------------------------------------
    // Read the declaration inside-out, starting at the identifier.
    // {
    //     int* altitude_ptr;      // altitude_ptr is a pointer to an int
    //     double* voltage_ptr;    // voltage_ptr is a pointer to a double
    //     int** altitude_handle;  // a pointer to a pointer to an int
    //
    //     (void)altitude_ptr;     // silence -Wunused-variable for this demo
    //     (void)voltage_ptr;
    //     (void)altitude_handle;
    // }

    // --- [Slide 15] The trap: one declaration per line -------------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr, target_m;  // only altitude_ptr is a pointer!
    //     int *left_ptr, *right_ptr;    // both of these ARE pointers
    //
    //     altitude_ptr = &altitude_m;
    //     target_m = 7;                 // target_m is a plain int
    //     left_ptr = right_ptr = nullptr;
    //
    //     std::cout << sizeof(altitude_ptr) << ' ' << sizeof(target_m) << '\n';
    // }
    //
    // Prints 8 4. The * bound to altitude_ptr only. The fix is one declaration
    // per line (Core Guidelines ES.10).

    // --- [Slide 17] Wild pointers --- UNDEFINED BEHAVIOR ----------------------
    // An uninitialized pointer holds whatever was already in those 8 bytes.
    // This may crash, or may quietly corrupt something far away.
    // {
    //     int* imu_ptr;    // no initializer: a WILD pointer
    //     *imu_ptr = 100;  // UB: writing to a random address
    // }
    //
    // The fix is to give every pointer a value on the line that declares it.
    // If you have nothing to point at yet, that value is nullptr.

    // --- [Slide 18] The address-of and dereference operators -------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};  // & is the address-of operator
    //
    //     std::cout << altitude_ptr << '\n';   // the address of altitude_m
    //     std::cout << *altitude_ptr << '\n';  // * is the dereference operator
    // }

    // --- [Slide 19] Both directions in one program ----------------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //
    //     std::cout << &altitude_m << '\n';     // 0x...a04, its address
    //     std::cout << altitude_ptr << '\n';    // 0x...a04, the same address
    //     std::cout << *altitude_ptr << '\n';   // 120, the object there
    //     std::cout << *(&altitude_m) << '\n';  // 120, the same object
    // }
    //
    // & and * undo each other: *(&altitude_m) IS altitude_m.

    // --- [Slide 22] Same address, and the same type ---------------------------
    // &altitude_m does not produce "an address" in some generic sense. It
    // produces an int*, which is exactly the type of altitude_ptr.
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //
    //     std::cout << &altitude_m << '\n';                   // an address
    //     std::cout << altitude_ptr << '\n';                  // the same one
    //     std::cout << typeid(&altitude_m).name() << '\n';    // and its type
    //     std::cout << typeid(altitude_ptr).name() << '\n';   // the same type
    // }
    //
    // The last two print Pi, which is GCC's MANGLED spelling of "Pointer to
    // int". Run the program through c++filt to read it:
    //     ./build/project/week3/week3 | c++filt -t

    // --- [Slide 23] Dereferencing is not read-only -----------------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //
    //     *altitude_ptr = 90;                // writes through the pointer
    //     std::cout << altitude_m << '\n';   // 90
    //     *altitude_ptr *= 2;                // works like any other name for it
    //     std::cout << altitude_m << '\n';   // 180
    // }
    //
    // One * decides which variable you write to. altitude_ptr = ... writes to
    // the pointer; *altitude_ptr = ... writes to the object.

    // --- [Slide 25] Exercise 1: trace it on paper ------------------------------
    // Do NOT run this first. Write down altitude_m, climb_m and what
    // target_ptr points to, line by line. Then run it and check.
    // {
    //     int altitude_m{4};
    //     int climb_m{7};
    //     int* target_ptr{&altitude_m};
    //
    //     *target_ptr = *target_ptr + climb_m;  // (1)
    //     std::cout << altitude_m << '\n';
    //     std::cout << climb_m << '\n';
    //
    //     target_ptr = &climb_m;                // (2)
    //     *target_ptr = altitude_m;
    //     std::cout << altitude_m << '\n';
    //     std::cout << climb_m << '\n';
    // }

    // #########################################################################
    // SECTION: NULL POINTERS
    // #########################################################################

    // --- [Slide 26] Four ways to write a null pointer -------------------------
    // {
    //     int* imu_ptr{nullptr};  // C++11 and later: the one to use
    //     int* gps_ptr{NULL};     // a C macro, usually 0; avoid in C++
    //     int* mag_ptr{0};        // legal, but 0 reads like a number
    //     int* baro_ptr{};        // value initialization: also null
    //
    //     std::cout << (imu_ptr == gps_ptr) << (mag_ptr == baro_ptr) << '\n';
    // }

    // --- [Slide 27] Testing a pointer ------------------------------------------
    // A pointer converts to bool, and is false exactly when it is null.
    // {
    //     int* sensor_ptr{nullptr};
    //
    //     if (sensor_ptr == nullptr) { /* explicit, and fine */ }
    //     if (!sensor_ptr) { /* shorter, and means the same */ }
    //
    //     if (sensor_ptr) {
    //         std::cout << *sensor_ptr << '\n';  // only reached when not null
    //     } else {
    //         std::cout << "no sensor attached\n";
    //     }
    // }

    // --- [Slide 29] Comparing pointers -----------------------------------------
    // {
    //     int altitude_m{120};
    //     int battery_pct{88};  // an unrelated object
    //
    //     int* altitude_ptr{&altitude_m};
    //     int* battery_ptr{&battery_pct};
    //
    //     // unspecified: it compiles and gives an answer that means nothing
    //     bool guess{altitude_ptr < battery_ptr};
    //
    //     // well defined: a real order, the same one every time you ask
    //     std::less<int*> address_order{};
    //     bool ordered{address_order(altitude_ptr, battery_ptr)};
    //
    //     std::cout << guess << ' ' << ordered << '\n';
    // }
    //
    // std::less compares the two ADDRESSES, never the 120 and the 88.

    // #########################################################################
    // SECTION: SIZE, AND WHAT THE TYPE IS FOR
    // #########################################################################

    // --- [Slide 30] Size of a pointer ------------------------------------------
    // {
    //     int altitude_m{120};
    //     double voltage_v{11.1};
    //     char status{'A'};
    //
    //     int* altitude_ptr{&altitude_m};
    //     double* voltage_ptr{&voltage_v};
    //     char* status_ptr{&status};
    //
    //     std::cout << sizeof(altitude_ptr) << ' ' << sizeof(voltage_ptr) << ' '
    //               << sizeof(status_ptr) << '\n';   // 8 8 8
    //     std::cout << sizeof(altitude_m) << ' ' << sizeof(voltage_v) << ' '
    //               << sizeof(status) << '\n';       // 4 8 1
    // }
    //
    // The suffixes are UNITS: _m is metres, _v volts, _pct percent.
    // sizeof(p) is the pointer; sizeof(*p) is the object it points at.

    // --- [Slide 35] The type is checked --- DOES NOT COMPILE ------------------
    // {
    //     int altitude_m{120};
    //     double voltage_v{11.1};
    //
    //     int* altitude_ptr{nullptr};
    //     altitude_ptr = &altitude_m;  // OK
    //     altitude_ptr = &voltage_v;   // error: cannot convert double* to int*
    // }

    // --- [Slide 35] auto hides the pointer from the reader ---------------------
    // {
    //     auto altitude_m{120};
    //     auto ptr{&altitude_m};  // ptr is int*, but the line does not say so
    //
    //     std::cout << typeid(ptr).name() << '\n';
    // }
    //
    // Write the type out for pointers: int* altitude_ptr{&altitude_m};

    // #########################################################################
    // SECTION: POINTER ARITHMETIC
    // #########################################################################

    // [Slide 36] The operations, and the one rule behind all of them:
    //
    //     ptr + n     ptr - n     ++ptr     --ptr     ptr2 - ptr1
    //
    // Every one counts in OBJECTS, never in bytes.

    // --- [Slide 37] p + 1 moves by one object, not one byte -------------------
    // {
    //     int altitude_m{120};
    //     double voltage_v{11.1};
    //
    //     int* altitude_ptr{&altitude_m};
    //     double* voltage_ptr{&voltage_v};
    //
    //     std::cout << altitude_ptr << '\n';      // 0x7ffd...a10
    //     std::cout << altitude_ptr + 1 << '\n';  // four bytes on
    //     std::cout << voltage_ptr << '\n';       // 0x7ffd...a20
    //     std::cout << voltage_ptr + 1 << '\n';   // eight bytes on
    // }

    // --- [Slide 38] Subtracting two pointers counts objects -------------------
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //
    //     std::cout << (altitude_ptr + 1) - altitude_ptr << '\n';  // 1, not 4
    // }
    //
    // + scales up by sizeof(*p); - scales back down by it. The 4 cancels out.
    // The result type is std::ptrdiff_t, which is SIGNED.

    // --- [Slide 39] What is legal --- UNDEFINED BEHAVIOR ----------------------
    // A lone variable counts as an array of one. You may name the object and
    // the one-past-the-end position, and nothing else.
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //
    //     std::cout << *(altitude_ptr + 1) << '\n';  // UB: reading past it
    //     int* far{altitude_ptr + 2};                // UB: even FORMING this
    //     (void)far;
    // }
    //
    // *(altitude_ptr + 1) is the dangerous one, because it WORKS. It prints
    // whatever sits next on the stack. Build with -fsanitize=address and the
    // same line reports a stack-buffer-overflow.

    // #########################################################################
    // SECTION: CONST-CORRECTNESS
    // #########################################################################

    // --- [Slide 42] The three combinations ------------------------------------
    // Read right to left from the identifier, and read * as "pointer to".
    // {
    //     int altitude_m{120};
    //     int target_alt_m{80};
    //
    //     // 1. pointer to const: read-only THROUGH THIS POINTER
    //     const int* altitude_ptr1{&altitude_m};
    //     // *altitude_ptr1 = 90;            // error
    //     altitude_ptr1 = &target_alt_m;     // OK
    //
    //     // 2. const pointer: the arrow is frozen, the object is not
    //     int* const altitude_ptr2{&altitude_m};  // must be initialized here
    //     *altitude_ptr2 = 90;                    // OK
    //     // altitude_ptr2 = &target_alt_m;       // error
    //
    //     // 3. const pointer to const: both frozen
    //     const int* const altitude_ptr3{&altitude_m};
    //     // *altitude_ptr3 = 90;                 // error
    //     // altitude_ptr3 = &target_alt_m;       // error
    //
    //     std::cout << *altitude_ptr1 << ' ' << *altitude_ptr3 << '\n';
    // }

    // --- [Slide 43] Permission, not immutability -------------------------------
    // {
    //     int altitude_m{120};
    //     const int* altitude_view{&altitude_m};
    //
    //     altitude_m = 118;                     // fine: altitude_m is not const
    //     std::cout << *altitude_view << '\n';  // 118
    // }
    //
    // const int* restricts THIS ACCESS PATH. It does not promise the object
    // never changes.

    // --- [Slide 44] Discussion 1: who is const for? ----------------------------
    // All four compile. Which can write? Which can be repointed? Which can be
    // given "no altitude at all"? What does sizeof report for each?
    // {
    //     int altitude_m{120};
    //     int target_alt_m{80};
    //
    //     int altitude_copy{altitude_m};           // 1. a copy
    //     int* altitude_ctrl{&altitude_m};         // 2. a pointer
    //     const int* altitude_view{&altitude_m};   // 3. a pointer to const
    //     int* const altitude_fixed{&altitude_m};  // 4. a const pointer
    //
    //     altitude_m = 118;
    //     std::cout << altitude_copy << ' ' << *altitude_ctrl << ' '
    //               << *altitude_view << ' ' << *altitude_fixed << '\n';
    //     (void)target_alt_m;
    // }
    //
    // Only altitude_copy still reports 120. It was a snapshot.

    // #########################################################################
    // SECTION: DYNAMIC MEMORY
    // #########################################################################

    // --- [Slide 47] The new operator -------------------------------------------
    // new does three things: asks the allocator for the bytes, creates the
    // object there, and yields its ADDRESS.
    // {
    //     int* battery_pct{new int{88}};
    //     std::cout << *battery_pct << '\n';
    //     delete battery_pct;
    //     battery_pct = nullptr;
    // }

    // --- [Slide 48] The object has no name --- LEAKS ON PURPOSE ---------------
    // {
    //     int* battery_pct{new int{88}};  // the only address of that object
    //     (void)battery_pct;
    // }  // battery_pct dies here. The int does not: it is still allocated,
    //    // and nothing knows where it is any more.
    //
    // Run this under Valgrind: 4 bytes "definitely lost".

    // --- [Slide 49] The delete operator ----------------------------------------
    // Write the two lines as a pair, and in this order.
    // {
    //     int* battery_pct{new int{88}};
    //     std::cout << *battery_pct << '\n';  // ... use *battery_pct ...
    //     delete battery_pct;                 // the storage goes back
    //     battery_pct = nullptr;              // and now the pointer says so
    // }
    //
    // delete acts on the STORAGE, not on the pointer.

    // --- [Slide 52] Rule 1: only delete what you new --- UNDEFINED BEHAVIOR ---
    // {
    //     int altitude_m{120};
    //     int* altitude_ptr{&altitude_m};
    //     delete altitude_ptr;  // UB: the allocator never handed this out
    // }

    // --- [Slide 52] Rule 2: delete nullptr is safe ----------------------------
    // {
    //     int* battery_pct{nullptr};
    //     delete battery_pct;  // does nothing, guaranteed by the standard
    // }
    //
    // So "if (ptr) { delete ptr; }" is redundant. Just write "delete ptr;".

    // --- [Slide 52] Rule 3: delete exactly once --- UNDEFINED BEHAVIOR --------
    // {
    //     int* battery_pct{new int{88}};
    //     delete battery_pct;
    //     delete battery_pct;  // UB
    // }
    //
    // Setting battery_pct to nullptr after the first delete turns the second
    // into a harmless no-op (rule 2).

    // --- [Slide 53] Exercise 2: where does it go wrong? -----------------------
    // Compiles cleanly with -Wall -Wextra -Wpedantic. It has THREE distinct
    // memory bugs. Find them, name them, and give the minimal fix for each.
    // {
    //     int* imu_reading{new int{118}};
    //     int* gps_reading{new int{121}};

    //     imu_reading = gps_reading;  // (a)

    //     std::cout << *imu_reading << '\n';
    //     delete imu_reading;  // (b)
    //     delete gps_reading;  // (c)

    //     std::cout << *gps_reading << '\n';  // (d)
    // }

    // #########################################################################
    // SECTION: WHAT GOES WRONG
    // #########################################################################

    // --- [Slide 55] Dangling pointers --- UNDEFINED BEHAVIOR ------------------
    // Two ways to make one, and only the first involves the heap.
    // {
    //     // 1. deleting, and then forgetting
    //     int* battery_pct{new int{88}};
    //     delete battery_pct;
    //     // battery_pct dangles from here until reassigned or nulled
    //
    //     // 2. keeping a pointer to something whose scope ended
    //     int* reading_ptr{nullptr};
    //     {
    //         int reading{5};
    //         reading_ptr = &reading;
    //         std::cout << *reading_ptr << '\n';  // fine: reading is alive
    //     }                                       // reading dies here
    //     std::cout << *reading_ptr << '\n';      // UB: reading_ptr dangles
    // }
    //
    // The habit that costs nothing: delete p; then p = nullptr; always, in
    // that order, on the same pair of lines.

    // --- [Slide 56] Memory leaks --- LEAKS ON PURPOSE -------------------------
    // {
    //     for (int i{0}; i < 100000; ++i) {
    //         int* reading{new int{i}};  // allocated on every iteration
    //         (void)reading;
    //     }  // the pointer dies each iteration; the int it pointed at does not
    // }
    //
    // Valgrind will report 400,000 bytes lost in 100,000 blocks. A robot's
    // control node is not a short-lived program.

    // --- [Slide 57] Double delete --- UNDEFINED BEHAVIOR ----------------------
    // {
    //     int* primary{new int{88}};
    //     int* backup{primary};  // owns nothing, but it looks like it does
    //
    //     delete primary;
    //     primary = nullptr;
    //     delete backup;  // UB: this block was freed a moment ago
    // }
    //
    // Copying a raw pointer copies the ADDRESS, not the OWNERSHIP. Nulling
    // primary does nothing to backup.

    // --- [Slide 58] Null dereference --- UNDEFINED BEHAVIOR -------------------
    // {
    //     int* sensor{nullptr};
    //     std::cout << *sensor << '\n';  // UB: immediate segmentation fault
    // }
    //
    // Of the four failures this is the friendly one: it stops the program at
    // the line that is wrong.

    // #########################################################################
    // SECTION: FINDING THESE BUGS WITH VALGRIND
    // #########################################################################

    // --- [Slide 61] A leak, as Valgrind reports it ----------------------------
    // Build Debug, then:
    //     valgrind --leak-check=full ./build/project/week3/week3
    // {
    //     int* battery_pct{new int{88}};
    //     std::cout << *battery_pct << '\n';
    //     // forgot: delete battery_pct;
    // }
    //
    // Read the report from the bottom up: 4 bytes definitely lost, allocated
    // by operator new at THIS line. That line number is what -g bought you.

    // --- [Slide 61] A use-after-free, as Valgrind reports it -----------------
    // {
    //     int* battery_pct{new int{88}};
    //     delete battery_pct;
    //     std::cout << *battery_pct << '\n';  // UB
    // }
    //
    // Valgrind gives you the three lines that matter: where the bad read
    // happened, where the block was freed, and where it was allocated. Note
    // that the program still PRINTS 88. Without the tool the bug is invisible.

    // #########################################################################
    // SECTION: REFERENCES
    // #########################################################################

    // --- [Slide 67] A reference is a second name ------------------------------
    // {
    //     int altitude_m{120};
    //     int& alt{altitude_m};  // alt is another name for altitude_m
    //
    //     alt = 90;
    //     std::cout << altitude_m << '\n';  // 90, there is one object here
    //     altitude_m = 118;
    //     std::cout << alt << '\n';         // 118
    // }

    // --- [Slide 68] Property 1: must be initialized --- DOES NOT COMPILE ------
    // {
    //     int& alt1;    // error: a reference must bind to something
    //     int& alt2{};  // error: nothing to bind to
    // }

    // --- [Slide 68] Property 3: no identity of its own ------------------------
    // {
    //     int altitude_m{120};
    //     int& alt{altitude_m};
    //
    //     std::cout << &altitude_m << '\n';  // 0x7ffd...a04
    //     std::cout << &alt << '\n';         // the SAME address
    //     std::cout << sizeof(alt) << '\n';  // 4: the size of an int
    // }
    //
    // &alt gives the address of altitude_m, because alt IS altitude_m. There
    // is no way to ask for "the address of the reference".

    // --- [Slide 69] Property 4: cannot be reseated ----------------------------
    // This is the one that surprises people.
    // {
    //     int altitude_m{120};
    //     int target_alt_m{80};
    //     int& alt{altitude_m};
    //
    //     alt = target_alt_m;                   // assigns the VALUE
    //     std::cout << altitude_m << '\n';      // 80
    //     std::cout << target_alt_m << '\n';    // 80, unchanged
    // }
    //
    // r = b looks like rebinding and is not. A reference is bound once, at
    // initialization, for life.

    // --- [Slide 71] Where references are actually used ------------------------
    // Uncomment the Battery struct and the two declarations at the top of this
    // file as well. Parameter passing is Lecture 5; containers are Lecture 4.
    // {
    //     std::vector<int> altitudes{120, 118, 121};
    //
    //     for (const auto& reading : altitudes) {  // no copy per element
    //         std::cout << reading << ' ';
    //     }
    //     std::cout << '\n';
    //
    //     for (auto& reading : altitudes) {  // modifies in place
    //         reading += 1;
    //     }
    //     std::cout << altitudes[0] << '\n';  // 121
    // }
    //
    // const Battery& passes an ADDRESS, the same 8 bytes any pointer is,
    // instead of copying the whole object.

    // #########################################################################
    // SECTION: EXERCISE 3
    // #########################################################################

    // --- [Slide 75] Exercise 3: audit this code -------------------------------
    // Twelve lines, several bugs. For each line, say whether it is fine or
    // broken, and if broken, name the bug using the vocabulary of the lecture.
    // Then run it under Valgrind and compare with your answer.
    // {
    //     int* imu_reading{new int{10}};
    //     int* gps_reading{new int{20}};
    //     int& fused{*imu_reading};
    //
    //     fused = *gps_reading;
    //     *gps_reading = 30;
    //     imu_reading = new int{40};
    //     delete gps_reading;
    //     *imu_reading = *gps_reading;
    //     int* spare{gps_reading};
    //     fused = 50;
    //     delete spare;
    // }

    return 0;
}
