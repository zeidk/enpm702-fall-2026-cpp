// =============================================================================
// ENPM702 - Introductory Robot Programming
// Reading-material playground
//
// Snippets from the self-study reading modules on ReadTheDocs:
//   * Flow Control and Operations
//   * Exception Handling
//   * Compiler Warning Flags
//
// HOW TO USE THIS FILE
//   Every snippet below is copied from a reading module and is COMMENTED OUT.
//   Uncomment one block, build, run, then comment it back and move on.
//   In VS Code: select the block and press Ctrl+/ (Cmd+/ on macOS).
//
//   Blocks are labelled [Module > Section] to match the headings on the
//   ReadTheDocs page, so you can read and run side by side. Section names are
//   used rather than page numbers because the headings are stable.
//
//   Each snippet lives in its own { } block, so the same variable name can be
//   reused from one snippet to the next without a redeclaration error.
//
// THREE THINGS TO EXPECT
//   1. Blocks marked "DOES NOT COMPILE" are in the reading to show you an
//      error. Uncomment them on purpose, read the message, comment them back.
//   2. Blocks marked "WARNS ON PURPOSE" are meant to make the compiler
//      complain. The warning IS the point; do not try to silence it. Five
//      blocks are marked: three in the Compiler Warning Flags module, plus
//      the "if Statements" and "Dangling else" traps, which -Wall catches.
//   3. Blocks marked "READS STDIN" wait for you to type something.
//
// These exercises are not graded and are not submitted on Canvas.
// =============================================================================

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// =============================================================================
// NAMESPACE SCOPE
//
// A few snippets need a function or a class declared outside main().
// Uncomment the piece you need here, then uncomment its block inside main().
// =============================================================================

// --- [Exception Handling > Throwing Exceptions] ------------------------------
// double divide(double a, double b) {
//     if (b == 0.0) {
//         throw std::invalid_argument("division by zero");
//     }
//     return a / b;
// }

// --- [Exception Handling > Custom Exception Classes] -------------------------
// class SensorError : public std::runtime_error {
// public:
//     SensorError(const std::string& sensor_name, const std::string& message)
//         : std::runtime_error("Sensor '" + sensor_name + "': " + message),
//           sensor_name_{sensor_name} {}
//
//     const std::string& sensor_name() const { return sensor_name_; }
//
// private:
//     std::string sensor_name_;
// };
//
// void read_sensor(const std::string& name, double value) {
//     if (value < 0.0) {
//         throw SensorError(name, "negative reading");
//     }
//     std::cout << name << ": " << value << '\n';
// }

// --- [Exception Handling > RAII and Exception Safety] ------------------------
// void process() {
//     auto ptr{std::make_unique<int>(42)};
//     // ... code that might throw ...
//     throw std::runtime_error("something failed");
//     // ptr is automatically deleted when the stack unwinds
// }

