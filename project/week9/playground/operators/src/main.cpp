/**
 * @file main.cpp
 * @brief The program of the L9 reading page Operator Overloading, on Read the
 *        Docs: target week9_operators.
 * @author Zeid Kootbally
 *
 * @details One source file per part of the page. Each part has a key, the
 * name of its file and of its namespace:
 * @code
 * 702build week9_operators
 * 702run week9_operators          # every part
 * 702run week9_operators print    # only the part print
 * @endcode
 */
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "parts.hpp"

namespace {

/// One part of the reading page.
struct Part {
  std::string_view key;    ///< What you type after 702run week9_operators.
  std::string_view title;  ///< The title of the part on the page.
  void (*run)();           ///< Runs the code of the part.
};

void print_header(const Part& part) {
  const std::string header{"[Reading: " + std::string{part.key} + "] " +
                           std::string{part.title}};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
}

}  // namespace

void print_commented_out(std::string_view file) {
  std::cout << "This part's code does not compile, so it is commented out in\n"
            << file << ". Uncomment it and build to see the message.\n";
}

int main(int argc, char* argv[]) {
  // One entry per part, in the order of the page.
  const std::vector<Part> parts{
      {"calls", "What an Operator Overload Is", calls::run},
      {"print", "operator<< for Position", print::run},
      {"compare", "operator< and operator> for Route", compare::run},
      {"member_output", "Mistake: operator<< as a Member Function", member_output::run},
      {"by_value", "Mistake: Returning the Stream by Value", by_value::run},
      {"no_const", "Mistake: A Comparison Without const", no_const::run},
  };

  if (argc == 1) {
    for (const Part& part : parts) {
      print_header(part);
      part.run();
    }
    return 0;
  }
  if (argc == 2) {
    const std::string_view asked{argv[1]};
    for (const Part& part : parts) {
      if (part.key == asked) {
        print_header(part);
        part.run();
        return 0;
      }
    }
    std::cerr << asked << " is not a part of week9_operators.\n";
  }
  std::cerr << "usage: 702run week9_operators [the key of a part, such as print]\n"
            << "Parts:";
  for (const Part& part : parts) {
    std::cerr << ' ' << part.key;
  }
  std::cerr << '\n';
  return 1;
}
