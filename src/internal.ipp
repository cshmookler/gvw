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

enum struct glfw_bool
{
    // NOLINTBEGIN
    eTrue = GLFW_TRUE,
    eFalse = GLFW_FALSE
    // NOLINTEND
};

template<typename Type1,
         typename Type2,
         typename Out,
         typename CallableIdentical,
         typename CallableGet>
std::vector<Out> GetCommonElements(const std::vector<Type1>& Arr_1,
                                   const std::vector<Type2>& Arr_2,
                                   CallableIdentical Identical,
                                   CallableGet Get) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2> &&
    std::is_invocable_r_v<Out, CallableGet, Type1, Type2>
{
    const size_t SIZE_1 = Arr_1.size();
    const size_t SIZE_2 = Arr_2.size();
    const size_t MAX_COMMON_ELEMENTS = (SIZE_1 > SIZE_2) ? SIZE_1 : SIZE_2;
    std::vector<Out> commonElements;
    commonElements.reserve(MAX_COMMON_ELEMENTS);
    for (const auto& item1 : Arr_1) {
        for (const auto& item2 : Arr_2) {
            if (Identical(item1, item2)) {
                commonElements.emplace_back(Get(item1, item2));
                break;
            }
        }
    }
    commonElements.shrink_to_fit();
    return commonElements;
}

template<typename Type1, typename Type2, typename CallableIdentical>
std::vector<Type1> GetCommonElementsInArr1(const std::vector<Type1>& Arr_1,
                                           const std::vector<Type2>& Arr_2,
                                           CallableIdentical Identical) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2>
{
    std::vector<Type1> commonElementsInArr1;
    commonElementsInArr1.reserve(Arr_1.size());
    for (const auto& item1 : Arr_1) {
        for (const auto& item2 : Arr_2) {
            if (Identical(item1, item2)) {
                commonElementsInArr1.emplace_back(item1);
                break;
            }
        }
    }
    commonElementsInArr1.shrink_to_fit();
    return commonElementsInArr1;
}

template<typename Type1, typename Type2, typename CallableIdentical>
std::vector<Type1> GetUncommonElementsInArr1(
    const std::vector<Type1>& Arr_1,
    const std::vector<Type2>& Arr_2,
    CallableIdentical Identical) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2>
{
    std::vector<Type1> uncommonElementsInArr1;
    uncommonElementsInArr1.reserve(Arr_1.size());
    for (const auto& item1 : Arr_1) {
        bool matchNotFound = true;
        for (const auto& item2 : Arr_2) {
            if (Identical(item1, item2)) {
                matchNotFound = false;
                break;
            }
        }
        if (matchNotFound) {
            uncommonElementsInArr1.emplace_back(item1);
        }
    }
    uncommonElementsInArr1.shrink_to_fit();
    return uncommonElementsInArr1;
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
    switch (Message_Type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            message += "Vulkan: General: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            message += "Vulkan: Validation: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            message += "Vulkan: Performance: ";
            break;
        default:
            message += "Vulkan: Unknown: ";
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
void VerboseCallbackTemplate(const char* Message)
{
    std::string verboseMessage =
        std::string(ansiec::BOLD) + "GVW verbose: " + Message + ansiec::RESET;
    PrintFunction(verboseMessage.c_str());
}

template<callback_print_function PrintFunction>
void InfoCallbackTemplate(const char* Message)
{
    std::string infoMessage = std::string(ansiec::BOLD) + ansiec::CYAN_FG +
                              "GVW info: " + Message + ansiec::RESET;
    PrintFunction(infoMessage.c_str());
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

template<callback_print_function PrintFunction>
void GlfwErrorCallbackTemplate(int Error_Code, const char* Message)
{
    std::string errorMessage = std::string(ansiec::BOLD) + ansiec::RED_FG +
                               "GLFW error [" + std::to_string(Error_Code) +
                               "]: " + Message + ansiec::RESET;
    PrintFunction(errorMessage.c_str());
}

enum struct window_input_mode
{
    // NOLINTBEGIN

    eCursor = 0x00033001,            // GLFW_CURSOR
    eStickyKeys = 0x00033002,        // GLFW_STICKY_KEYS
    eStickyMouseButtons = 0x00033003 // GLFW_STICKY_MOUSE_BUTTONS

    // NOLINTEND
};

enum struct window_input_mode_cursor
{
    // NOLINTBEGIN

    eNormal = 0x00034001,  // GLFW_CURSOR_NORMAL
    eHidden = 0x00034002,  // GLFW_CURSOR_HIDDEN
    eDisabled = 0x00034003 // GLFW_CURSOR_DISABLED

    // NOLINTEND
};

} // namespace gvw::internal