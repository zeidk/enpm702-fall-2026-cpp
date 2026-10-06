#pragma once
/**
 * @file dispatcher.hpp
 * @brief Sends named commands to robots, and logs what happened.
 * @author Zeid Kootbally
 */
#include <functional>
#include <map>
#include <source_location>
#include <string>
#include <string_view>

/// Command name to handler. A handler takes the id of the robot to command.
using CommandTable = std::map<std::string, std::function<void(int)>>;

/**
 * @brief The commands every robot understands: "dock", "pause", "resume".
 * @return A table whose handlers print what the robot is told to do.
 */
CommandTable make_command_table();

/**
 * @brief Runs one command for one robot.
 *
 * Looks the command up with @c find, so an unknown name is never inserted
 * into the table, and an empty handler is never called.
 *
 * @param table The commands.
 * @param command The command name.
 * @param robot_id The robot to command.
 * @return true if the command was found and run, false otherwise.
 */
bool dispatch(const CommandTable& table, const std::string& command, int robot_id);

/**
 * @brief Prints a message with the file, line and function it came from.
 * @param text The message.
 * @param at Where the call was written. The default argument is evaluated at
 *           the call, so it records the caller's line.
 */
void log_message(std::string_view text,
                 std::source_location at = std::source_location::current());
