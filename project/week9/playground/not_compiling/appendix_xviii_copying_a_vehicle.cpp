// [Appendix xviii] Copying a Vehicle of the Design. Uses the finished Taxi of
// ../include (compile with -I../include). Copying a Vehicle copies sensors_, a
// std::vector of std::unique_ptr, and a std::unique_ptr cannot be copied.
// Each line gives 4 errors, from inside std::vector. The copy is not suppressed:
// std::is_copy_constructible_v<fleet::Taxi> is true. [Appendix xix] shows the
// fix: Vehicle deletes its copy constructor and copy assignment (= delete).
#include <utility>

#include "taxi.hpp"

int main() {
  fleet::Taxi taxi{13.4, "Ana"};
  fleet::Taxi copy{taxi};               // does not compile
  fleet::Taxi moved{std::move(taxi)};   // does not compile either
}
