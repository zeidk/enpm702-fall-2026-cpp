// =============================================================================
// ENPM702 - Introductory Robot Programming
// Week 2 playground - L2: Variables, Types and Memory
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
// TWO THINGS TO EXPECT
//   1. Blocks marked "DOES NOT COMPILE" are on the slides to show you an error.
//      Uncomment them on purpose, read the message, then comment them back.
//   2. This target builds with -Wall -Wextra. Exactly three snippets warn, and
//      each warning IS the lesson: -Wsign-compare on [Slide 37] and on the
//      [Slide 55] exercise, and -Wsequence-point on the macro in [Slide 60].
//      Anything else you see is worth a second look.
// =============================================================================

#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

// =============================================================================
// NAMESPACE SCOPE
//
// A few snippets cannot live inside main(): a global variable, a function, a
// namespace and a macro all have to be declared outside it. Uncomment the
// piece you need here, then uncomment its matching block inside main().
// =============================================================================

// --- [Slide 69] Global Scope -------------------------------------------------
// int global_var{1};

// --- [Slide 70] Where globals live -------------------------------------------
// int global_x;      // uninitialized -> BSS
// int global_y{1};   // initialized   -> data

// --- [Slide 60] Symbolic Constants (Macros) ----------------------------------
// Avoid these in your own code. They are here so you can see why.
// #define PI 3.14159         // not a variable: no type, no scope
// #define SQUARE(x) ((x) * (x))

// --- [Slide 73] Namespaces ---------------------------------------------------
// namespace MyNamespace {
// int x{3};
// int y{4};
// }  // namespace MyNamespace

// --- [Slide 74] Why to avoid "using namespace" -------------------------------
// Uncomment ALL FOUR lines together, plus the [Slide 74] block in main().
// namespace Collide {
// int cout{1};
// }
// using namespace std;
// using namespace Collide;

// --- [Slide 75] Aliases ------------------------------------------------------
// using uint = unsigned int;
// using JointAngles = std::vector<std::array<double, 6>>;

// --- [Slide 76] Exercise 2 ---------------------------------------------------
// int counter{1};                       // global

