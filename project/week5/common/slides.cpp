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
std::string as_typed(int number, Part part) {
  return part == Part::lecture ? std::to_string(number) : to_roman(number);
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
  const std::string header{(part == Part::lecture ? "[Slide " : "[Appendix ") +
                           as_typed(slide.number, part) + "] " + std::string{slide.title}};
  const std::string rule(header.size(), '-');  // ( ), not { }: { } means a list of two chars
  std::cout << rule << '\n' << header << '\n' << rule << '\n';
}

}  // namespace

int run_slides(std::vector<Slide> slides, int argc, char* argv[], Part part) {
  std::stable_sort(slides.begin(), slides.end(),
                   [](const Slide& a, const Slide& b) { return a.number < b.number; });
  for (std::size_t i{1}; i < slides.size(); ++i) {
    if (slides[i].number == slides[i - 1].number) {
      std::cerr << "two entries for slide " << as_typed(slides[i].number, part) << ": \""
                << slides[i - 1].title << "\" and \"" << slides[i].title << "\"\n";
      return 1;
    }
  }

  const std::string program{std::filesystem::path{argv[0]}.filename().string()};

  if (argc == 1) {
    for (const Slide& slide : slides) {
      print_header(slide, part);
      if (slide.run_alone) {
        std::cout << "Skipped: this code has undefined behavior or throws, so it runs only on "
                     "its own: 702run "
                  << program << ' ' << as_typed(slide.number, part) << '\n';
      } else {
        slide.run();
      }
    }
    return 0;
  }

  const int number{argc == 2 ? parse_number(argv[1], part) : 0};
  if (number == 0) {
    std::cerr << "usage: 702run " << program
              << (part == Part::lecture ? " [slide number, such as 12]\n"
                                        : " [appendix frame, such as iv or 4]\n");
    return 1;
  }

  const auto match{std::find_if(slides.begin(), slides.end(),
                                [number](const Slide& slide) { return slide.number == number; })};
  if (match == slides.end()) {
    std::cerr << (part == Part::lecture ? "Slide " : "Appendix frame ") << as_typed(number, part)
              << " has no code in " << program << ". Frames with code:";
    for (const Slide& slide : slides) { std::cerr << ' ' << as_typed(slide.number, part); }
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
