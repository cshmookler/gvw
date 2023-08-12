#pragma once

// Standard includes
#include <iostream>
#include <exception>
#include <mutex>
#include <sstream>

// External includes
#include <boost/exception/diagnostic_information.hpp>

// Local includes
#include "../ansiec/ansiec.hpp"

namespace test {

namespace {

std::mutex OUT_MUTEX; // NOLINT

} // anonymous namespace

template<typename Func, typename... Args>
bool ForThrow(const char* Name, Func&& Function, Args&&... Arguments)
{
    std::stringstream log;
    log << ansiec::BOLD << ansiec::CYAN_FG
        << "[Unit Test]: " << ansiec::YELLOW_FG << "\"" << Name << "\""
        << ansiec::RESET << " -> ";
    try {
        Function(std::forward<Args>(Arguments)...);
    } catch (...) {
        log << ansiec::BOLD << ansiec::RED_FG << "FAILED" << ansiec::RESET
            << " -> " << boost::current_exception_diagnostic_information()
            << "\n";
        OUT_MUTEX.lock();
        std::cout << log.str();
        OUT_MUTEX.unlock();
        return false;
    }
    log << ansiec::BOLD << ansiec::GREEN_FG << "PASSED\n" << ansiec::RESET;
    OUT_MUTEX.lock();
    std::cout << log.str();
    OUT_MUTEX.unlock();
    return true;
}

} // namespace test