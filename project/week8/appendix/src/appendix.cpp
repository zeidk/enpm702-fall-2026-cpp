/**
 * @file appendix.cpp
 * @brief L8: the code of every appendix frame that has code, runnable.
 * @author Zeid Kootbally
 *
 * @details Build target: @c week8_appendix.
 *
 * @code
 * 702run week8_appendix        # every appendix frame
 * 702run week8_appendix iii    # only [Appendix iii]
 * @endcode
 */
#include <iostream>
#include <memory>
#include <vector>

#include "sensor.hpp"
#include "slides.hpp"

namespace {

// [Appendix iii] Returning a Large Member
namespace large_member {
class RoboTaxi {
 public:
  int add_sensor(SensorType type, double range) {
    const int id{next_id_++};
    sensors_.push_back(std::make_unique<Sensor>(id, type, range));
    return id;
  }

  [[nodiscard]] const std::vector<std::unique_ptr<Sensor>>&
  get_sensors() const noexcept {
    return sensors_;
  }

  // Returning a copy does not compile: each std::unique_ptr would be copied.
  // [[nodiscard]] std::vector<std::unique_ptr<Sensor>> get_sensors_copy() const {
  //   return sensors_;
  // }

 private:
  std::vector<std::unique_ptr<Sensor>> sensors_;
  inline static int next_id_{1};
};

void run() {
  RoboTaxi taxi_a;
  taxi_a.add_sensor(SensorType::lidar, 100.0);
  taxi_a.add_sensor(SensorType::camera, 80.0);
  for (const std::unique_ptr<Sensor>& sensor : taxi_a.get_sensors()) {
    std::cout << sensor->get_id() << ' ' << sensor->get_range() << '\n';
  }  // prints 1 100, then 2 80
}
}  // namespace large_member

}  // namespace

int main(int argc, char* argv[]) {
  return run_slides({{3, "Returning a Large Member", large_member::run}}, argc, argv,
                    Part::appendix);
}
