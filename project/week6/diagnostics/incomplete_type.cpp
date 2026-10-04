struct RobotStatus;  // declared, not defined

double get_battery(RobotStatus r) {
  return r.battery_pct;
}

// [Slide 9] Where a Struct Goes
// DOES NOT COMPILE. To make a parameter of type RobotStatus, or to read one of
// its members, the compiler needs the whole definition, not only the name.
// Compile it by hand: 702g++ -c incomplete_type.cpp
