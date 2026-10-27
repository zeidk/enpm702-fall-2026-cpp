/**
 * @file slides.cpp
 * @brief Runs a list of slides: all of them, or the one asked for.
 * @author Zeid Kootbally
 */
#include "slides.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>

namespace {

// 4 gives "iv": the appendix frames are numbered this way in the deck.
std::string to_roman(int number) {
  constexpr std::array<std::pair<int, std::string_view>, 9> digits{{
      {100, "c"}, {89, "xc"}, {49, "l"}, {40, "xl"}, {10, "x"},
      {9, "ix"}, {5, "v"}, {4, "iv"}, {1, "i"}}};
  std::string roman;
  for (const auto& [value, letters] : digits) {
    while (number >= value) {
      roman += letters;
      number -= value;
    }
  }
  return roman;
}

// How the slide is asked for on the command line: "12", "iv" in the appendix,
// or the key of a reading part, such as "five".
std::string as_typed(const Slide& slide, Part part) {
  if (!slide.key.empty()) { return std::string{slide.key}; }
  return part == Part::lecture ? std::to_string(slide.number) : to_roman(slide.number);
}

// "12" gives 12. In the appendix, "iv" and "IV" give 4 too. 0 means neither.
int parse_number(std::string_view text, Part part) {
  int number{0};
  const char* end{text.data() + text.size()};
  const std::from_chars_result result{std::from_chars(text.data(), end, number)};
  if (result.ec == std::errc{} && result.ptr == end && number > 0) {
    return number;
  }
  if (part == Part::appendix) {
    std::string lower;
    for (char c : text) {
      lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    for (int n{1}; n < 400; ++n) {
      if (to_roman(n) == lower) { return n; }
    }
  }
  return 0;
}

void print_header(const Slide& slide, Part part) {
  std::string label{"[Reading: "};
  if (slide.key.empty()) { label = part == Part::lecture ? "[Slide " : "[Appendix "; }
  const std::string header{label + as_typed(slide, part) + "] " + std::string{slide.title}};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
}

}  // namespace

int run_slides(std::vector<Slide> slides, int argc, char* argv[], Part part) {
  // The frames first, by number; then the reading parts, in the order listed.
  std::stable_sort(slides.begin(), slides.end(), [](const Slide& a, const Slide& b) {
    const bool a_named{!a.key.empty()};
    const bool b_named{!b.key.empty()};
    if (a_named != b_named) { return b_named; }
    return !a_named && a.number < b.number;
  });
  for (std::size_t i{0}; i < slides.size(); ++i) {
    for (std::size_t j{i + 1}; j < slides.size(); ++j) {
      if (as_typed(slides[i], part) == as_typed(slides[j], part)) {
        std::cerr << "two entries for " << as_typed(slides[i], part) << ": \""
                  << slides[i].title << "\" and \"" << slides[j].title << "\"\n";
        return 1;
      }
    }
  }

  const std::string program{std::filesystem::path{argv[0]}.filename().string()};

  if (argc == 1) {
    for (const Slide& slide : slides) {
      print_header(slide, part);
      if (slide.run_alone) {
        std::cout << "Skipped: this code stops the program, leaks or has undefined behavior, "
                     "so it runs only on its own: 702run "
                  << program << ' ' << as_typed(slide, part) << '\n';
      } else {
        slide.run();
      }
    }
    return 0;
  }

  const Slide* match{nullptr};
  if (argc == 2) {
    const std::string_view asked{argv[1]};
    const int number{parse_number(asked, part)};
    for (const Slide& slide : slides) {
      const bool named_match{!slide.key.empty() && slide.key == asked};
      const bool number_match{slide.key.empty() && number != 0 && slide.number == number};
      if (named_match || number_match) {
        match = &slide;
        break;
      }
    }
  }
  if (match == nullptr) {
    if (argc == 2) { std::cerr << argv[1] << " is not a slide with code in " << program << ".\n"; }
    std::cerr << "usage: 702run " << program
              << (part == Part::lecture ? " [slide number, such as 9]\n"
                                        : " [appendix frame, such as xi or 11, or a reading key]\n")
              << "Slides with code in " << program << ':';
    for (const Slide& slide : slides) { std::cerr << ' ' << as_typed(slide, part); }
    std::cerr << '\n';
    return 1;
  }
  if (match->run_alone) {
    // The code may crash. Flush after every output, so what it printed before
    // the crash still appears when the output goes to a pipe or a file.
    std::cout << std::unitbuf;
  }
  print_header(*match, part);
  match->run();
  return 0;
}

void print_commented_out(std::string_view file) {
  std::cout << "This slide's code does not compile, or makes the compiler warn, so it is\n"
               "commented out in "
            << file
            << ", with what the compiler says.\n"
               "Uncomment it and build to see the message.\n";
}