int main() {
    std::cout << "Week 2 - uncomment a snippet in src/main.cpp and rebuild.\n";

    // #########################################################################
    // SECTION: TERMINAL INPUT AND OUTPUT
    // #########################################################################

    // === Subsection: Basic Input and Output ==================================

    // --- [Slide 7] Output ----------------------------------------------------
    // {
    //     std::cout << "Hello, World!";           // text, no newline
    //     std::cout << '\n';                      // newline
    //     std::cout << "Number: " << 42 << '\n';  // chained insertions
    //     std::cout << "Pi: " << 3.14159 << '\n';
    // }

    // --- [Slide 8] Input -----------------------------------------------------
    // {
    //     int age{};
    //     double height{};
    //
    //     std::cout << "Enter your age: ";
    //     std::cin >> age;                        // reads an int
    //
    //     std::cout << "Enter your height: ";
    //     std::cin >> height;                     // reads a double
    //
    //     std::cout << "Age: " << age << ", Height: " << height << '\n';
    // }
    //
    // Try it: type 3.7 as the age. What is printed, and what did std::cin do
    // with the .7?

    // --- [Slide 9] Stream Manipulators ---------------------------------------
    // A manipulator is a value you insert with << that changes HOW the stream
    // formats, instead of printing anything itself.
    // {
    //     std::cout << true << '\n';                    // 1
    //     std::cout << std::boolalpha << true << '\n';  // true
    //     std::cout << false << '\n';                   // false  <- STILL in effect
    //     std::cout << std::noboolalpha;                // switch it back
    //
    //     std::cout << 3.14159265 << '\n';              // 3.14159
    //     std::cout << std::setprecision(3) << 3.14159265 << '\n';   // 3.14
    //     std::cout << std::setprecision(6);            // put it back for later
    // }
    //
    // They are STICKY: a manipulator changes the stream until something changes
    // it back, so a choice made here silently affects output written elsewhere.
    // Manipulators taking NO argument (boolalpha, fixed, hex) come with
    // <iostream>; those taking an ARGUMENT (setprecision, setw) need <iomanip>.
    // There is no std::precision -- the manipulator is std::setprecision(n),
    // while precision is a member function: std::cout.precision(9).

    // #########################################################################
    // SECTION: MEMORY AND VARIABLES
    // #########################################################################

    // === Subsection: Variables ===============================================

    // --- [Slide 18] Characteristics: all five, on one line -------------------
    // {
    //     int number{2500};
    //     // Type: int   Name: number   Value: 2500
    //     // Scope: this block          Lifetime: until the closing brace
    //     std::cout << number << '\n';
    // }

    // --- [Slide 19] Naming (Identifiers) -------------------------------------
    // These four are fine.
    // {
    //     int break1{};     // OK
    //     int break_1{};    // OK
    //     int Break1{};     // OK
    //     int BREAK{};      // OK
    //     std::cout << break1 + break_1 + Break1 + BREAK << '\n';
    // }
    //
    // These three COMPILE but are reserved for the implementation. Do not use
    // them. They produce no warning at all, even with -Wall -Wextra.
    // {
    //     int _break1{};    // legal, but reserved in the global namespace
    //     int _Break{};     // legal, but reserved everywhere: _ then a capital
    //     int my__name{};   // legal, but reserved everywhere: contains __
    //     std::cout << _break1 + _Break + my__name << '\n';
    // }
    //
    // DOES NOT COMPILE - uncomment one line at a time and read the error.
    // {
    //     int 1Break;     // an identifier cannot start with a digit
    //     int my-name;    // '-' is not part of an identifier
    //     int class;      // 'class' is a reserved keyword
    // }

    // --- [Slide 20] Course convention: snake_case ----------------------------
    // {
    //     int my_variable{};       // use this style
    //     int student_count{};
    //     int max_temperature{};
    //     bool is_valid{};
    //
    //     int myVariable{};        // avoid: camelCase
    //     int MyVariable{};        // avoid: PascalCase
    //     int MY_VARIABLE{};       // avoid: ALL_CAPS signals a macro
    //
    //     std::cout << my_variable + student_count + max_temperature
    //               << is_valid << myVariable << MyVariable << MY_VARIABLE
    //               << '\n';
    // }

    // --- [Slide 22] The sizeof operator --------------------------------------
    // {
    //     int number{2500};
    //     std::cout << sizeof(number) << '\n';  // 4 on a typical x86-64 machine
    //     std::cout << sizeof(int) << '\n';     // 4 on a typical x86-64 machine
    // }
    //
    // Run this on your own machine and compare. Sizes are platform-dependent:
    // the standard fixes minimum widths, not exact sizes.
    // {
    //     std::cout << "char        " << sizeof(char)        << '\n';
    //     std::cout << "short       " << sizeof(short)       << '\n';
    //     std::cout << "int         " << sizeof(int)         << '\n';
    //     std::cout << "long        " << sizeof(long)        << '\n';
    //     std::cout << "long long   " << sizeof(long long)   << '\n';
    //     std::cout << "float       " << sizeof(float)       << '\n';
    //     std::cout << "double      " << sizeof(double)      << '\n';
    //     std::cout << "long double " << sizeof(long double) << '\n';
    //     std::cout << "bool        " << sizeof(bool)        << '\n';
    // }

    // --- [Slide 23] Memory Allocation: the address-of operator ---------------
    // {
    //     int number{2500};
    //     std::cout << &number << '\n';  // e.g. 0x7fff214aba04
    // }
    //
    // Run it several times. The value changes from run to run.

    // --- [Slide 25] Declarations and Assignments -----------------------------
    // {
    //     int number;                     // declaration (and definition)
    //
    //     number = 1;                     // assignment
    //     std::cout << number << '\n';    // 1
    //     number = 2;                     // assignment: overwrites the 1
    //     std::cout << number << '\n';    // 2
    // }
    //
    // Coming from Python? The name is NOT rebound to a new object here: the
    // same 4 bytes are rewritten in place. Print &number to convince yourself.

    // --- [Slide 26] The three forms of initialization ------------------------
    // {
    //     int a = 1;   // copy initialization    - inherited from C
    //     int b(1);    // direct initialization  - little used now
    //     int c{1};    // uniform initialization - THE ONE THIS COURSE USES
    //     std::cout << a << ' ' << b << ' ' << c << '\n';
    // }

    // --- [Slide 27] Zero Initialization --------------------------------------
    // {
    //     int a{};                 // 0
    //     double b{};              // 0.0
    //     bool c{};                // false
    //
    //     std::cout << a << '\n';  // 0
    //     std::cout << b << '\n';  // 0  <- cout drops the trailing .0
    //     std::cout << std::fixed << std::setprecision(1) << b << '\n';  // 0.0
    //     std::cout << std::defaultfloat;  // undo the sticky manipulators
    //     std::cout << std::boolalpha << c << std::noboolalpha << '\n';
    // }

    // --- [Slide 28] Empty braces, or an explicit zero? -----------------------
    // {
    //     int a{};    // placeholder: a is assigned before it is read
    //     int b{0};   // the value 0 is meaningful here
    //     a = b + 3;  // value of b is used, a is assigned a new value
    //     std::cout << a << '\n';
    // }

    // === Subsection: Undefined Behavior ======================================

    // --- [Slide 28] Uninitialized Variables ----------------------------------
    // UNDEFINED BEHAVIOR. It may print 0, may print garbage, may differ between
    // -O0 and -O2. A plausible-looking number here proves nothing.
    // {
    //     int number;                   // uninitialized
    //     std::cout << number << '\n';  // undefined behavior
    // }

    // #########################################################################
    // SECTION: TYPES AND CONVERSIONS
    // #########################################################################

    // === Subsection: Integral Types ==========================================

    // --- [Slide 36] Characters -----------------------------------------------
    // A char holds ONE character, in single quotes. Double quotes make a
    // string literal, which is a different type.
    // {
    //     char letter{'a'};  // single quotes: one character, one byte
    //     std::cout << letter << '\n';                         // a
    //     std::cout << static_cast<int>(letter) << '\n';       // 97
    //     std::cout << letter + 1 << '\n';                     // 98, an int
    //     std::cout << static_cast<char>(letter + 1) << '\n';  // b
    //
    //     std::cout << sizeof('a') << '\n';   // 1: 'a' is a char
    //     std::cout << sizeof("a") << '\n';   // 2: "a" is 'a' plus a '\0'
    // }
    //
    // Why char is an INTEGRAL type: it stores the numeric character code, so
    // 'a' is just another way to write 97. It promotes to int in arithmetic,
    // which is why letter + 1 gives 98 and not b. Only std::cout treats it
    // specially, printing the glyph instead of the number.
    //
    // Escape sequences: a backslash and the next character are ONE character.
    // {
    //     std::cout << "newline, tab[\t], backslash[\\], quote[\"]\n";
    //     char quote{'\''};
    //     char backslash{'\\'};
    //     char nul{'\0'};
    //     std::cout << quote << backslash << static_cast<int>(nul) << '\n';
    // }

    // --- [Slide 37] Signed vs Unsigned Pitfalls ------------------------------
    // Comparisons can invert: -1 is converted to unsigned and becomes
    // 4,294,967,295. Build with -Wall and read the -Wsign-compare warning.
    // {
    //     std::cout << std::boolalpha << (-1 < 1u) << '\n';  // false
    //     std::cout << std::noboolalpha;
    // }
    //
    // C++20 fixes it without changing the types:
    // {
    //     std::cout << std::boolalpha << std::cmp_less(-1, 1u) << '\n';  // true
    //     std::cout << std::noboolalpha;
    // }

    // === Subsection: Floating-point Number Types =============================

    // --- [Slide 39] Float suffix ---------------------------------------------
    // {
    //     std::cout << 1.05 << '\n';   // double literal
    //     std::cout << 1.05f << '\n';  // float literal
    // }
    //
    // DOES NOT COMPILE - 1 is an integer literal and f is not a valid integer
    // suffix. Write 1.0f.
    // {
    //     std::cout << 1f << '\n';
    // }

    // --- [Slide 40] Controlling Output ---------------------------------------
    // {
    //     std::cout << std::setprecision(9);
    //     std::cout << 0.33333333333f << '\n';           // 0.333333343
    //
    //     std::cout << std::setprecision(15);
    //     std::cout << 8.3642343534322323232322 << '\n'; // 8.36423435343223
    //
    //     std::cout << std::setprecision(6);             // back to the default
    // }
    //
    // 0.333333343 is not a display bug: a float carries about 7 significant
    // digits, so asking for 9 exposes the representation error.

    // === Subsection: Boolean Type ============================================

    // --- [Slide 42] Printing Booleans ----------------------------------------
    // {
    //     bool is_today_sunny{true};
    //     bool is_today_cloudy{false};
    //
    //     std::cout << is_today_sunny << '\n';   // 1
    //     std::cout << is_today_cloudy << '\n';  // 0
    //
    //     // std::boolalpha prints the words instead of the digits
    //     std::cout << std::boolalpha;
    //     std::cout << is_today_sunny << '\n';   // true
    //     std::cout << is_today_cloudy << '\n';  // false
    //
    //     // std::noboolalpha switches back
    //     std::cout << std::noboolalpha;
    //     std::cout << is_today_sunny << '\n';   // 1
    // }

    // === Subsection: Type Conversion =========================================

    // --- [Slide 45] Inspecting Types with typeid -----------------------------
    // {
    //     double num1{1.5};
    //     int num2 = num1;  // 1.5 converted to 1
    //
    //     std::cout << "Value of num1: " << num1 << '\n';                 // 1.5
    //     std::cout << "Type of num1:  " << typeid(num1).name() << '\n';  // d
    //     std::cout << "Value of num2: " << num2 << '\n';                 // 1
    // }
    //
    // .name() returns GCC's MANGLED name, not "double":
    //   b bool, c char, s short, i int, j unsigned int, l long,
    //   m unsigned long, x long long, y unsigned long long, f float, d double

    // --- [Slide 47] Numeric Promotion ----------------------------------------
    // {
    //     double num1{5.0};                // no promotion needed
    //     double num2{4.0f};               // float -> double
    //
    //     short s{1};
    //     int a = s;                       // short -> int
    //     int b = 'a';                     // char  -> int
    //     int c = true;                    // bool  -> int
    //
    //     std::cout << a << '\n';          // 1
    //     std::cout << b << '\n';          // 97
    //     std::cout << c << '\n';          // 1
    //     std::cout << sizeof(s) << '\n';  // 2
    //     std::cout << num1 + num2 << '\n';
    // }

    // --- [Slide 48] Numeric Conversion ---------------------------------------
    // Unlike a promotion, a conversion MAY lose data.
    // {
    //     // Integral conversions
    //     short s = 1;            // int    -> short
    //     long l = 1;             // int    -> long
    //     char c = s;             // short  -> char
    //     bool b = 3;             // int    -> bool
    //
    //     // Floating-point conversions
    //     float f = 3.0;          // double -> float
    //     long double ld = 3.0;   // double -> long double  (not a promotion!)
    //
    //     // Floating-integral conversions
    //     int i = 5.8;            // double -> int, the .8 is discarded
    //     int j = 3.453f;         // float  -> int
    //
    //     // Integral-floating conversions
    //     double d = 5;           // int    -> double       (not a promotion!)
    //
    //     std::cout << s << ' ' << l << ' ' << +c << ' ' << b << ' '
    //               << f << ' ' << ld << ' ' << i << ' ' << j << ' '
    //               << d << '\n';
    // }

    // --- [Slide 50] Narrowing conversions ------------------------------------
    // Lines 2, 3 and 4 all request the SAME conversion, but only the braced
    // form is rejected. DOES NOT COMPILE because of int d{3.5}.
    // {
    //     int a;       // uninitialized: garbage
    //     int b = 3.2; // b == 3, implicit double -> int
    //     int c(1.3);  // c == 1, implicit double -> int
    //     int d{3.5};  // Error: narrowing conversion of '3.5e+0'
    // }
    //
    // If you did mean it, say so. static_cast TRUNCATES, it does not round.
    // {
    //     int b = static_cast<int>(3.2);
    //     int c(static_cast<int>(1.3));
    //     int d{static_cast<int>(3.5)};   // OK: explicit
    //     std::cout << b << ' ' << c << ' ' << d << '\n';   // 3 1 3
    //     std::cout << static_cast<int>(3.7) << '\n';       // 3, not 4
    // }

    // --- [Slide 53] Arithmetic conversions: steps 1 and 3 --------------------
    // {
    //     int i{42};
    //     double d{3.14};
    //     std::cout << typeid(i + d).name() << '\n';   // d  -> double
    //     std::cout << i + d << '\n';                  // 45.14
    //
    //     unsigned int ui{100};
    //     long l{5000};
    //     std::cout << typeid(ui + l).name() << '\n';  // l  -> long
    //     std::cout << ui + l << '\n';                 // 5100
    //
    //     unsigned short us{10};
    //     unsigned long ul{700000};
    //     std::cout << typeid(us + ul).name() << '\n'; // m  -> unsigned long
    //     std::cout << us + ul << '\n';                // 700010
    // }

    // --- [Slide 54] Arithmetic conversions: step 2 ---------------------------
    // Neither short nor char survives step 2: both are promoted to int.
    // {
    //     short s1{100};
    //     char c{50};
    //     std::cout << typeid(s1 + c).name() << '\n';  // i  -> int
    //     std::cout << s1 + c << '\n';                 // 150
    //
    //     unsigned char uc{200};
    //     bool b1{true};
    //     std::cout << typeid(uc + b1).name() << '\n'; // i  -> int
    //     std::cout << uc + b1 << '\n';                // 201
    // }
    //
    // Integer division is the classic trap.
    // {
    //     int a{3};
    //     int b{2};
    //     std::cout << a / b << '\n';                             // 1, not 1.5
    //     std::cout << static_cast<double>(a) / b << '\n';         // 1.5
    // }

    // #########################################################################
    // SECTION: EXERCISE 1
    // #########################################################################

    // --- [Slide 55] Predict first, then compile ------------------------------
    // Write down all five answers BEFORE you run this. Then for (3) and (4),
    // name the conversion that fires and say whether it is a promotion or a
    // conversion. Built with -Wall -Wextra, exactly one line warns. Which?
    // {
    //     int a{7};
    //     double b{2.0};
    //     short c{300};
    //     unsigned d{3};
    //     int e{-4};
    //
    //     std::cout << a / 2   << '\n';   // (1)
    //     std::cout << a / b   << '\n';   // (2)
    //     std::cout << c + c   << '\n';   // (3)
    //     std::cout << (e < d) << '\n';   // (4)
    //     std::cout << char(c) << '\n';   // (5)
    // }

    // #########################################################################
    // SECTION: CONSTANTS
    // #########################################################################

    // === Subsection: Literals, const and constexpr ===========================

    // --- [Slide 58] Literal Constants ----------------------------------------
    // {
    //     // Integral literals
    //     int dec{12};          // decimal
    //     int hex{0xFF};        // hexadecimal
    //     int bin{0b1010};      // binary
    //     long big{12L};        // long suffix
    //
    //     // Floating-point literals
    //     double e{2.71};
    //     float pi{3.14159f};
    //     double exp{1.23e4};   // 12300.0
    //
    //     // Character and string literals
    //     char a{'a'};
    //     char newline{'\n'};
    //     std::string hello{"Hello"};
    //
    //     // Boolean literals
    //     bool yes{true};
    //     bool no{false};
    //
    //     std::cout << dec << ' ' << hex << ' ' << bin << ' ' << big << '\n';
    //     std::cout << e << ' ' << pi << ' ' << exp << '\n';
    //     std::cout << a << hello << newline;
    //     std::cout << yes << no << '\n';
    // }

    // --- [Slide 59] Constant Variables ---------------------------------------
    // {
    //     const double radius{3.5};   // "west const": preferred in this course
    //     double const radius2{3.5};  // "east const": legal, not our style
    //     std::cout << radius << ' ' << radius2 << '\n';
    // }
    //
    // DOES NOT COMPILE - the two rules the compiler enforces.
    // {
    //     const double pi;            // Error: uninitialized const 'pi'
    //
    //     const double e{2.71828};
    //     e = 2.7;             // Error: assignment of read-only variable 'e'
    // }

    // --- [Slide 60] Symbolic Constants (Macros) ------------------------------
    // Uncomment the #define lines at the top of this file first.
    // {
    //     double r{2.0};
    //     double area{PI * r * r};   // the compiler only sees 3.14159 * r * r
    //     std::cout << area << '\n';
    //
    //     int i{5};
    //     int bad{SQUARE(i++)};      // expands to ((i++) * (i++))
    //                                // i is modified twice: undefined behavior
    //     std::cout << bad << '\n';
    // }
    //
    // [Slide 61] SEE THE SUBSTITUTION FOR YOURSELF. Stop after the preprocessor
    // and read what the compiler is actually handed:
    //
    //     g++ -E -P src/main.cpp -o main.i
    //
    // Then open main.i and search for 3.14159 and for ((i++) * (i++)). Both
    // #define lines are gone, and the names PI and SQUARE appear nowhere --
    // which is exactly why a debugger can never show them back to you.
    //
    // A macro has no type, no scope, and does not survive into the debugger.
    // Use const or constexpr instead.

    // --- [Slide 62] Compile-time and Runtime Constants -----------------------
    // {
    //     const int a{10};         // compile-time: the initializer is a literal
    //
    //     int input{};
    //     std::cin >> input;
    //     const int b{input};      // runtime: value known only at run time
    //
    //     constexpr int c{10};     // compile-time, guaranteed by the compiler
    //     // constexpr int d{input};  // Error: 'input' is not a constant
    //                                 //        expression
    //
    //     std::cout << a << ' ' << b << ' ' << c << '\n';
    // }

    // --- [Slide 63] Why constexpr --------------------------------------------
    // 1. A const of NON-INTEGRAL type is not usable in a constant expression.
    //    The area1 line DOES NOT COMPILE; the area2 line does.
    // {
    //     const     double r1{3.5};
    //     // constexpr double area1{r1 * r1};  // Error: 'r1' not usable here
    //     constexpr double r2{3.5};
    //     constexpr double area2{r2 * r2};     // OK: constexpr works for every type
    //     std::cout << r1 << ' ' << area2 << '\n';
    // }
    //
    // 2. Sizes and compile-time checks require a constant expression.
    // {
    //     constexpr std::size_t kJoints{6};
    //     std::array<double, kJoints> angles{};  // OK
    //     static_assert(kJoints == 6);           // checked while compiling
    //     std::cout << angles.size() << '\n';
    // }
    //
    // C++20 ships the mathematical constants, so you never type the digits:
    // {
    //     constexpr double pi{std::numbers::pi};
    //     std::cout << std::setprecision(17) << pi << '\n';
    //     std::cout << std::setprecision(6);
    // }

    // #########################################################################
    // SECTION: SCOPE AND NAMES
    // #########################################################################

    // === Subsection: Compound Statements =====================================

    // --- [Slide 65] Blocks ---------------------------------------------------
    // A block can appear anywhere a single statement can, and there is NO
    // semicolon after its closing brace.
    // {
    //     int a{};
    //     {               // start nested block 1
    //         int b{};
    //         {           // start nested block 2
    //             int c{};
    //             std::cout << a + b + c << '\n';
    //         }           // end nested block 2
    //     }               // end nested block 1
    // }
    //
    // Keep nesting to three levels or fewer.

    // === Subsection: Scopes ==================================================

    // --- [Slide 67] Local Scope ----------------------------------------------
    // Uncomment the whole block: it DOES NOT COMPILE, because b is used after
    // its block ended. Delete that one line and it builds.
    // {
    //     int a{1};
    //     {
    //         int b{2};
    //         std::cout << a << '\n';  // 1: a is still in scope
    //         std::cout << b << '\n';  // 2
    //     }  // b goes out of scope here
    //
    //     std::cout << b << '\n';      // Error: 'b' was not declared in this scope
    //     int c{3};
    // }  // a and c go out of scope here

    // --- [Slide 69] Global Scope ---------------------------------------------
    // Uncomment global_var at the top of this file first.
    // ::name, with nothing on its left, names the GLOBAL namespace -- exactly
    // as std::cout names std. It reaches a global that a local is hiding.
    // {
    //     std::cout << global_var << '\n';    // 1
    //     global_var++;                       // any code here can change it: 2
    //
    //     {
    //         int global_var{100};            // a LOCAL that shadows the global
    //         std::cout << global_var   << '\n';  // 100  <- the local
    //         std::cout << ::global_var << '\n';  // 2    <- :: reaches the global
    //     }                                   // the local is gone here
    //
    //     std::cout << global_var << '\n';    // 2    <- the global again
    // }
    //
    // -Wall -Wextra says NOTHING about that shadowing. -Wshadow is the flag.

    // --- [Slide 70] Where globals live ---------------------------------------
    // Uncomment global_x and global_y at the top of this file first.
    // global_x is NOT garbage: globals in BSS are zeroed by the loader. That
    // is the one exception; the rule still holds for locals.
    // {
    //     std::cout << global_x << '\n';  // 0, zeroed by the loader
    //     std::cout << global_y << '\n';  // 1
    // }
    //
    // Prefer no globals at all (Core Guidelines R.6). If you truly need one,
    // make it const or constexpr.

    // === Subsection: Naming Collisions and Namespaces ========================

    // --- [Slide 71] A naming collision ---------------------------------------
    // DOES NOT COMPILE - two identifiers with the same name in one scope.
    // {
    //     int x{1};
    //     int x{2};  // Error: redeclaration of 'int x'
    //     std::cout << x << '\n';
    // }

    // --- [Slide 73] Namespaces -----------------------------------------------
    // Uncomment namespace MyNamespace at the top of this file first.
    // {
    //     std::cout << MyNamespace::x << '\n';  // 3
    //     std::cout << MyNamespace::y << '\n';  // 4
    // }
    //
    // The other two forms. A using DECLARATION brings in one name; a using
    // DIRECTIVE brings in all of them.
    // {
    //     using MyNamespace::x;         // declaration: one name only
    //     std::cout << x << '\n';       // 3
    //     // std::cout << y << '\n';    // Error: 'y' was not declared
    // }

    // --- [Slide 74] Why to avoid "using namespace" ---------------------------
    // Uncomment the Collide namespace AND both using-directives at the top.
    // DOES NOT COMPILE: two directives pulled in the same name.
    // {
    //     cout << cout << '\n';  // Error: reference to 'cout' is ambiguous
    // }
    //
    // Never put a using-directive in a header. Write std:: in full: it is five
    // characters and it tells the reader exactly where the name comes from.

    // #########################################################################
    // SECTION: ALIASES
    // #########################################################################

    // --- [Slide 75] Type aliases ---------------------------------------------
    // Uncomment the two using-declarations at the top of this file first.
    // An alias creates NO new type: uint and unsigned int are interchangeable,
    // and the compiler will not stop you from mixing them.
    // {
    //     uint age{30};
    //     unsigned int same_thing{age};   // no conversion: identical types
    //
    //     JointAngles home{};             // std::vector<std::array<double,6>>
    //     std::cout << age << ' ' << same_thing << ' ' << home.size() << '\n';
    // }

    // #########################################################################
    // SECTION: EXERCISE 2
    // #########################################################################

    // --- [Slide 76] Predict first, then compile ------------------------------
    // Uncomment counter at the top of this file first.
    //
    // Predict (1)-(4). Which SEGMENT holds each of the three counter variables,
    // and which has STATIC storage duration? Why did counter += 5 reach one and
    // ::counter += 3 another? The global earns nothing here -- rewrite without
    // it (Core Guidelines R.6).
    // {
    //     int counter{100};                 // shadows the global from here on
    //
    //     counter   += 5;                   // which counter?
    //     ::counter += 3;                   // and which one?
    //
    //     std::cout << counter   << '\n';   // (1)
    //     std::cout << ::counter << '\n';   // (2)
    //
    //     {
    //         int counter{7};               // shadows again, inside this block
    //         std::cout << counter << '\n'; // (3)
    //     }
    //
    //     std::cout << counter << '\n';     // (4)
    // }

    return 0;
}