int main() {
    std::cout << "Reading material - uncomment a snippet in src/main.cpp.\n";

    // #########################################################################
    // MODULE: FLOW CONTROL AND OPERATIONS
    // #########################################################################

    // === Statements ==========================================================

    // --- [Flow Control > Statements] -----------------------------------------
    // A compound statement is a group of statements inside braces. Blocks
    // nest, and each one opens a new scope.
    // {
    //     {  // start of compound statement 1
    //         int a{10};          // simple statement
    //         if (a <= 100) {     // start of compound statement 2
    //             a = 100;        // simple statement
    //             std::cout << "a is: " << a << '\n'; // simple statement
    //         }  // end of compound statement 2
    //     }      // end of compound statement 1
    // }

    // === Selection Statements ================================================

    // --- [Flow Control > if Statements] --------------------------------------
    // WARNS ON PURPOSE. Without braces, an if controls exactly ONE statement.
    // The indentation below is a lie: "a = -a;" runs whatever the condition
    // was. -Wall catches this one for you: -Wmisleading-indentation.
    // {
    //     int a{10};
    //
    //     if (a > 0)
    //         std::cout << "a is positive\n";
    //         a = -a;   // NOT part of the if: this always executes
    //
    //     std::cout << "a is now " << a << '\n';
    // }

    // --- [Flow Control > Conditional (Ternary) Operator] ---------------------
    // Note x % 2 is NON-ZERO when x is odd.
    // {
    //     int x{1};
    //
    //     if (x % 2)
    //         std::cout << x << " is odd\n";
    //     else
    //         std::cout << x << " is even\n";
    //
    //     // the same thing with the conditional operator
    //     std::cout << ((x % 2) ? "x is odd\n" : "x is even\n");
    // }
    //
    // The operator is an EXPRESSION, so it can initialize a constant:
    // {
    //     constexpr int x{3};
    //     constexpr int y{2};
    //
    //     constexpr int larger_value{x > y ? x : y};
    //     std::cout << "The larger value is: " << larger_value << '\n';  // 3
    // }
    //
    // DOES NOT COMPILE - the if/else version cannot do this. Each
    // larger_value is local to its own branch and is out of scope by the
    // time you try to print it.
    // {
    //     constexpr int x{3};
    //     constexpr int y{2};
    //
    //     if (x > y)
    //         constexpr int larger_value{x};
    //     else
    //         constexpr int larger_value{y};
    //
    //     std::cout << "The larger value is: " << larger_value << '\n';
    // }

    // --- [Flow Control > Dangling else] --------------------------------------
    // WARNS ON PURPOSE. An else binds to the NEAREST unmatched if, not to the
    // one the indentation suggests. Predict the output before running --
    // -Wall catches this trap too, as -Wdangling-else.
    // {
    //     int x{5};
    //     int y{10};
    //
    //     if (x > 0)
    //         if (y > 0)
    //             std::cout << "x and y are positive\n";
    //     else
    //         std::cout << "x is non-positive\n";
    // }
    //
    // Braces remove the ambiguity, and show what the code above really means:
    // {
    //     int x{5};
    //     int y{10};
    //
    //     if (x > 0){
    //         if (y > 0){
    //             std::cout << "x and y are positive\n";
    //         }
    //         else{
    //             std::cout << "x is non-positive\n";
    //         }
    //     }
    // }

    // --- [Flow Control > switch Statements] ----------------------------------
    // {
    //     int a{1};
    //
    //     switch (a){
    //     case 1:
    //         std::cout << "one\n";
    //         break;
    //     case 2:
    //         std::cout << "two\n";
    //         break;
    //     case 3:
    //         std::cout << "three\n";
    //         break;
    //     default:
    //         std::cout << "unknown\n";
    //         break;
    //     }
    // }

    // --- [Flow Control > Fallthrough] ----------------------------------------
    // Forgetting break falls through to the next case. Here that is
    // intentional, but -Wextra warns unless you say so with [[fallthrough]].
    // {
    //     int choice{1};
    //
    //     switch (choice) {
    //         case 1: // we have a match here
    //             std::cout << "Choice is 1\n"; // this is executed
    //             [[fallthrough]]; // compiler will not raise a warning
    //         case 2:
    //             std::cout << "Choice is 2\n"; // I want to execute this as well
    //             break;
    //         default:
    //             std::cout << "Unknown choice\n";
    //     }
    // }

    // === Iteration Statements ================================================

    // --- [Flow Control > while Statements] -----------------------------------
    // {
    //     int counter{1};
    //     while (counter <= 10) {
    //         std::cout << counter << ' ';
    //         ++counter;
    //     }
    //     std::cout << '\n';
    // }

    // --- [Flow Control > Nested Loops] ---------------------------------------
    // {
    //     // outer loop loops 5 times
    //     int outer{1};
    //     while (outer <= 5) {
    //         int inner{1};
    //         // inner loop loops 10 times
    //         while (inner <= 10) {
    //             std::cout << outer * inner << ' ';
    //             ++inner;
    //         }
    //         // print a newline at the end of each row
    //         std::cout << '\n';
    //         ++outer;
    //     }
    // }

    // --- [Flow Control > do-while Statements] --------------------------------
    // READS STDIN. A do-while always runs its body at least once, which is
    // what you want when the body is the thing that gets the input.
    // {
    //     // selection must be declared outside of the do/while so we can use it later
    //     int selection{};
    //
    //     do {
    //         std::cout << "Which approach do you want to use (1 or 2)?:\n";
    //         std::cout << "1) Breadth-first search\n";
    //         std::cout << "2) Depth-first search\n";
    //         std::cout << "Please select an approach: ";
    //         std::cin >> selection;
    //     } while (selection != 1 && selection != 2);
    //
    //     switch (selection) {
    //         case 1:
    //           std::cout << "You selected: Breadth-first\n";
    //           break;
    //         case 2:
    //           std::cout << "You selected: Depth-first search\n";
    //           break;
    //     }
    // }

    // --- [Flow Control > Off-by-one Errors] ----------------------------------
    // Count the lines. "<= 10" runs ELEVEN times, not ten.
    // {
    //     for (int i{0}; i <= 10; i++) {
    //       std::cout << i << '\n';
    //     }
    // }

    // --- [Flow Control > break Statements] -----------------------------------
    // {
    //     // iterate 10 times
    //     for (auto i{0}; i < 10; ++i) {
    //         // exit loop if i is 3
    //         if (i == 3)
    //             break;  // exit the loop now
    //
    //         // otherwise print i
    //         std::cout << i << ' ';
    //     }
    //
    //     // execution will continue here after the break
    //     std::cout << "\nResuming program execution\n";
    // }

    // === Operations ==========================================================

    // --- [Flow Control > Increment/Decrement Operators] ----------------------
    // PREFIX: change the value first, then use it.
    // {
    //     int a{2};
    //     int b{++a}; // increment a first then initialize b
    //     int c{--a}; // decrement a first then initialize c
    //
    //     std::cout << a << ' ' << b << ' ' << c << '\n'; // 2 3 2
    // }
    //
    // POSTFIX: use the old value, then change it.
    // {
    //     int a{2};
    //     int b{a++}; // initialize b and then increment a
    //     int c{a--}; // initialize c and then decrement a
    //
    //     std::cout << a << ' ' << b << ' ' << c << '\n'; // 2 2 3
    // }

    // --- [Flow Control > Comma Symbol] ---------------------------------------
    // As an OPERATOR, the comma evaluates both sides and yields the right one.
    // {
    //     int x{1};
    //     int y{2};
    //     auto z{(++x, ++y)};// increment x, increment y, return y
    //     std::cout << x << ' ' << y << ' ' << z << '\n';  // 2 3 3
    // }
    //
    // The same thing written so a reader can follow it. Prefer this.
    // {
    //     int x{1};
    //     int y{2};
    //     ++x;        // increment x
    //     auto z{++y};// increment y and use its incremented value to initialize z
    //     std::cout << x << ' ' << y << ' ' << z << '\n';  // 2 3 3
    // }

    // --- [Flow Control > Short Circuit Evaluation] ---------------------------
    // && stops at the first false, so "b == 2" is never evaluated here.
    // {
    //     int a{2};
    //     int b{2};
    //     if (a == 1 && b == 2)
    //         std::cout << "if statement evaluated to true\n";
    //     else
    //         std::cout << "if statement evaluated to false\n";
    // }

    // #########################################################################
    // MODULE: EXCEPTION HANDLING
    // #########################################################################

    // --- [Exception Handling > try, catch, and throw] ------------------------
    // .at() checks the index; operator[] does not. This is the cheapest way
    // to see an exception in action.
    // {
    //     std::vector<int> readings{1, 2, 3, 4, 5};
    //
    //     try {
    //         std::cout << readings.at(10) << '\n';   // throws
    //     } catch (const std::out_of_range& e) {
    //         std::cout << "Caught out_of_range: " << e.what() << '\n';
    //     }
    // }

    // --- [Exception Handling > Throwing Exceptions] --------------------------
    // Uncomment divide() at the top of this file first.
    // {
    //     try {
    //         double result{divide(10.0, 0.0)};
    //         std::cout << "Result: " << result << '\n';
    //     } catch (const std::invalid_argument& e) {
    //         std::cout << "Error: " << e.what() << '\n';
    //     }
    // }

    // --- [Exception Handling > Custom Exception Classes] ---------------------
    // Uncomment SensorError and read_sensor() at the top of this file first.
    // {
    //     try {
    //         read_sensor("lidar_front", -1.5);
    //     } catch (const SensorError& e) {
    //         std::cout << "Sensor failure on " << e.sensor_name()
    //                   << ": " << e.what() << '\n';
    //     }
    // }

    // --- [Exception Handling > Best Practices for Catching] ------------------
    // Catch by CONST REFERENCE. Catching by value slices a derived exception
    // down to its base and loses the information you wanted.
    // {
    //     try {
    //         throw std::runtime_error("something failed");
    //     } catch (const std::exception& e) {   // good: by const reference
    //         std::cout << "Caught: " << e.what() << '\n';
    //     }
    // }

    // --- [Exception Handling > RAII and Exception Safety] --------------------
    // Uncomment process() at the top of this file first. The unique_ptr is
    // released during stack unwinding, with no delete written anywhere.
    // {
    //     try {
    //         process();
    //     } catch (const std::runtime_error& e) {
    //         std::cout << "Caught: " << e.what()
    //                   << " (the unique_ptr was already freed)\n";
    //     }
    // }

    // #########################################################################
    // MODULE: COMPILER WARNING FLAGS
    //
    // These build, but each one is meant to make the compiler complain. Build
    // with -Wall -Wextra (this project already does) and read the output.
    // #########################################################################

    // --- [Compiler Warnings > -Wall] -----------------------------------------
    // WARNS ON PURPOSE: the stray semicolon makes the if body empty, so the
    // std::cout below runs unconditionally.
    // {
    //     int count{0};
    //     if (count > 0);                 // warning: this if does nothing
    //         std::cout << "positive\n";  // always runs
    // }

    // --- [Compiler Warnings > -Wextra] ---------------------------------------
    // WARNS ON PURPOSE: comparing signed against unsigned.
    // {
    //     int i{-1};
    //     unsigned int u{1};
    //     if (i < u) {                    // warning: -Wsign-compare
    //         std::cout << "-1 is less than 1\n";
    //     } else {
    //         std::cout << "-1 is NOT less than 1\n";   // this one prints
    //     }
    // }

    // --- [Compiler Warnings > -Wpedantic] ------------------------------------
    // WARNS ON PURPOSE with -Wpedantic: a variable-length array is a GNU
    // extension, not standard C++. Use std::vector instead.
    // {
    //     int n{5};
    //     int arr[n];                     // warning: ISO C++ forbids VLA
    //     arr[0] = 1;
    //     std::cout << arr[0] << '\n';
    // }

    return 0;
}
