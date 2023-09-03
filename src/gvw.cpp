// Local includes
#include "gvw.hpp"

namespace gvw {

creation_hint_bool::creation_hint_bool(bool Boolean)
    : boolean(Boolean)
{
}

creation_hint_bool& creation_hint_bool::operator=(bool Boolean)
{
    this->boolean = Boolean;
    return *this;
}

int creation_hint_bool::ToInt() const
{
    return this->boolean ? GLFW_TRUE : GLFW_FALSE;
}

bool creation_hint_bool::Get() const
{
    return this->boolean;
}

creation_hint_int::creation_hint_int(int Integer)
{
    this->AssertWithinBounds(Integer);
}

creation_hint_int& creation_hint_int::operator=(int Integer)
{
    this->AssertWithinBounds(Integer);
    return *this;
}

void creation_hint_int::AssertWithinBounds(int Integer)
{
    // Integer can't be larger than INT_MAX, so don't bother checking.
    if ((Integer < 0) && (Integer != GLFW_DONT_CARE)) {
        this->integer = GLFW_DONT_CARE;
        WarningCallback("Creation hint out of range. Reset to GLFW_DONT_CARE.");
    } else {
        this->integer = Integer;
    }
}

int creation_hint_int::Get() const
{
    return this->integer;
}

version GetGlfwRuntimeVersion() noexcept
{
    std::scoped_lock<std::mutex> lock(internal::global::GLFW_MUTEX);
    version glfwVersion = { .major = 0, .minor = 0, .revision = 0 };
    glfwGetVersion(
        &glfwVersion.major, &glfwVersion.minor, &glfwVersion.revision);
    return glfwVersion;
}

version GetGlfwCompiletimeVersion() noexcept
{
    return version{ .major = GLFW_VERSION_MAJOR,
                    .minor = GLFW_VERSION_MINOR,
                    .revision = GLFW_VERSION_REVISION };
}

instance_creation_hints::instance_creation_hints(
    const instance_creation_hints_info& Creation_Hints_Info)
    : glfw_hints({ { { GLFW_JOYSTICK_HAT_BUTTONS,
                       Creation_Hints_Info.joystickHatButtons.ToInt() },
                     { GLFW_COCOA_CHDIR_RESOURCES,
                       Creation_Hints_Info.cocoaChdirResources.ToInt() },
                     { GLFW_COCOA_MENUBAR,
                       Creation_Hints_Info.cocoaMenubar.ToInt() } } },
                 {})
{
}

void SetGlfwCallback(instance_glfw_error_callback GLFW_Error_Callback)
{
    std::scoped_lock<std::mutex> lock(internal::global::GLFW_MUTEX);
    internal::global::GLFW_ERROR_CALLBACK = GLFW_Error_Callback;
    glfwSetErrorCallback(internal::global::GLFW_ERROR_CALLBACK);
}

void SetWarningCallback(instance_warning_callback Warning_Callback)
{
    std::scoped_lock<std::mutex> lock(internal::global::WARNING_CALLBACK_MUTEX);
    internal::global::WARNING_CALLBACK = Warning_Callback;
}

void SetErrorCallback(instance_error_callback Error_Callback)
{
    std::scoped_lock<std::mutex> lock(internal::global::ERROR_CALLBACK_MUTEX);
    internal::global::ERROR_CALLBACK = Error_Callback;
}

void GlfwErrorCallback(int Error_Code, const char* Message)
{
    internal::global::GLFW_ERROR_CALLBACK(Error_Code, Message);
}

void WarningCallback(const char* Message)
{
    internal::global::WARNING_CALLBACK(Message);
}

void ErrorCallback(const char* Message)
{
    internal::global::ERROR_CALLBACK(Message);
}

instance_ptr CreateInstance()
{
    return CreateInstance(instance_info_config::DEFAULT);
}

instance_ptr CreateInstance(const instance_info& Instance_Info)
{
    if (internal::global::GVW_INSTANCE == nullptr) {
        internal::global::GVW_INSTANCE =
            std::make_shared<internal::instance_public_constructor>(
                Instance_Info);
    }
    return internal::global::GVW_INSTANCE;
}

instance_ptr GetInstance()
{
    return internal::global::GVW_INSTANCE;
}

void DestroyInstance()
{
    if (internal::global::GVW_INSTANCE.unique()) {
        internal::global::GVW_INSTANCE.reset();
        return;
    }

    auto gvwInstancePtrCount = internal::global::GVW_INSTANCE.use_count() - 1;
    std::string error =
        "Failed to destroy GVW. " + std::to_string(gvwInstancePtrCount);
    if (gvwInstancePtrCount > 1) {
        error += " objects are still using GVW.";
    } else {
        error += " object is still using GVW.";
    }
    ErrorCallback(error.c_str());
}

window_creation_hints::window_creation_hints(
    const window_creation_hints_info& Creation_Hints_Info)
    : glfw_hints(
          { { { GLFW_RESIZABLE, Creation_Hints_Info.resizable.ToInt() },
              { GLFW_VISIBLE, Creation_Hints_Info.visible.ToInt() },
              { GLFW_DECORATED, Creation_Hints_Info.decorated.ToInt() },
              { GLFW_FOCUSED, Creation_Hints_Info.focused.ToInt() },
              { GLFW_AUTO_ICONIFY, Creation_Hints_Info.autoIconify.ToInt() },
              { GLFW_FLOATING, Creation_Hints_Info.floating.ToInt() },
              { GLFW_MAXIMIZED, Creation_Hints_Info.maximized.ToInt() },
              { GLFW_CENTER_CURSOR, Creation_Hints_Info.centerCursor.ToInt() },
              { GLFW_TRANSPARENT_FRAMEBUFFER,
                Creation_Hints_Info.transparentFramebuffer.ToInt() },
              { GLFW_FOCUS_ON_SHOW, Creation_Hints_Info.focusOnShow.ToInt() },
              { GLFW_SCALE_TO_MONITOR,
                Creation_Hints_Info.scaleToMonitor.ToInt() },
              { GLFW_RED_BITS, Creation_Hints_Info.redBits.Get() },
              { GLFW_GREEN_BITS, Creation_Hints_Info.greenBits.Get() },
              { GLFW_BLUE_BITS, Creation_Hints_Info.blueBits.Get() },
              { GLFW_ALPHA_BITS, Creation_Hints_Info.alphaBits.Get() },
              { GLFW_DEPTH_BITS, Creation_Hints_Info.depthBits.Get() },
              { GLFW_STENCIL_BITS, Creation_Hints_Info.stencilBits.Get() },
              { GLFW_AUX_BUFFERS, Creation_Hints_Info.auxBuffers.Get() },
              { GLFW_STEREO, Creation_Hints_Info.stereo.ToInt() },
              { GLFW_SAMPLES, Creation_Hints_Info.samples.Get() },
              { GLFW_SRGB_CAPABLE, Creation_Hints_Info.srgbCapable.ToInt() },
              { GLFW_DOUBLEBUFFER, Creation_Hints_Info.doubleBuffer.ToInt() },
              { GLFW_REFRESH_RATE, Creation_Hints_Info.refreshRate.Get() },
              { GLFW_CLIENT_API, int(Creation_Hints_Info.clientApi) },
              { GLFW_CONTEXT_CREATION_API,
                int(Creation_Hints_Info.contextCreationApi) },
              { GLFW_CONTEXT_VERSION_MAJOR,
                Creation_Hints_Info.contextVersionMajor.Get() },
              { GLFW_CONTEXT_VERSION_MINOR,
                Creation_Hints_Info.contextVersionMinor.Get() },
              { GLFW_OPENGL_FORWARD_COMPAT,
                Creation_Hints_Info.openglForwardCompatible.ToInt() },
              { GLFW_OPENGL_DEBUG_CONTEXT,
                Creation_Hints_Info.openglDebugContext.ToInt() },
              { GLFW_OPENGL_PROFILE, int(Creation_Hints_Info.openglProfile) },
              { GLFW_CONTEXT_ROBUSTNESS,
                int(Creation_Hints_Info.contextRobustness) },
              { GLFW_CONTEXT_RELEASE_BEHAVIOR,
                int(Creation_Hints_Info.contextReleaseBehavior) },
              { GLFW_CONTEXT_NO_ERROR,
                Creation_Hints_Info.contextNoError.ToInt() },
              { GLFW_COCOA_RETINA_FRAMEBUFFER,
                Creation_Hints_Info.cocoaRetinaFramebuffer.ToInt() },
              { GLFW_COCOA_GRAPHICS_SWITCHING,
                Creation_Hints_Info.cocoaGraphicsSwitching.ToInt() } } },
          { { { GLFW_COCOA_FRAME_NAME, Creation_Hints_Info.cocoaFrameName },
              { GLFW_X11_CLASS_NAME, Creation_Hints_Info.x11ClassName },
              { GLFW_X11_INSTANCE_NAME,
                Creation_Hints_Info.x11InstanceName } } })
{
}

cursor::cursor(const standard_cursor_info& Standard_Cursor_Info)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    // NOLINTNEXTLINE
    this->handle = glfwCreateStandardCursor(Standard_Cursor_Info.shape);
}

cursor::cursor(const custom_cursor_info& Custom_Cursor_Info)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    // NOLINTNEXTLINE
    this->handle = glfwCreateCursor(Custom_Cursor_Info.image,
                                    Custom_Cursor_Info.hotspot.x,
                                    Custom_Cursor_Info.hotspot.y);
}

cursor::~cursor()
{
    glfwDestroyCursor(this->handle);
}

std::vector<vk::PipelineShaderStageCreateInfo>
pipeline_shaders::StageCreationInfos() const
{
    return { { .stage = this->vertex->stage,
               .module = this->vertex->handle.get(),
               .pName = this->vertex->entryPoint },
             { .stage = this->fragment->stage,
               .module = this->fragment->handle.get(),
               .pName = this->fragment->entryPoint } };
}

} // namespace gvw