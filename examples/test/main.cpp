// Standard includes
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/mat4x4.hpp>
// #include <glm/vec4.hpp>

// Local includes
#include "../../gvw/gvw.hpp"
#include "utils.hpp"

#ifdef _DEBUG
void ErrorTestCallback(int errorCode, const char* description)
{
    if (errorCode != GLFW_NO_ERROR) {
        std::cout << "GLFW error: " << description << std::endl;
    }
}
#endif

void CursorEnterCallback(GLFWwindow* window, int entered)
{
    std::cout << "window handle: " << window << ", entered state: " << entered
              << std::endl;
}

int main(int argc, char** argv)
{
#ifdef _DEBUG
    std::cout << "Running in Debug mode\n" << std::endl;
    std::cout << "Command line arguments:\n";
    utils::PrintCommandLineArguments(argc, argv, '\t');
#else
    std::cout << "Running in Release mode\n" << std::endl;
#endif

    // Initialize GLFW
#ifdef _DEBUG
    int returnCode = gvw::Init(ErrorTestCallback);
    std::cout << "gvw::Init() returned with: " << returnCode << std::endl;
#else
    gvw::Init();
#endif

    // Create the window
    gvw::window window;
    window.Create(600, 450, "This is the window title", "", nullptr, nullptr);

    gvw::monitor primaryMonitor = gvw::PrimaryMonitor();
    std::vector<gvw::monitor> monitors = gvw::Monitors();
    std::cout << "monitor count: " << monitors.size() << std::endl;
    std::cout << size_t(primaryMonitor.Id()) << std::endl;
    std::cout << "monitor name: " << primaryMonitor.Name() << std::endl;

    window.AlwaysOnTop();

    std::string iconPath;
    while (!window.ShouldClose()) {
        std::cout << "Set icon path: ";
        std::cin >> iconPath;
        window.SetIcon(iconPath.c_str());
        gvw::PollEvents();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    window.CancelClose();

    window.SetupCursorEnterInputBuffer();

    while (!window.ShouldClose()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        gvw::PollEvents();
        std::vector<gvw::cursor_enter_event> cursorEnterEvents =
            window.GetCursorEnterEvents();
        for (auto& element : cursorEnterEvents) {
            std::cout << "Cursor enter event: " << element << std::endl;
        }

        window.ClearInputBuffers();
    }

    window.CancelClose();

    std::vector<const char*> iconPaths(2);
    iconPaths.at(0) = "icon.png";
    iconPaths.at(1) = "icon_no_transparency.png";
    window.SetCandidateIcons(iconPaths);

    window.Undecorate();
    window.NotAlwaysOnTop();
    std::cout << "Undecorated and not always on top" << std::endl;
    gvw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Decorate();
    std::cout << "Decorated" << std::endl;
    gvw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Undecorate();
    std::cout << "Undecorated" << std::endl;
    gvw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> now;
    std::chrono::milliseconds duration;

    start = std::chrono::steady_clock::now();

    // Window main loop
    while (!window.ShouldClose()) {
        now = std::chrono::steady_clock::now();
        duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        if (duration.count() > 3000) {
            window.Close();
        }

        gvw::PollEvents();
    }

    window.FullScreen(primaryMonitor);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.ExitFullScreen();
    window.Restore();
    window.Focus();
    window.Undecorate();
    gvw::PollEvents();
    std::cout << "Undecorated and focused" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Decorate();
    gvw::PollEvents();
    std::cout << "Decorated" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Focus();
    std::cout << "Focused" << std::endl;
    gvw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto pixels = window.GetSize();
    auto screenCoordinates = window.GetSizeInScreenCoordinates();
    auto contentScale = window.GetContentScale();

    std::cout << "pixels width: " << pixels.width
              << ", pixels height: " << pixels.height << std::endl;
    std::cout << "sc width: " << screenCoordinates.width
              << ", sc height: " << screenCoordinates.height << std::endl;
    std::cout << "content scale x: " << contentScale.x
              << ", content scale y: " << contentScale.y << std::endl;

    gvw::coordinate<int> screenCoordinate;
    screenCoordinate.x = screenCoordinates.width;
    screenCoordinate.y = screenCoordinates.height;

    screenCoordinate = window.ScreenCoordinateToPixel(screenCoordinate);

    std::cout << "pixels width: " << screenCoordinate.x
              << ", pixels height: " << screenCoordinate.y << std::endl;

    window.Destroy();

    window.SetOpacity(0.5);

    // These function should fail because the window doesn't exist anymore
    window.FullScreen(primaryMonitor);
    window.Close();
    window.ExitFullScreen();
    window.Id();
    window.SetPosition(0, 1);
    gvw::window thing;
    thing.Create(39,
                 2903,
                 "THIDHFSDIUFHDSFJHUDFHSFJDHFJKDSFHDJKSFHDKJFHSDJFHSDKFH",
                 "",
                 nullptr,
                 window.Id());
    gvw::size<int> windowSize = window.GetSize();
    std::cout << "window.GetSize(): x=" << windowSize.width
              << ", y=" << windowSize.height << std::endl;

    window.Destroy();

    gvw::Destroy();
}