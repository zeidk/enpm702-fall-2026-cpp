// [Slide 21] Exercise 1: A Task Type
//
// About 5 minutes. Build and run:  702build week6_ex1 && 702run week6_ex1
//
// 1. Declare struct Task with these members, in this order, each with a
//    default: int id (0), Position pickup ({}), double deadline_s (60.0),
//    bool assigned (false).
// 2. Write void print_task(const Task& t). For task 17 it prints:
//      task 17: pickup (5, 5), deadline 30 s, unassigned
// 3. Un-comment the five lines in main, build and run.
// 4. Reorder the members of Task so that sizeof(Task) is 32, not 40.
//    Build again, and fix the line the compiler now rejects.
//
// Expected output after step 4:
//   task 17: pickup (5, 5), deadline 30 s, unassigned
//   task 18: pickup (0, 0), deadline 60 s, unassigned
//   32
#include <iostream>

struct Position {
  double x{0.0};
  double y{0.0};
};

// Step 1: struct Task goes here.

// Step 2: print_task goes here.

int main() {
  // Step 3: un-comment these five lines.
  // Task urgent{.id = 17, .pickup = {5.0, 5.0}, .deadline_s = 30.0};
  // Task later{.id = 18};
  // print_task(urgent);
  // print_task(later);
  // std::cout << sizeof(Task) << '\n';
}
