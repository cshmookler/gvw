// Standard includes

// External includes

// Local includes
#include "types.tpp"

namespace gvw {

glfw_shared_init_hints::glfw_shared_init_hints(int Joystick_Hat_Buttons)
    : glfw_hints(glfwInitHint,
                 nullptr,
                 { { { GLFW_JOYSTICK_HAT_BUTTONS, Joystick_Hat_Buttons } } },
                 { {} })
{
}

glfw_macos_init_hints::glfw_macos_init_hints(int Cocoa_CHDIR_Resources,
                                             int Cocoa_Menubar)
    : glfw_hints(glfwInitHint,
                 nullptr,
                 { { { GLFW_COCOA_CHDIR_RESOURCES, Cocoa_CHDIR_Resources },
                     { GLFW_COCOA_MENUBAR, Cocoa_Menubar } } },
                 { {} })
{
}

glfw_general_hints::glfw_general_hints(int Resizable,
                                       int Visible,
                                       int Decorated,
                                       int Focused,
                                       int Auto_Iconify,
                                       int Floating,
                                       int Maximized,
                                       int Center_Cursor,
                                       int Transparent_Framebuffer,
                                       int Focus_On_Show,
                                       int Scale_To_Monitor)
    : glfw_hints(glfwWindowHint,
                 glfwWindowHintString,
                 { { { GLFW_RESIZABLE, Resizable },
                     { GLFW_VISIBLE, Visible },
                     { GLFW_DECORATED, Decorated },
                     { GLFW_FOCUSED, Focused },
                     { GLFW_AUTO_ICONIFY, Auto_Iconify },
                     { GLFW_FLOATING, Floating },
                     { GLFW_MAXIMIZED, Maximized },
                     { GLFW_CENTER_CURSOR, Center_Cursor },
                     { GLFW_TRANSPARENT_FRAMEBUFFER, Transparent_Framebuffer },
                     { GLFW_FOCUS_ON_SHOW, Focus_On_Show },
                     { GLFW_SCALE_TO_MONITOR, Scale_To_Monitor } } },
                 { {} })
{
}

glfw_framebuffer_hints::glfw_framebuffer_hints(int Red_Bits,
                                               int Green_Bits,
                                               int Blue_Bits,
                                               int Alpha_Bits,
                                               int Depth_Bits,
                                               int Stencil_Bits,
                                               int AUX_Buffers,
                                               int Stereo,
                                               int Samples,
                                               int SRGB_Capable,
                                               int Double_Buffer)
    : glfw_hints(glfwWindowHint,
                 glfwWindowHintString,
                 { { { GLFW_RED_BITS, Red_Bits },
                     { GLFW_GREEN_BITS, Green_Bits },
                     { GLFW_BLUE_BITS, Blue_Bits },
                     { GLFW_ALPHA_BITS, Alpha_Bits },
                     { GLFW_DEPTH_BITS, Depth_Bits },
                     { GLFW_STENCIL_BITS, Stencil_Bits },
                     { GLFW_AUX_BUFFERS, AUX_Buffers },
                     { GLFW_STEREO, Stereo },
                     { GLFW_SAMPLES, Samples },
                     { GLFW_SRGB_CAPABLE, SRGB_Capable },
                     { GLFW_DOUBLEBUFFER, Double_Buffer } } },
                 { {} })
{
}

glfw_monitor_hints::glfw_monitor_hints(int Refresh_Rate)
    : glfw_hints(glfwWindowHint,
                 glfwWindowHintString,
                 { { { GLFW_REFRESH_RATE, Refresh_Rate } } },
                 { {} })
{
}

glfw_context_hints::glfw_context_hints(int Client_API,
                                       int Context_Creation_API,
                                       int Context_Version_Major,
                                       int Context_Version_Minor,
                                       int OpenGL_Forward_Compatible,
                                       int OpenGL_Debug_Context,
                                       int OpenGL_Profile,
                                       int Context_Robustness,
                                       int Context_Release_Behavior,
                                       int Context_No_Error)
    : glfw_hints(
          glfwWindowHint,
          glfwWindowHintString,
          { { { GLFW_CLIENT_API, Client_API },
              { GLFW_CONTEXT_CREATION_API, Context_Creation_API },
              { GLFW_CONTEXT_VERSION_MAJOR, Context_Version_Major },
              { GLFW_CONTEXT_VERSION_MINOR, Context_Version_Minor },
              { GLFW_OPENGL_FORWARD_COMPAT, OpenGL_Forward_Compatible },
              { GLFW_OPENGL_DEBUG_CONTEXT, OpenGL_Debug_Context },
              { GLFW_OPENGL_PROFILE, OpenGL_Profile },
              { GLFW_CONTEXT_ROBUSTNESS, Context_Robustness },
              { GLFW_CONTEXT_RELEASE_BEHAVIOR, Context_Release_Behavior },
              { GLFW_CONTEXT_NO_ERROR, Context_No_Error } } },
          { {} })
{
}

glfw_macos_window_hints::glfw_macos_window_hints(int Cocoa_Retina_Framebuffer,
                                                 int Cocoa_Graphics_Switching,
                                                 const char* Cocoa_Frame_Name)
    : glfw_hints(
          glfwWindowHint,
          glfwWindowHintString,
          { { { GLFW_COCOA_RETINA_FRAMEBUFFER, Cocoa_Retina_Framebuffer },
              { GLFW_COCOA_GRAPHICS_SWITCHING, Cocoa_Graphics_Switching } } },
          { { { GLFW_COCOA_FRAME_NAME, Cocoa_Frame_Name } } })
{
}

glfw_linux_window_hints::glfw_linux_window_hints(const char* X11_Class_Name,
                                                 const char* X11_Instance_Name)
    : glfw_hints(glfwWindowHint,
                 glfwWindowHintString,
                 { {} },
                 { { { GLFW_X11_CLASS_NAME, X11_Class_Name },
                     { GLFW_X11_INSTANCE_NAME, X11_Instance_Name } } })
{
}

key_event::key_event(int Key, int Scancode, int Action, int Mods)
    : key(Key)
    , scancode(Scancode)
    , action(Action)
    , mods(Mods)
{
}

mouse_button_event::mouse_button_event(int Button, int Action, int Mods)
    : button(Button)
    , action(Action)
    , mods(Mods)
{
}

file_drop_event::file_drop_event(int Count, const char** Paths)
    : count(Count)
    , paths(Paths)
{
}

joystick_event::joystick_event(int JID, int Event)
    : jid(JID)
    , event(Event)
{
}

} // namespace gvw