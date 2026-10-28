/**
 * @file file_owner.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Beyond Memory: A File.
 * @author Zeid Kootbally
 *
 * @details TripLog owns an open file, a std::FILE* from the C library, and must
 * close it with std::fclose(). It follows the rule of five: no copies (two
 * owners would close one file twice), and moves that hand the file over.
 * std::tmpfile() opens a temporary file that the system deletes when the file
 * is closed, so the run leaves nothing behind.
 * Run it: 702run week8_rule_of_five file_owner
 */
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "parts.hpp"

namespace file_owner {
class TripLog {
 public:
  // Constructor: opens the file
  TripLog() : file_{std::tmpfile()} {
    if (file_ == nullptr) {
      throw std::runtime_error{"cannot open a temporary file"};
    }
  }
  // Destructor: closes the file, if this TripLog owns one
  ~TripLog() {
    if (file_ != nullptr) {
      std::fclose(file_);
      std::cout << "~TripLog: file closed\n";
    } else {
      std::cout << "~TripLog: no file\n";
    }
  }
  // Copy constructor and copy assignment: deleted, so no copies
  TripLog(const TripLog&) = delete;
  TripLog& operator=(const TripLog&) = delete;
  // Move constructor
  TripLog(TripLog&& other) noexcept : file_{other.file_} {
    other.file_ = nullptr;
  }
  // Move assignment
  TripLog& operator=(TripLog&& other) noexcept {
    if (this != &other) {
      if (file_ != nullptr) {
        std::fclose(file_);
      }
      file_ = other.file_;
      other.file_ = nullptr;
    }
    return *this;
  }
  // Writes one line to the file
  void write(const char* line) {
    std::fputs(line, file_);
    std::fputs("\n", file_);
  }

 private:
  std::FILE* file_;  // the open file, or nullptr after a move
};

void run() {
  TripLog log;
  log.write("trip 1: 4.2 km");
  TripLog kept{std::move(log)};
  kept.write("trip 2: 1.5 km");
}  // ~TripLog: file closed, then ~TripLog: no file
}  // namespace file_owner
