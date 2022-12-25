#include "utils.hpp"

// For printing to the console
#include <iostream>

namespace utils {

void PrintCommandLineArguments(int argc, char** argv, char prefix)
{
    for (int i = 0; i < argc; i++) {
        std::cout << prefix << argv[i] << std::endl;
    }
}

} // namespace utils