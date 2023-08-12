#include <iostream>
#include <thread>
#include <mutex>
#include "../../gvw/gvw.hpp"
#include "../../utils/unit-test/unit-test.hpp"

// void GlfwGlobalThreadTest()
// {
//     auto callable = [](size_t Integer, gvw::coordinate<int> Position) {
//         gvw& gvw = gvw::Get();

//         gvw::window window = gvw.CreateWindow(
//             { .position = Position, .title = std::to_string(Integer).c_str()
//             });

//         const uint MSPF = 1;
//         uint loops = 0;
//         const uint LOOPS = 1000;

//         while (!window.ShouldClose() && loops++ < LOOPS) {
//             std::this_thread::sleep_for(std::chrono::milliseconds(MSPF));
//             gvw.PollEvents();
//         }
//     };

//     gvw& gvw = gvw::Get();
//     gvw::area<int> monitorSize = gvw.PrimaryMonitor().WorkAreaSize();

//     const size_t THREADS = 10;
//     gvw::area<int> positionIncrement = {
//         monitorSize.width / static_cast<int>(THREADS),
//         monitorSize.height / static_cast<int>(THREADS)
//     };

//     std::vector<std::thread> threads;
//     for (size_t i = 1; i <= THREADS; ++i) {
//         threads.emplace_back(
//             callable,
//             i,
//             gvw::coordinate<int>{ positionIncrement.width *
//             static_cast<int>(i),
//                                   positionIncrement.height *
//                                       static_cast<int>(i) });
//         threads.end()->detach();
//     }

//     std::this_thread::sleep_for(std::chrono::seconds(2));

//     for (auto& thread : threads) {
//         if (thread.joinable()) {
//             thread.join();
//         }
//     }
// }

int main()
{
    gvw& gvw = gvw::Get();
    gvw::window window = gvw.CreateWindow();

    auto callable = [&gvw, &window](size_t Window_ID) {
        std::stringstream testName;
        testName << "Thread " << Window_ID
                 << " Window.EnterFullScreen(...) and Window.ExitFullScreen()";
        test::ForThrow(testName.str().c_str(), [&gvw, &window]() {
            const uint MSPF = 25;
            const uint LOOPS = 20;
            uint loops = 0;

            gvw::monitor monitor = gvw.PrimaryMonitor();

            while (loops++ < LOOPS) {
                std::this_thread::sleep_for(std::chrono::milliseconds(MSPF));
                window.EnterFullScreen(monitor);
                std::this_thread::sleep_for(std::chrono::milliseconds(MSPF));
                window.ExitFullScreen();
                gvw.PollEvents();
            }
        });
    };

    size_t threadCount = 10;
    std::vector<std::thread> threads;
    for (size_t i = 1; i <= threadCount; ++i) {
        threads.emplace_back(callable, i);
        threads.back().detach();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "Window closed - GLFW uninitialized\n";

    // for (auto& thread : threads) {
    //     if (thread.joinable()) {
    //         thread.join();
    //     } else {
    //         throw std::runtime_error("Failed to join thread!");
    //     }
    // }
}