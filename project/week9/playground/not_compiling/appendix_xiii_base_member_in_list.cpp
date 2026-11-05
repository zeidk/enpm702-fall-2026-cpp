// [Appendix xiii] A Base Member in the List. A member initializer list names
// only the class's own members and its base classes.
#include <stdexcept>
#include <string>
#include <utility>

enum class AutomationLevel { level_0, level_1, level_2, level_3, level_4, level_5 };

class Vehicle {
 public:
  explicit Vehicle(double max_speed) : max_speed_{max_speed} {}

 private:
  double max_speed_;
};

class RoboTaxi : public Vehicle {
 public:
  explicit RoboTaxi(
      double max_speed, std::string operating_zone = "College Park",
      AutomationLevel automation_level = AutomationLevel::level_4)
      : max_speed_{max_speed},  // does not compile
        operating_zone_{std::move(operating_zone)},
        automation_level_{automation_level} {
    if (automation_level_ < AutomationLevel::level_0 ||
        automation_level_ > AutomationLevel::level_5) {
      throw std::invalid_argument{"unknown automation level"};
    }
  }

 private:
  std::string operating_zone_;
  AutomationLevel automation_level_;
};

int main() { RoboTaxi robotaxi{13.4}; }
