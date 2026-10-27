/**
 * @file appendix.cpp
 * @brief L8, OOP Basics: the code of every appendix frame that has code, and
 *        of the reading page Rule of Five and Rule of Zero, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_appendix.
 *
 * @code
 * 702build week8_appendix
 * 702run week8_appendix         # every appendix frame, then every reading part
 * 702run week8_appendix xi      # only [Appendix xi]; 702run week8_appendix 11 works too
 * 702run week8_appendix five    # only [Reading: five], a part of the reading page
 * @endcode
 *
 * The appendix frames are numbered i, ii, iii, ... in their top-left corner,
 * and @c "[Appendix xi]" is that number. Most appendix frames belong to a
 * section of the lecture, so their code is in that section's file in
 * ../../playground/src/, listed in its appendix_slides(): State and Behavior
 * (classes.cpp), Header and Source (encapsulation.cpp), The Compiler Checks
 * const and A Broken Rule Is Caught (accessors.cpp), A Constructor That Throws
 * and The Class So Far (constructors.cpp), Many Sensors (destructors.cpp),
 * const Objects (const_and_this.cpp and discussions.cpp) and static Members
 * (static_members.cpp). This file holds One Definition in Many Files.
 *
 * The reading page Rule of Five and Rule of Zero, on Read the Docs, is not
 * part of the deck. Its code has names instead of numbers, so they never
 * collide with a frame's number: each part's key is the name of its
 * namespace, as in @c "[Reading: five]". This file holds a Vehicle that owns
 * its Sensor through a raw pointer, and the five special members it must then
 * write (the rule of five). Each step's Vehicle is a class of its own, in its
 * own namespace, and leaves out the speed members of the lecture. The course
 * does not write classes like these: a std::unique_ptr member owns the heap
 * memory (Core Guidelines R.11, the rule of zero: [Reading: zero] in
 * ../../playground/src/destructors.cpp).
 *
 * Code that leaks or has undefined behavior is in
 * @c ../../undefined/src/undefined.cpp, built with the sanitizers; a full run
 * skips it, and it runs only when asked for by its key.
 */
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "sections.hpp"
#include "sensor.hpp"
#include "slides.hpp"
#include "speed.hpp"
#include "undefined.hpp"

double a();  // defined in speed_user.cpp

// [Appendix xi] One Definition in Many Files: kmh() is defined in
// ../include/speed.hpp, which this file and speed_user.cpp (the slide's a.cpp)
// both include, so the program holds two definitions of kmh(). They are the
// same, and kmh() is inline, so the program links. This run() is the slide's
// main(). Remove inline in speed.hpp and the program does not link (g++ 13.3,
// paths cut to ...):
// /usr/bin/ld: CMakeFiles/week8_appendix.dir/appendix/src/speed_user.cpp.o: in function `kmh(double)':
// .../appendix/src/speed_user.cpp:15: multiple definition of `kmh(double)';
//   CMakeFiles/week8_appendix.dir/appendix/src/appendix.cpp.o:.../appendix/include/speed.hpp:15: first defined here
// collect2: error: ld returned 1 exit status
// (the slide builds a.cpp and main.cpp without -g, so its error names them
// and shows (.text+0x0) instead of a line)
namespace one_definition {
void run() {
  std::cout << kmh(12.0) << ' '
            << a() << '\n';  // 43.2 36
}
}  // namespace one_definition

// Appendix frames xii, A Method Defined in Its Class, and xiii, inline Does
// Not Force Inlining, have no code to run. They compare the
// assembly g++ 13.3 writes for a call of the in-class get_speed() of Short
// Methods in the Class (lecture slide 42): with -O2 the call becomes one read
// of speed_, with -O0 it stays a call. With the body in another source file,
// the call stays a call at -O2.

// [Reading: raw_owner] A Class That Owns Memory: one Sensor on the heap,
// through a raw pointer. The constructor calls new, so the destructor calls delete.
namespace raw_owner {
class Vehicle {  // a Sensor on the heap
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() {
    std::cout << "~Vehicle: sensor "
              << sensor_->get_id()
              << '\n';
    delete sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  {
    Vehicle car_a{Sensor{1}};
    Vehicle car_b{Sensor{2}};
  }
  auto spare_car{
    std::make_unique<Vehicle>(
        Sensor{3})};
  spare_car.reset();
}  // prints ~Vehicle: sensor 2, then 1, then 3
}  // namespace raw_owner

// [Reading: no_destructor] Without the Destructor: undefined::no_destructor
// [Reading: double_free] Copying an Owner, and Double Free, Measured:
// undefined::double_free

// [Reading: five] Rule of Five, The Copies, The Moves, and All Five, Run.
// ~Vehicle prints as in [Reading: raw_owner], and 0 when the Vehicle owns
// nothing (after a move).
namespace five {
class Vehicle {
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}

  ~Vehicle() {
    std::cout << "~Vehicle: sensor " << (sensor_ != nullptr ? sensor_->get_id() : 0)
              << '\n';
    delete sensor_;
  }

  // The Copies
  Vehicle(const Vehicle& other)  // a new Sensor with other's values, if any
      : sensor_{other.sensor_ != nullptr ? new Sensor{*other.sensor_} : nullptr} {}

  Vehicle& operator=(const Vehicle& other) {
    if (this != &other) {  // v = v must not delete its own Sensor
      // Copy first: if new throws, this Vehicle keeps its old Sensor.
      Sensor* copy{other.sensor_ != nullptr ? new Sensor{*other.sensor_} : nullptr};
      delete sensor_;  // give back what we owned
      sensor_ = copy;
    }
    return *this;
  }

