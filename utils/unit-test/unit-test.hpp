#pragma once

// Standard includes
#include <iostream>
#include <exception>
#include <mutex>

// Local includes
#include "../ansiec/ansiec.hpp"

namespace test {

namespace internal {

// Global variables
std::mutex CONSOLE_MUTEX; // NOLINT

} // namespace internal

void SynchronizedPrintToConsole(const std::string& Message)
{
    std::scoped_lock lock(internal::CONSOLE_MUTEX);
    std::cout << Message;
}

/// @warning Only std::runtime_error exceptions are caught.
template<typename Func, typename... Args>
bool ForThrow(const char* Name, Func&& Function, Args&&... Arguments)
{
    std::string message = std::string(ansiec::BOLD) + ansiec::CYAN_FG +
                          "[Unit Test]: " + ansiec::YELLOW_FG + "\"" + Name +
                          "\"" + ansiec::RESET + " -> ";

    try {
        Function(std::forward<Args>(Arguments)...);
    } catch (const std::runtime_error& e) {

        message += std::string(ansiec::BOLD) + ansiec::RED_FG + "FAILED" +
                   ansiec::RESET + " -> " + e.what() + "\n";
        SynchronizedPrintToConsole(message);

        return false;
    }

    message += std::string(ansiec::BOLD) + ansiec::GREEN_FG + "PASSED\n" +
               ansiec::RESET;
    SynchronizedPrintToConsole(message);

    return true;
}

} // namespace test