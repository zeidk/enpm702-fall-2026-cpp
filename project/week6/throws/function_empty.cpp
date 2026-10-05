#include <functional>
#include <map>
#include <string>

int main() {
  std::map<std::string, std::function<void(int)>> on_command;
  on_command["reboot"](2);  // no handler was ever stored
}

// [Slide 70] An Empty std::function
// THROWS. operator[] inserts an empty std::function for the missing key, and
// calling an empty std::function throws std::bad_function_call.
// Run: 702run week6_function_empty
