/**
 * @file dispatcher.cpp
 * @brief Definitions for dispatcher.hpp.
 * @author Zeid Kootbally
 */
#include "dispatcher.hpp"

#include <iostream>

CommandTable make_command_table() {
    CommandTable table{};
    table["dock"] = [](int id) { std::cout << "robot " << id << ": go to dock\n"; };
    table["pause"] = [](int id) { std::cout << "robot " << id << ": paused\n"; };
    table["resume"] = [](int id) { std::cout << "robot " << id << ": resumed\n"; };
    return table;
}

bool dispatch(const CommandTable& table, const std::string& command, int robot_id) {
    auto handler{table.find(command)};
    if (handler == table.end() || !handler->second) {
        return false;
    }
    handler->second(robot_id);
    return true;
}

void log_message(std::string_view text, std::source_location at) {
    // file_name() is the path the compiler was given, which CMake makes
    // absolute. Keep the part after the last '/'.
    std::string_view file{at.file_name()};
    file.remove_prefix(file.rfind('/') + 1);
    std::cout << file << ':' << at.line() << ' ' << at.function_name() << ": " << text << '\n';
}
