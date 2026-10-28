/**
 * @file which_one.cpp
 * @brief Rule of Five and Rule of Zero, the L8 reading on Read the Docs:
 *        Which Function Runs.
 * @author Zeid Kootbally
 *
 * @details A Vehicle that owns nothing. Each special member function prints its
 * name and the id, so each line of run() shows which one it calls. A move sets
 * the id of its source to 0, the way a real move leaves its source owning
 * nothing.
 * Run it: 702run week8_rule_of_five which_one
 */
#include <iostream>
#include <utility>

#include "parts.hpp"

namespace which_one {
class Vehicle {
 public:
  // Constructor
  explicit Vehicle(int id) : id_{id} {
    std::cout << "  constructor, id " << id_ << '\n';
  }
  // Destructor
  ~Vehicle() { std::cout << "  destructor, id " << id_ << '\n'; }
  // Copy constructor
  Vehicle(const Vehicle& other) : id_{other.id_} {
    std::cout << "  copy constructor, id " << id_ << '\n';
  }
  // Copy assignment
  Vehicle& operator=(const Vehicle& other) {
    id_ = other.id_;
    std::cout << "  copy assignment, id " << id_ << '\n';
    return *this;
  }
  // Move constructor
  Vehicle(Vehicle&& other) noexcept : id_{other.id_} {
    other.id_ = 0;  // as if the source gave its contents away
    std::cout << "  move constructor, id " << id_ << '\n';
  }
  // Move assignment
  Vehicle& operator=(Vehicle&& other) noexcept {
    id_ = other.id_;
    other.id_ = 0;
    std::cout << "  move assignment, id " << id_ << '\n';
    return *this;
  }
  // Accessor
  [[nodiscard]] int get_id() const noexcept { return id_; }

 private:
  int id_;
};

void park(Vehicle vehicle) {  // takes a Vehicle by value: a new object
  std::cout << "  park(), id " << vehicle.get_id() << '\n';
}  // vehicle is destroyed here

void run() {
  std::cout << "1: Vehicle car_a{1};\n";
  Vehicle car_a{1};
  std::cout << "2: Vehicle car_b{2};\n";
  Vehicle car_b{2};
  std::cout << "3: Vehicle copy{car_a};\n";
  Vehicle copy{car_a};
  std::cout << "4: Vehicle other = car_a;\n";
  Vehicle other = car_a;
  std::cout << "5: copy = car_b;\n";
  copy = car_b;
  std::cout << "6: Vehicle moved{std::move(car_a)};\n";
  Vehicle moved{std::move(car_a)};
  std::cout << "7: moved = std::move(car_b);\n";
  moved = std::move(car_b);
  std::cout << "8: park(copy);\n";
  park(copy);
  std::cout << "9: end of run()\n";
}
}  // namespace which_one
