#include "input.tpp"

// Local includes
#include "types.tpp"
#include "window.hpp"

namespace glfw {

std::vector<window*> WINDOWS_RECIEVING_INPUT;
std::vector<joystick_event> JOYSTICK_EVENTS;
size_t STORED_JOYSTICK_EVENTS = 0;

void PollEvents()
{
    glfwPollEvents();
}

void WaitThenPollEvents()
{
    glfwWaitEvents();
}

void WaitThenPollEvents(double timeout)
{
    glfwWaitEventsTimeout(timeout);
}

void PostEmptyEvent()
{
    glfwPostEmptyEvent();
}

void SetSwapInterval(int interval)
{
    glfwSwapInterval(interval);
}

int FindWindowInputIndex(GLFWwindow* associatedWindow,
                         std::vector<window*>& windowsRecievingInput,
                         size_t& index)
{
    for (index = 0; index < windowsRecievingInput.size(); index++) {
        if (windowsRecievingInput.at(index)->Id() == associatedWindow) {
            return GLFW_TRUE;
        }
    }
    return GLFW_FALSE;
}

void KeyCallback(GLFWwindow* associatedWindow,
                 int key,
                 int scancode,
                 int action,
                 int mods)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    key_event keyEvent = { key, scancode, action, mods };
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedKeyEvents >= windowReference.keyEvents.size()) {
        windowReference.keyEvents.resize(windowReference.keyEvents.size() +
                                         INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.keyEvents.at(windowReference.storedKeyEvents) = keyEvent;
}

void CharacterCallback(GLFWwindow* associatedWindow, unsigned int codePoint)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedCharacterEvents >=
        windowReference.characterEvents.size()) {
        windowReference.characterEvents.resize(
            windowReference.characterEvents.size() +
            INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.characterEvents.at(windowReference.storedCharacterEvents) =
        character_event(codePoint);
}

void CursorPositionCallback(GLFWwindow* associatedWindow,
                            double xPosition,
                            double yPosition)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    cursor_position_event cursorPosition = { xPosition, yPosition };
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedCursorPositionEvents >=
        windowReference.cursorPositionEvents.size()) {
        windowReference.cursorPositionEvents.resize(
            windowReference.cursorPositionEvents.size() +
            INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.cursorPositionEvents.at(
        windowReference.storedCursorPositionEvents) = cursorPosition;
}

void CursorEnterCallback(GLFWwindow* associatedWindow, int entered)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedCursorEnterEvents >=
        windowReference.cursorEnterEvents.size()) {
        windowReference.cursorEnterEvents.resize(
            windowReference.cursorEnterEvents.size() +
            INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.cursorEnterEvents.at(
        windowReference.storedCursorEnterEvents) = cursor_enter_event(entered);
}

void MouseButtonCallback(GLFWwindow* associatedWindow,
                         int button,
                         int action,
                         int mods)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }

    mouse_button_event mouseButtonEvent = { button, action, mods };
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedMouseButtonEvents >=
        windowReference.mouseButtonEvents.size()) {
        windowReference.mouseButtonEvents.resize(
            windowReference.mouseButtonEvents.size() +
            INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.mouseButtonEvents.at(
        windowReference.storedMouseButtonEvents) = mouseButtonEvent;
}

void ScrollCallback(GLFWwindow* associatedWindow,
                    double xOffset,
                    double yOffset)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    scroll_event scrollEvent = { xOffset, yOffset };
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedScrollEvents >=
        windowReference.scrollEvents.size()) {
        windowReference.scrollEvents.resize(
            windowReference.scrollEvents.size() + INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.scrollEvents.at(windowReference.storedScrollEvents) =
        scrollEvent;
}

void JoystickCallback(int jid, int event)
{
    // TODO
}

void FileDropCallback(GLFWwindow* associatedWindow,
                      int count,
                      const char** paths)
{
    size_t windowIndex;
    if (FindWindowInputIndex(associatedWindow,
                             WINDOWS_RECIEVING_INPUT,
                             windowIndex) == GLFW_FALSE) {
        return;
    }
    file_drop_event fileDrop = { count, paths };
    window& windowReference = *WINDOWS_RECIEVING_INPUT.at(windowIndex);
    if (windowReference.storedFileDropEvents >=
        windowReference.fileDropEvents.size()) {
        windowReference.fileDropEvents.resize(
            windowReference.fileDropEvents.size() +
            INPUT_BUFFER_INCREMENT_SIZE);
    }
    windowReference.fileDropEvents.at(windowReference.storedFileDropEvents) =
        fileDrop;
}

void SetupJoystickInputBuffer()
{
    JOYSTICK_EVENTS.resize(INPUT_BUFFER_INITIAL_SIZE);
    glfwSetJoystickCallback(JoystickCallback);
}

void ClearJoystickInputBuffer()
{
    JOYSTICK_EVENTS.resize(INPUT_BUFFER_INITIAL_SIZE);
    STORED_JOYSTICK_EVENTS = 0;
}

std::vector<joystick_event> GetJoystickEvents()
{
    std::vector<joystick_event> truncatedJoystickEvents = JOYSTICK_EVENTS;
    truncatedJoystickEvents.resize(STORED_JOYSTICK_EVENTS);
    return truncatedJoystickEvents;
}

} // namespace glfw