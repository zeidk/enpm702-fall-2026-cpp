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
      {100, "c"}, {90, "xc"}, {50, "l"}, {40, "xl"}, {10, "x"},
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

// How the slide is asked for on the command line: "12", or "iv" in the appendix.
std::string as_typed(const Slide& slide) {
  return slide.part == Part::lecture ? std::to_string(slide.number) : to_roman(slide.number);
}

// What the command line names: "12" is [Slide 11]; "iv" and "IV" are
// [Appendix iv]. A number of 0 means neither.
struct Wanted {
  int number{0};
  Part part{Part::lecture};
};

Wanted parse_wanted(std::string_view text) {
  int number{0};
  const char* end{text.data() + text.size()};
  const std::from_chars_result result{std::from_chars(text.data(), end, number)};
  if (result.ec == std::errc{} && result.ptr == end && number > 0) {
    return {number, Part::lecture};
  }
  std::string lower;
  for (char c : text) {
    lower += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  for (int n{1}; n < 400; ++n) {
    if (to_roman(n) == lower) { return {n, Part::appendix}; }
  }
  return {};
}

void print_header(const Slide& slide) {
  const std::string header{(slide.part == Part::lecture ? "[Slide " : "[Appendix ") +
                           as_typed(slide) + "] " + std::string{slide.title}};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
}

}  // namespace

int run_slides(std::vector<Slide> slides, int argc, char* argv[]) {
  // The main deck first, then the appendix, each in frame order.
  std::stable_sort(slides.begin(), slides.end(), [](const Slide& a, const Slide& b) {
    return std::pair{a.part, a.number} < std::pair{b.part, b.number};
  });
  for (std::size_t i{1}; i < slides.size(); ++i) {
    if (slides[i].part == slides[i - 1].part && slides[i].number == slides[i - 1].number) {
      std::cerr << "two entries for " << as_typed(slides[i]) << ": \""
                << slides[i - 1].title << "\" and \"" << slides[i].title << "\"\n";
      return 1;
    }
  }

  const std::string program{std::filesystem::path{argv[0]}.filename().string()};

  if (argc == 1) {
    for (const Slide& slide : slides) {
      print_header(slide);
      if (slide.run_alone) {
        std::cout << "Skipped: this code leaks or has undefined behavior, so it runs only on "
                     "its own: 702run "
                  << program << ' ' << as_typed(slide) << '\n';
      } else {
        slide.run();
      }
    }
    return 0;
  }

  const Wanted wanted{argc == 2 ? parse_wanted(argv[1]) : Wanted{}};
  if (wanted.number == 0) {
    std::cerr << "usage: 702run " << program
              << " [slide number, such as 12, or appendix frame, such as iv]\n";
    return 1;
  }

  const auto match{std::find_if(slides.begin(), slides.end(), [wanted](const Slide& slide) {
    return slide.part == wanted.part && slide.number == wanted.number;
  })};
  if (match == slides.end()) {
    std::cerr << argv[1] << " has no code in " << program << ". Frames with code:";
    for (const Slide& slide : slides) { std::cerr << ' ' << as_typed(slide); }
    std::cerr << '\n';
    return 1;
  }
  if (match->run_alone) {
    // The code may crash. Flush after every output, so what it printed before
    // the crash still appears when the output goes to a pipe or a file.
    std::cout << std::unitbuf;
  }
  print_header(*match);
  match->run();
  return 0;
}
