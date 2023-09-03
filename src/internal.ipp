#pragma once

// Standard includes
#include <list>

// Local includes
#include "gvw.hpp"
#include "../utils/ansiec/ansiec.hpp"

namespace gvw::internal {

template<typename T>
struct public_constructor : public T
{
    template<typename... Args>
    public_constructor(Args&&... Arguments)
        : T(std::forward<Args>(Arguments)...)
    {
    }
};

struct uncopyable_unmovable // NOLINT
{
    uncopyable_unmovable() = default;
    uncopyable_unmovable(const uncopyable_unmovable&) = delete;
    uncopyable_unmovable(uncopyable_unmovable&&) noexcept = delete;
    uncopyable_unmovable& operator=(const uncopyable_unmovable&) = delete;
    uncopyable_unmovable& operator=(uncopyable_unmovable&&) noexcept = delete;
};

template<typename... Args>
class terminator
{
  public:
    using deleter_signature = void (*)(Args...);

  private:
    deleter_signature deleter;
    std::tuple<Args...> args;

  public:
    terminator(deleter_signature Deleter, Args... Arguments)
        : deleter(Deleter)
        , args(Arguments...)
    {
    }
    terminator(const terminator&) = default;
    terminator(terminator&&) noexcept = default;
    terminator& operator=(const terminator&) = default;
    terminator& operator=(terminator&&) noexcept = default;
    ~terminator() { std::apply(this->deleter, this->args); }
};

template<typename UserT, typename VulkanT>
std::vector<UserT> UserItemsMissingInVulkanArray(
    const std::vector<UserT>& User_Array,
    const std::vector<VulkanT>& Vulkan_Array,
    bool (*Is_Identical)(UserT, VulkanT))
{
    std::vector<UserT> userStringsMissing = {};
    std::list<VulkanT> vulkanArrayCopy(Vulkan_Array.begin(),
                                       Vulkan_Array.end());
    for (UserT userItem : User_Array) {
        bool matchFound = false;
        for (auto vulkanIt = vulkanArrayCopy.begin();
             vulkanIt != vulkanArrayCopy.end();
             ++vulkanIt) {
            if (Is_Identical(userItem, *vulkanIt)) {
                vulkanIt = vulkanArrayCopy.erase(vulkanIt);
                matchFound = true;
                break;
            }
        }
        if (!matchFound) {
            userStringsMissing.emplace_back(userItem);
        }
    }
    return userStringsMissing;
}

template<typename UserT, typename VulkanT>
std::vector<UserT> UserItemsFoundInVulkanArray(
    const std::vector<UserT>& User_Array,
    const std::vector<VulkanT>& Vulkan_Array,
    bool (*Is_Identical)(UserT, VulkanT))
{
    std::vector<UserT> userStringsFound = {};
    std::list<VulkanT> vulkanArrayCopy(Vulkan_Array.begin(),
                                       Vulkan_Array.end());
    for (UserT userItem : User_Array) {
        bool matchFound = false;
        for (auto vulkanIt = vulkanArrayCopy.begin();
             vulkanIt != vulkanArrayCopy.end();
             ++vulkanIt) {
            if (Is_Identical(userItem, *vulkanIt)) {
                vulkanIt = vulkanArrayCopy.erase(vulkanIt);
                matchFound = true;
                break;
            }
        }
        if (matchFound) {
            userStringsFound.emplace_back(userItem);
        }
    }
    return userStringsFound;
}

template<typename T>
struct glfw_hint
{
    const int HINT;
    T value;

    glfw_hint(int Hint, T Value)
        : HINT(Hint)
        , value(Value)
    {
    }
    glfw_hint(const glfw_hint<T>& GLFW_Hint)
        : HINT(GLFW_Hint.HINT)
        , value(GLFW_Hint.value)
    {
    }
    glfw_hint(glfw_hint<T>&& GLFW_Hint) noexcept
        : HINT(std::move(GLFW_Hint.HINT))
        , value(std::move(GLFW_Hint.value))
    {
    }
    glfw_hint<T>& operator=(const glfw_hint<T>&) = delete;
    glfw_hint<T>& operator=(glfw_hint<T>&&) = delete;
    glfw_hint<T>& operator=(T Value)
    {
        this->value = Value;
        return *this;
    }
    ~glfw_hint() = default;
};

/// @todo Change how hints are stored to preserve custom types when creating
/// glfw_hints from hint creation structs.
template<int_hint_function IntHintFunc,
         string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
struct glfw_hints
{
    std::array<glfw_hint<int>, IntHints> intHints;
    std::array<glfw_hint<const char*>, StringHints> stringHints;