  // The Moves
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;  // the source owns nothing now
  }

  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {   // v = std::move(v) must not delete
      delete sensor_;       // give back what we owned
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

// All Five, Run
void run() {
  Vehicle car_b{Sensor{2}};
  Vehicle copy{car_b};
  std::cout << "copy: sensor " << copy.get_sensor().get_id() << ", own Sensor: "
            << std::boolalpha << (&copy.get_sensor() != &car_b.get_sensor())
            << '\n';  // copy: sensor 2, own Sensor: true
  Vehicle car_a{Sensor{1}};
  copy = car_a;
  std::cout << "after copy assignment: sensor " << copy.get_sensor().get_id()
            << '\n';  // 1
  Vehicle moved{std::move(car_b)};
  std::cout << "moved: sensor " << moved.get_sensor().get_id() << '\n';  // 2
  car_a = std::move(moved);
  std::cout << "after move assignment: sensor " << car_a.get_sensor().get_id()
            << '\n';  // 2
}  // ~Vehicle: sensor 0, 2, 1, 0
}  // namespace five

// [Reading: move_noexcept] Why the Moves Are noexcept: a Vehicle that only
// counts its copies and moves. A growing std::vector moves its elements only
// when the move constructor is noexcept; otherwise it copies them.
namespace move_noexcept {
template <bool NoExcept>
struct Counts {
  static inline int copies{0};
  static inline int moves{0};
};

template <bool NoExcept>
class Vehicle {  // counts its copies and moves
 public:
  Vehicle() = default;
  Vehicle(const Vehicle&) { ++Counts<NoExcept>::copies; }
  Vehicle(Vehicle&&) noexcept(NoExcept) { ++Counts<NoExcept>::moves; }
};

template <bool NoExcept>
void grow() {
  std::vector<Vehicle<NoExcept>> fleet;
  for (int i{0}; i < 5; ++i) {
    fleet.emplace_back();
  }
  std::cout << (NoExcept ? "with noexcept: " : "without it: ")
            << Counts<NoExcept>::copies << " copies, "
            << Counts<NoExcept>::moves << " moves\n";
}

void run() {
  grow<true>();   // with noexcept: 0 copies, 7 moves
  grow<false>();  // without it: 7 copies, 0 moves
}
}  // namespace move_noexcept

// [Reading: move_bug] Discussion: Who Deletes What?: undefined::move_bug

// [Reading: move_only] = default and = delete: the raw-pointer Vehicle, move-only.
// Remove the two move lines and Vehicle moved{std::move(car_b)}; picks the
// deleted copy:
// error: use of deleted function 'move_only::Vehicle::Vehicle(const move_only::Vehicle&)'
// (the page's code has no namespaces, so the page's error names Vehicle only)
namespace move_only {
class Vehicle {  // the raw-pointer Vehicle, move-only on purpose
 public:
  explicit Vehicle(const Sensor& sensor)
      : sensor_{new Sensor{sensor}} {}
  ~Vehicle() { delete sensor_; }
  Vehicle(const Vehicle&) = delete;             // no copies
  Vehicle& operator=(const Vehicle&) = delete;
  Vehicle(Vehicle&& other) noexcept : sensor_{other.sensor_} {
    other.sensor_ = nullptr;
  }
  Vehicle& operator=(Vehicle&& other) noexcept {
    if (this != &other) {
      delete sensor_;
      sensor_ = other.sensor_;
      other.sensor_ = nullptr;
    }
    return *this;
  }
  [[nodiscard]] const Sensor& get_sensor() const noexcept {
    return *sensor_;
  }

 private:
  Sensor* sensor_;
};

void run() {
  Vehicle car_b{Sensor{2}};
  Vehicle moved{std::move(car_b)};
  std::cout << moved.get_sensor().get_id() << '\n';  // 2
}
}  // namespace move_only

int main(int argc, char* argv[]) {
  // One entry per appendix frame that has code: its number (11 is xi), its
  // title, and the function that runs it. Then one entry per part of the
  // reading page, with 0 for its number and its key last. true: the code
  // leaks or has undefined behavior, so it runs only when asked for.
  std::vector<Slide> slides{
      {11, "One Definition in Many Files", one_definition::run},
      {0, "A Class That Owns Memory", raw_owner::run, false, "raw_owner"},
      {0, "Without the Destructor", undefined::no_destructor::run, true, "no_destructor"},
      {0, "Double Free, Measured", undefined::double_free::run, true, "double_free"},
      {0, "Why the Moves Are noexcept", move_noexcept::run, false, "move_noexcept"},
      {0, "All Five, Run", five::run, false, "five"},
      {0, "Discussion: Who Deletes What?", undefined::move_bug::run, true, "move_bug"},
      {0, "= default and = delete", move_only::run, false, "move_only"},
  };
  // The frames of the other files, and [Reading: zero] of destructors.cpp.
  for (const std::vector<Slide>& section :
       {classes::appendix_slides(), encapsulation::appendix_slides(),
        accessors::appendix_slides(), constructors::appendix_slides(),
        destructors::appendix_slides(), const_and_this::appendix_slides(),
        discussions::appendix_slides(), static_members::appendix_slides()}) {
    slides.insert(slides.end(), section.begin(), section.end());
  }
  return run_slides(slides, argc, argv, Part::appendix);
}
