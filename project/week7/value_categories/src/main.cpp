/**
 * @file main.cpp
 * @brief The program of the reading page Value Categories, on Read the Docs:
 *        target week7_value_categories.
 * @author Zeid Kootbally
 *
 * @details One function per part of the page. Each part has a key:
 * @code
 * 702build week7_value_categories
 * 702run week7_value_categories            # every part that is safe to run
 * 702run week7_value_categories examples   # only the part examples
 * @endcode
 * Keys, in the order of the page: helper, examples, traps, dangling,
 * overloads, exercises. The part dangling reads freed memory, so a full run
 * skips it, and it runs only when asked for by its key. The program is built
 * with AddressSanitizer, which stops it there with a report.
 */
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "category.hpp"
#include "obstacle.hpp"

namespace {

// A path of n poses, one meter apart along the road (Lecture 7).
std::vector<Pose> make_path(int n) {
  std::vector<Pose> path;
  path.reserve(n);  // room for n poses: one allocation
  for (int i{0}; i < n; ++i) {
    path.push_back({static_cast<double>(i), 0.0, 0.0});
  }
  return path;
}

// An Obstacle by value (Lecture 7).
Obstacle make_obstacle(int id) {
  return Obstacle{id};  // the other members take their defaults
}

// The Obstacle with this id, or the first one when no id matches (Lecture 7,
// Exercise 2).
Obstacle& lookup(std::vector<Obstacle>& seen, int id) {
  for (Obstacle& obstacle : seen) {
    if (obstacle.id == id) {
      return obstacle;
    }
  }
  return seen[0];
}

// The first pose of a path.
const Pose& first_pose(const std::vector<Pose>& path) {
  return path[0];
}

// Overload 1 reads its argument; overload 2 may take it.
void take(const std::vector<Pose>&) {
  std::cout << "1 const&\n";
}
void take(std::vector<Pose>&&) {
  std::cout << "2 &&\n";
}

// ---------------------------------------------------------------------------
// The CATEGORY Helper
// ---------------------------------------------------------------------------
void helper() {
  int count{3};
  std::vector<int> path{1, 2, 3};
  std::cout << CATEGORY(count) << '\n';
  std::cout << CATEGORY(count + 1) << '\n';
  std::cout << CATEGORY(std::move(path)) << '\n';
  std::cout << CATEGORY(count++) << ' ' << count << '\n';  // 3: not evaluated
  std::cout << path.size() << '\n';                        // 3: nothing moved
  std::cout << CATEGORY((Pose{1.0, 2.0, 0.0})) << '\n';    // extra parentheses
  static_assert(CATEGORY(count) == "lvalue");
  static_assert(CATEGORY(std::move(path)) == "xvalue");
  Pose pose{};
  static_assert(CATEGORY(std::move(pose).x) == "xvalue");
}

// ---------------------------------------------------------------------------
// Worked Examples: the 54 expressions of the page's tables
// ---------------------------------------------------------------------------
void examples() {
  // The declarations of the page. Some are used only inside CATEGORY, which
  // does not evaluate them, so g++ would call them unused.
  int count{3};
  [[maybe_unused]] int other{7};
  [[maybe_unused]] const double limit{40.0};
  int& alias{count};
  int numbers[3]{10, 20, 30};
  int* p{&numbers[1]};
  Pose pose{1.0, 2.0, 0.0};
  Pose* pp{&pose};
  Obstacle obstacle{2, {12.0, -1.0, 0.0}, 4.0};
  std::vector<Pose> path{make_path(3)};
  std::vector<Pose>&& r{make_path(10)};
  std::vector<Obstacle> seen{make_obstacle(0), make_obstacle(1)};
  std::string a{"cyc"};
  std::string b{"list"};

  std::cout << "Names (Step 1)\n";
  SHOW(count);
  SHOW(limit);
  SHOW(alias);
  SHOW(numbers);
  SHOW(p);
  SHOW(path);
  SHOW(r);

  std::cout << "Literals (Step 2)\n";
  SHOW(42);
  SHOW(3.5);
  SHOW(true);
  SHOW('a');
  SHOW(nullptr);
  SHOW("hello");

  std::cout << "Function Calls and Casts (Step 3)\n";
  SHOW(make_path(3));
  SHOW(make_obstacle(3));
  SHOW(lookup(seen, 1));
  SHOW(first_pose(path));
  SHOW(seen[0]);
  SHOW(seen.front());
  SHOW(seen.size());
  SHOW(a + b);
  SHOW(std::move(path));
  SHOW(static_cast<std::vector<Pose>&&>(path));
  SHOW(static_cast<int&>(count));
  SHOW(static_cast<double>(count));

  std::cout << "Built-in Operators (Step 4)\n";
  SHOW(count + 1);
  SHOW(count * 2);
  SHOW(-count);
  SHOW(count == 3);
  SHOW(&count);
  SHOW(*p);
  SHOW(numbers[1]);
  SHOW(++count);
  SHOW(count++);
  SHOW(count = 5);
  SHOW(count += 1);
  SHOW(count > 0 ? count : other);
  SHOW(count > 0 ? count : 0);

  std::cout << "Member Access (Step 4)\n";
  SHOW(pose.x);
  SHOW(obstacle.pose.x);
  SHOW(seen[1].pose);
  SHOW(pp->x);
  SHOW(std::move(obstacle).pose);
  SHOW(std::move(obstacle).pose.x);
  SHOW(make_obstacle(3).id);
  SHOW(Pose{}.x);
  SHOW(std::move(pp)->x);
  SHOW(make_path(3)[0]);
  SHOW(make_path(3)[0].x);
  SHOW(make_path(3).size());

  std::cout << "Objects Made on the Spot\n";
  SHOW(Pose{});
  SHOW((Pose{1.0, 2.0, 0.0}));             // extra parentheses: commas
  SHOW((std::vector<int>{1, 2, 3}));       // extra parentheses: commas
  SHOW(std::string{"cyclist"});

  std::cout << "count is still " << count << ": CATEGORY evaluates nothing\n";
}

// ---------------------------------------------------------------------------
// Traps 1, 2, 3 and 5 (Trap 4 is the part dangling)
// ---------------------------------------------------------------------------
void inside(std::vector<Pose>&& path) {
  take(path);             // path is a name: an lvalue
  take(std::move(path));  // an xvalue
}

void traps() {
  std::cout << "Trap 1: The Type Is Not the Category\n";
  std::vector<Pose>&& r{make_path(10)};
  static_assert(std::is_same_v<decltype(r), std::vector<Pose>&&>);   // the type of r
  static_assert(std::is_same_v<decltype((r)), std::vector<Pose>&>);  // r is an lvalue
  SHOW(r);
  inside(make_path(2));

  std::cout << "Trap 2: std::move Moves Nothing\n";
  const std::vector<Pose> fixed{make_path(2)};
  std::vector<Pose> dest{std::move(fixed)};  // an xvalue, and a copy
  std::cout << "  fixed.size(): " << fixed.size() << '\n';  // 2: nothing was taken

  std::cout << "Trap 3: \"Left Side of =\" Is Not the Test\n";
  make_obstacle(3) = Obstacle{};  // a prvalue on the left: compiles
  // const double limit{40.0};
  // limit = 2.0;      // error: assignment of read-only variable 'limit'
  // int count{3};
  // 5 = count;        // error: lvalue required as left operand of assignment
  // count++ = 3;      // error: lvalue required as left operand of assignment
  std::cout << "  make_obstacle(3) = Obstacle{} compiled\n";

  std::cout << "Trap 5: The Arrow Always Gives an lvalue\n";
  Pose pose{1.0, 2.0, 0.0};
  Pose* pp{&pose};
  SHOW(std::move(pose).x);
  SHOW(std::move(pp)->x);
}

// ---------------------------------------------------------------------------
// Trap 4: An lvalue Can Refer Into a Temporary. Reads freed memory: runs only
// when asked for by its key, and AddressSanitizer stops it at the last line.
// ---------------------------------------------------------------------------
void dangling() {
  double safe{make_path(3)[1].x};       // reads before the vector is destroyed
  std::cout << safe << std::endl;       // flush: the next lines stop the program
  const std::vector<Pose>& whole{make_path(3)};  // binds to the temporary itself
  std::cout << whole[2].x << std::endl;          // 2, the temporary lives; flush
  const Pose& second{make_path(3)[1]};  // refers into a destroyed vector
  std::cout << second.x << '\n';        // reads freed memory
}

// ---------------------------------------------------------------------------
// Why the Category Matters: which overload a call picks
// ---------------------------------------------------------------------------
void overloads() {
  std::vector<Pose> path{make_path(3)};
  std::vector<Pose>&& r{make_path(10)};
  const std::vector<Pose> fixed{make_path(2)};
  take(path);              // lvalue
  take(make_path(5));      // prvalue
  take(r);                 // lvalue: a name
  take(std::move(r));      // xvalue
  take(std::move(path));   // xvalue
  take(fixed);             // lvalue
  take(std::move(fixed));  // xvalue, but const
}

// ---------------------------------------------------------------------------
// The exercises page: the answers of Exercises 1 to 5 and 7. Exercise 6 is
// the part dangling.
// ---------------------------------------------------------------------------
struct Sensor {
  int id{0};
  double range{0.0};
};

Sensor make_sensor(int id) {
  return Sensor{id};
}
Sensor& front_sensor(std::vector<Sensor>& sensors) {
  return sensors.front();
}
const Sensor& rear_sensor(const std::vector<Sensor>& sensors) {
  return sensors.back();
}
Sensor&& release(Sensor& sensor) {
  return std::move(sensor);
}

void store(const Sensor&) {
  std::cout << "copy\n";
}
void store(Sensor&&) {
  std::cout << "move\n";
}

struct Planner {
  std::vector<Pose> path;
};

// Exercise 5 as given: path is a name, so the assignment copies.
void give_path(Planner& planner, std::vector<Pose>&& path) {
  planner.path = path;
}

// Exercise 5, fixed: mark the parameter again.
void give_path_fixed(Planner& planner, std::vector<Pose>&& path) {
  planner.path = std::move(path);
}

void exercises() {
  std::cout << "Exercise 1\n";
  int speed{30};
  const int limit{50};
  int& ref{speed};
  int lanes[2]{1, 2};
  int* lane{&lanes[0]};
  SHOW(speed);
  SHOW(limit);
  SHOW(ref);
  SHOW(30);
  SHOW(speed - 5);
  SHOW(*lane);
  SHOW(lanes[1]);
  SHOW(speed--);
  SHOW(--speed);
  SHOW(speed < limit);
  SHOW(&speed);
  SHOW(speed = limit);
  SHOW("lane");

  std::cout << "Exercise 2\n";
  std::vector<Sensor> sensors{make_sensor(1), make_sensor(2)};
  std::string name{"lidar"};
  SHOW(make_sensor(1));
  SHOW(front_sensor(sensors));
  SHOW(rear_sensor(sensors));
  SHOW(release(sensors[0]));
  SHOW(sensors.size());
  SHOW(sensors.back());
  SHOW(name + "_front");
  SHOW(std::move(name));
  SHOW(static_cast<double>(sensors.size()));
  SHOW(static_cast<Sensor&&>(sensors[0]));

  std::cout << "Exercise 3\n";
  SHOW(sensors[0].range);
  SHOW(make_sensor(1).range);
  SHOW(std::move(sensors[0]).id);
  SHOW(release(sensors[0]).range);
  SHOW(front_sensor(sensors).id);
  SHOW(Sensor{}.id);
  SHOW(Sensor{});
  SHOW(sensors.data()->range);
  SHOW(std::vector<Sensor>{}.size());
  SHOW((*sensors.data()).id);

  std::cout << "Exercise 4\n";
  Sensor lidar{1, 100.0};
  Sensor&& radar{make_sensor(2)};
  const Sensor camera{3, 50.0};
  store(lidar);
  store(make_sensor(4));
  store(radar);
  store(std::move(radar));
  store(camera);
  store(std::move(camera));
  store(Sensor{5, 20.0});
  store(release(lidar));

  std::cout << "Exercise 5\n";
  Planner planner{};
  std::vector<Pose> path{make_path(1000)};
  const Pose* where{path.data()};  // where path's block is
  give_path(planner, std::move(path));
  std::cout << std::boolalpha << (planner.path.data() == where) << ' '
            << path.size() << '\n';
  Planner fixed_planner{};
  std::vector<Pose> fixed_path{make_path(1000)};
  const Pose* fixed_where{fixed_path.data()};
  give_path_fixed(fixed_planner, std::move(fixed_path));
  std::cout << (fixed_planner.path.data() == fixed_where) << ' '
            << fixed_path.size() << " (fixed)\n";

  std::cout << "Exercise 7\n";
  std::vector<Pose> seen{make_path(3)};
  static_assert(CATEGORY(seen) == "lvalue");             // a name
  static_assert(CATEGORY(std::move(seen)) == "xvalue");  // returns T&&
  static_assert(CATEGORY(seen.size()) == "prvalue");     // returns by value
  SHOW(seen[0]);
  SHOW(seen.front().x);
  SHOW(std::move(seen[0]));
  SHOW(seen.empty());
}

/// One part of the page: its key, its title, the function that runs it, and
/// whether it runs only when asked for by its key.
struct Part {
  std::string_view key;
  std::string_view title;
  void (*run)();
  bool run_alone{false};
};

}  // namespace

int main(int argc, char* argv[]) {
  const std::vector<Part> parts{
      {"helper", "The CATEGORY Helper", helper},
      {"examples", "Worked Examples", examples},
      {"traps", "Traps", traps},
      {"dangling", "Trap 4: An lvalue Can Refer Into a Temporary", dangling, true},
      {"overloads", "Why the Category Matters", overloads},
      {"exercises", "C++ Exercises", exercises},
  };

  if (argc > 2) {
    std::cerr << "usage: week7_value_categories [key]\n";
    return 1;
  }
  const std::string_view wanted{argc == 2 ? argv[1] : ""};
  bool found{wanted.empty()};
  for (const Part& part : parts) {
    if (wanted.empty() ? part.run_alone : part.key != wanted) {
      continue;
    }
    std::cout << "=== " << part.title << " (" << part.key << ") ===\n";
    part.run();
    found = true;
  }
  if (!found) {
    std::cerr << "no part with the key " << wanted << ". Keys:";
    for (const Part& part : parts) {
      std::cerr << ' ' << part.key;
    }
    std::cerr << '\n';
    return 1;
  }
  return 0;
}