    glfw_hints(const glfw_hints& GLFW_Hints)
        : intHints(GLFW_Hints.intHints)
        , stringHints(GLFW_Hints.stringHints)
    {
    }
    glfw_hints(std::array<glfw_hint<int>, IntHints>&& Int_Hints,
               std::array<glfw_hint<const char*>, StringHints>&& String_Hints)
        : intHints(std::move(Int_Hints))
        , stringHints(std::move(String_Hints))
    {
    }
    glfw_hints(glfw_hints&& GLFW_Hints) noexcept = delete;
    ~glfw_hints() = default;
    glfw_hints& operator=(const glfw_hints& GLFW_Hints)
    {
        this->intHints = GLFW_Hints.intHints;
        this->stringHints = GLFW_Hints.stringHints;
        return *this;
    }
    glfw_hints& operator=(glfw_hints&& GLFW_Hints) noexcept
    {
        this->intHints = std::move(GLFW_Hints.intHints);
        this->stringHints = std::move(GLFW_Hints.stringHints);
        return *this;
    }

    /// @brief Apply int and string hints.
    /// @warning This function is NOT thread safe.
    void Apply() const
    {
        for (const glfw_hint<int>& intHint : this->intHints) {
            IntHintFunc(intHint.HINT, intHint.value);
        }
        for (const glfw_hint<const char*>& stringHint : this->stringHints) {
            StringHintFunc(stringHint.HINT, stringHint.value);
        }
    }
};

template<instance_debug_utils_messenger_callback_print_function PrintFunction>
VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallbackTemplate(
    VkDebugUtilsMessageSeverityFlagBitsEXT Message_Severity,
    VkDebugUtilsMessageTypeFlagsEXT Message_Type,
    const VkDebugUtilsMessengerCallbackDataEXT* P_Callback_Data,
    void* P_User_Data)
{
    std::string message;

    switch (Message_Severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            message += "[Verbose - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            message += std::string(ansiec::BOLD) + ansiec::CYAN_FG + "[Info - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            message +=
                std::string(ansiec::BOLD) + ansiec::YELLOW_FG + "[Warning - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            message += std::string(ansiec::BOLD) + ansiec::RED_FG + "[Error - ";
            break;
        default:
            message +=
                std::string(ansiec::BOLD) + ansiec::MAGENTA_FG + "[Unknown - ";
            break;
    }

    switch (Message_Type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            message += "General]: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            message += "Validation]: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            message += "Performance]: ";
            break;
        default:
            message += "Unknown]: ";
            break;
    }

    message += std::string(P_Callback_Data->pMessage) + ansiec::RESET;

    if (P_User_Data != nullptr) {
        message += " - Note: User data for this message was NOT "
                   "null but was ignored.";
    }

    PrintFunction(Message_Severity, message.c_str());

    return VK_FALSE;
}

template<callback_print_function PrintFunction>
void GlfwErrorCallbackTemplate(int Error_Code, const char* Message)
{
    std::string errorMessage = std::string(ansiec::BOLD) + ansiec::RED_FG +
                               "GLFW error [" + std::to_string(Error_Code) +
                               "]: " + Message + ansiec::RESET;
    PrintFunction(errorMessage.c_str());
}

template<callback_print_function PrintFunction>
void WarningCallbackTemplate(const char* Message)
{
    std::string warningMessage = std::string(ansiec::BOLD) + ansiec::YELLOW_FG +
                                 "GVW warning: " + Message + ansiec::RESET;
    PrintFunction(warningMessage.c_str());
}

template<callback_print_function PrintFunction>
void ErrorCallbackTemplate(const char* Message)
{
    std::string errorMessage = std::string(ansiec::BOLD) + ansiec::RED_FG +
                               "GVW error: " + Message + ansiec::RESET;
    PrintFunction(errorMessage.c_str());
}

} // namespace gvw::internal