/**
 * @file allocations.cpp
 * @brief L7 [Appendix xiv], make_shared Allocates Once: counts the heap
 *        allocations of std::make_shared and of std::shared_ptr{new ...}.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week7_allocations, built without the sanitizers.
 *
 * @code
 * 702run week7_allocations
 * @endcode
 *
 * The count works by replacing the global operator new, which this course
 * does not teach, and the sanitizers replace it too, so this frame is a
 * program of its own. Only the allocations made while @c counting is true are
 * recorded; every other allocation goes through unchanged.
 */
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>

#include "fleet.hpp"

namespace {
bool counting{false};
std::size_t sizes[8]{};
int count{0};

// Prints the allocations recorded since the last report, then clears them.
void report(const char* what) {
  std::cout << what << ": " << count << " allocation(s):";
  for (int i{0}; i < count; ++i) {
    std::cout << ' ' << sizes[i];
  }
  std::cout << " bytes\n";
  count = 0;
}
}  // namespace

void* operator new(std::size_t size) {
  if (counting && count < 8) {
    sizes[count++] = size;
  }
  if (void* p{std::malloc(size)}) {
    return p;
  }
  throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int main() {
  counting = true;
  auto made{std::make_shared<RobotStatus>(3, 14.0)};
  counting = false;
  report("std::make_shared<RobotStatus>");  // 1 allocation: 56 bytes

  counting = true;
  std::shared_ptr<RobotStatus> from_new{new RobotStatus{3, 14.0}};
  counting = false;
  report("std::shared_ptr<RobotStatus>{new ...}");  // 2 allocations: 40 24 bytes

  std::cout << "sizeof(RobotStatus): " << sizeof(RobotStatus) << '\n';  // 40
}
