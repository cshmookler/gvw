#pragma once

// Standard includes
#include <vector>

// Local includes
#include "init.hpp"

namespace gvw {

namespace internal {

extern std::vector<window*> WINDOWS_RECIEVING_INPUT;
extern std::vector<joystick_event> JOYSTICK_EVENTS;
extern size_t STORED_JOYSTICK_EVENTS;

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

void PollEvents();
void WaitThenPollEvents();
void WaitThenPollEvents(double timeout);
void PostEmptyEvent();
void SetSwapInterval(int interval);

// Setup the joystick input buffer and set the joystick callback
void SetupJoystickInputBuffer();

// Clear the joystick input buffer
void ClearJoystickInputBuffer();

std::vector<joystick_event> GetJoystickEvents();

} // namespace gvw