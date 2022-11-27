#include "utils.hpp"

// For printing to the console
#include <iostream>

namespace utils {

void PrintCommandLineArguments(int argc, char** argv)
{
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
}

} // namespace utils