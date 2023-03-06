#pragma once

/**
 * @file input.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Functions and global variables for handling input.
 * @date 2023-03-03
 */

// Standard includes
#include <vector>

// Local includes
#include "init.hpp"

namespace gvw {

namespace global {

extern std::vector<window*> WINDOWS_RECIEVING_INPUT;
extern std::vector<joystick_event> JOYSTICK_EVENTS;
extern size_t STORED_JOYSTICK_EVENTS;

} // namespace global

namespace internal {

int FindWindowInputIndex(GLFWwindow* associatedWindow,
                         std::vector<window*>& windowsRecievingInput,
                         size_t& index);

void KeyCallback(GLFWwindow* window,
                 int key,
                 int scancode,
                 int action,
                 int mods);
void CharacterCallback(GLFWwindow* associatedWindow, unsigned int codePoint);
void CursorPositionCallback(GLFWwindow* associatedWindow,
                            double xPosition,
                            double yPosition);
void CursorEnterCallback(GLFWwindow* associatedWindow, int entered);
void MouseButtonCallback(GLFWwindow* associatedWindow,
                         int button,
                         int action,
                         int mods);
void ScrollCallback(GLFWwindow* associatedWindow,
                    double xOffset,
                    double yOffset);
void JoystickCallback(int jid, int event);
void FileDropCallback(GLFWwindow* associatedWindow,
                      int count,
                      const char** paths);

} // namespace internal

/// @brief Polls events for all windows.
void PollEvents();

/// @brief Waits until a window event is received, then polls events for all
/// windows.
void WaitThenPollEvents();

/// @brief Waits until either the timout expires or a window event is received,
/// then polls events for all windows.
/// @param timeout The timeout in milliseconds.
void WaitThenPollEvents(double timeout);

/// @brief Posts an empty event. Causes `WaitThenPollEvents` to poll events.
void PostEmptyEvent();

/// @brief Setup the joystick input buffer and set the joystick event callback.
void SetupJoystickInputBuffer();

/// @brief Clear the joystick input buffer.
void ClearJoystickInputBuffer();

/// @brief Returns polled joystick events.
/// @return A vector of joystick_event.
std::vector<joystick_event> GetJoystickEvents();

} // namespace gvw