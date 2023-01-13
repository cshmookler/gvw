// Standard includes
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

// External includes
// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>
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
    int returnCode = glfw::Init(ErrorTestCallback);
    std::cout << "glfw::Init() returned with: " << returnCode << std::endl;
#else
    glfw::Init();
#endif

    // Create the window
    glfw::window window;
    window.Create(600, 450, "This is the window title");

    glfw::monitor primaryMonitor = glfw::PrimaryMonitor();
    std::vector<glfw::monitor> monitors = glfw::Monitors();
    std::cout << "monitor count: " << monitors.size() << std::endl;
    primaryMonitor.AssertInitialization();
    std::cout << "monitor name: " << primaryMonitor.Name() << std::endl;

    window.AlwaysOnTop();

    std::string iconPath;
    while (!window.ShouldClose()) {
        std::cout << "Set icon path: ";
        std::cin >> iconPath;
        window.SetIcon(iconPath.c_str());
        glfw::PollEvents();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    window.CancelClose();

    window.SetupCursorEnterInputBuffer();

    while (!window.ShouldClose()) {
        glfw::PollEvents();

        std::vector<glfw::cursor_enter_event> cursorEnterEvents =
            window.GetCursorEnterEvents();
        for (auto& element : cursorEnterEvents) {
            std::cout << "Cursor enter event: " << element << std::endl;
        }

        window.ClearInputBuffers();
    }

    glfwSetCursorEnterCallback(window.Id(), CursorEnterCallback);

    std::string dummyVariable;
    while (!window.ShouldClose()) {
        std::cout << "Enter anything to print cursor enter information: "
                  << std::endl;
        std::cin >> dummyVariable;

        glfw::PollEvents();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::vector<const char*> iconPaths;
    iconPaths.push_back("../../examples/icon.png");
    window.SetIcon(iconPaths);

    window.Undecorate();
    window.NotAlwaysOnTop();
    std::cout << "Undecorated and not always on top" << std::endl;
    glfw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Decorate();
    std::cout << "Decorated" << std::endl;
    glfw::PollEvents();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Undecorate();
    std::cout << "Undecorated" << std::endl;
    glfw::PollEvents();
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

        glfw::PollEvents();
    }

    window.FullScreen(primaryMonitor);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.ExitFullScreen();
    window.Restore();
    window.Focus();
    window.Undecorate();
    glfw::PollEvents();
    std::cout << "Undecorated and focused" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Decorate();
    glfw::PollEvents();
    std::cout << "Decorated" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.Focus();
    std::cout << "Focused" << std::endl;
    glfw::PollEvents();
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

    glfw::coordinate<int> screenCoordinate;
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
    window.AssertCreation();
    window.ExitFullScreen();
    window.Id();
    window.SetPosition(0, 1);
    glfw::window thing;
    thing.Create(39,
                 2903,
                 "THIDHFSDIUFHDSFJHUDFHSFJDHFJKDSFHDJKSFHDKJFHSDJFHSDKFH",
                 window);
    glfw::size<int> windowSize = window.GetSize();
    std::cout << "window.GetSize(): x=" << windowSize.width
              << ", y=" << windowSize.height << std::endl;

    window.Destroy();

    glfw::Destroy();
}