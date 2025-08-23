#pragma once

#include <cassert>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace WindowFramework
{
    using ErrorCallback = GLFWerrorfun;
    using KeyCallback = GLFWkeyfun;
    using CursorCallback = GLFWcursorposfun;
    using WindowSizeCallback = GLFWwindowsizefun;
    
    inline ErrorCallback SetErrorCallback(ErrorCallback callback) {return glfwSetErrorCallback(callback); };
    inline int Init() { return glfwInit(); }
    inline void PollEvents() { glfwPollEvents(); }
    inline void SetSwapInterval(int interval) { glfwSwapInterval(interval); }
    inline void SetWindowHint(int hint, int value) { glfwWindowHint(hint, value); }
    inline void SetWindowHint(int hint, const char * value) { glfwWindowHintString(hint, value); }

    namespace WindowHint
    {
        constexpr auto Focused = GLFW_FOCUSED;
        constexpr auto Iconified = GLFW_ICONIFIED;
        constexpr auto Resizable = GLFW_RESIZABLE;
        constexpr auto Visible = GLFW_VISIBLE;
        constexpr auto Decorated = GLFW_DECORATED;
        constexpr auto AutoIconify = GLFW_AUTO_ICONIFY;
        constexpr auto Floating = GLFW_FLOATING;
        constexpr auto Maximized = GLFW_MAXIMIZED;
        constexpr auto CenterCursor = GLFW_CENTER_CURSOR;
        constexpr auto TransparentFramebuffer = GLFW_TRANSPARENT_FRAMEBUFFER;
        constexpr auto Hovered = GLFW_HOVERED;
        constexpr auto FocusOnShow = GLFW_FOCUS_ON_SHOW;
        constexpr auto MousePassthrough = GLFW_MOUSE_PASSTHROUGH;
        constexpr auto PositionX = GLFW_POSITION_X;
        constexpr auto PositionY = GLFW_POSITION_Y;
        constexpr auto RedBits = GLFW_RED_BITS;
        constexpr auto GreenBits = GLFW_GREEN_BITS;
        constexpr auto BlueBits = GLFW_BLUE_BITS;
        constexpr auto AlphaBits = GLFW_ALPHA_BITS;
        constexpr auto DepthBits = GLFW_DEPTH_BITS;
        constexpr auto StencilBits = GLFW_STENCIL_BITS;
        constexpr auto AccumRedBits = GLFW_ACCUM_RED_BITS;
        constexpr auto AccumGreenBits = GLFW_ACCUM_GREEN_BITS;
        constexpr auto AccumBlueBits = GLFW_ACCUM_BLUE_BITS;
        constexpr auto AccumAlphaBits = GLFW_ACCUM_ALPHA_BITS;
        constexpr auto AuxBuffers = GLFW_AUX_BUFFERS;
        constexpr auto Stereo = GLFW_STEREO;
        constexpr auto Samples = GLFW_SAMPLES;
        constexpr auto SrgbCapable = GLFW_SRGB_CAPABLE;
        constexpr auto RefreshRate = GLFW_REFRESH_RATE;
        constexpr auto Doublebuffer = GLFW_DOUBLEBUFFER;
        constexpr auto ClientApi = GLFW_CLIENT_API;
        constexpr auto ContextVersionMajor = GLFW_CONTEXT_VERSION_MAJOR;
        constexpr auto ContextVersionMinor = GLFW_CONTEXT_VERSION_MINOR;
        constexpr auto ContextRevision = GLFW_CONTEXT_REVISION;
        constexpr auto ContextRobustness = GLFW_CONTEXT_ROBUSTNESS;
        constexpr auto OpenglForwardCompat = GLFW_OPENGL_FORWARD_COMPAT;
        constexpr auto ContextDebug = GLFW_CONTEXT_DEBUG;
        constexpr auto OpenglDebugContext = GLFW_OPENGL_DEBUG_CONTEXT;
        constexpr auto OpenglProfile = GLFW_OPENGL_PROFILE;
        constexpr auto ContextReleaseBehavior = GLFW_CONTEXT_RELEASE_BEHAVIOR;
        constexpr auto ContextNoError = GLFW_CONTEXT_NO_ERROR;
        constexpr auto ContextCreationApi = GLFW_CONTEXT_CREATION_API;
        constexpr auto ScaleToMonitor = GLFW_SCALE_TO_MONITOR;
        constexpr auto ScaleFramebuffer = GLFW_SCALE_FRAMEBUFFER;
        constexpr auto CocoaRetinaFramebuffer = GLFW_COCOA_RETINA_FRAMEBUFFER;
        constexpr auto CocoaFrameName = GLFW_COCOA_FRAME_NAME;
        constexpr auto CocoaGraphicsSwitching = GLFW_COCOA_GRAPHICS_SWITCHING;
        constexpr auto X11ClassName = GLFW_X11_CLASS_NAME;
        constexpr auto X11InstanceName = GLFW_X11_INSTANCE_NAME;
        constexpr auto Win32KeyboardMenu = GLFW_WIN32_KEYBOARD_MENU;
        constexpr auto Win32Showdefault = GLFW_WIN32_SHOWDEFAULT;
        constexpr auto WaylandAppId = GLFW_WAYLAND_APP_ID;

        constexpr auto NoApi = GLFW_NO_API;
        constexpr auto OpenglApi = GLFW_OPENGL_API;
        constexpr auto OpenglEsApi = GLFW_OPENGL_ES_API;
        constexpr auto NoRobustness = GLFW_NO_ROBUSTNESS;
        constexpr auto NoResetNotification = GLFW_NO_RESET_NOTIFICATION;
        constexpr auto LoseContextOnReset = GLFW_LOSE_CONTEXT_ON_RESET;
        constexpr auto OpenglAnyProfile = GLFW_OPENGL_ANY_PROFILE;
        constexpr auto OpenglCoreProfile = GLFW_OPENGL_CORE_PROFILE;
        constexpr auto OpenglCompatProfile = GLFW_OPENGL_COMPAT_PROFILE;
        constexpr auto Cursor = GLFW_CURSOR;
        constexpr auto StickyKeys = GLFW_STICKY_KEYS;
        constexpr auto StickyMouseButtons = GLFW_STICKY_MOUSE_BUTTONS;
        constexpr auto LockKeyMods = GLFW_LOCK_KEY_MODS;
        constexpr auto RawMouseMotion = GLFW_RAW_MOUSE_MOTION;
        constexpr auto UnlimitedMouseButtons = GLFW_UNLIMITED_MOUSE_BUTTONS;
        constexpr auto CursorNormal = GLFW_CURSOR_NORMAL;
        constexpr auto CursorHidden = GLFW_CURSOR_HIDDEN;
        constexpr auto CursorDisabled = GLFW_CURSOR_DISABLED;
        constexpr auto CursorCaptured = GLFW_CURSOR_CAPTURED;
        constexpr auto AnyReleaseBehavior = GLFW_ANY_RELEASE_BEHAVIOR;
        constexpr auto ReleaseBehaviorFlush = GLFW_RELEASE_BEHAVIOR_FLUSH;
        constexpr auto ReleaseBehaviorNone = GLFW_RELEASE_BEHAVIOR_NONE;
        constexpr auto NativeContextApi = GLFW_NATIVE_CONTEXT_API;
        constexpr auto EglContextApi = GLFW_EGL_CONTEXT_API;
        constexpr auto OsmesaContextApi = GLFW_OSMESA_CONTEXT_API;
        constexpr auto AnglePlatformTypeNone = GLFW_ANGLE_PLATFORM_TYPE_NONE;
        constexpr auto AnglePlatformTypeOpengl = GLFW_ANGLE_PLATFORM_TYPE_OPENGL;
        constexpr auto AnglePlatformTypeOpengles = GLFW_ANGLE_PLATFORM_TYPE_OPENGLES;
        constexpr auto AnglePlatformTypeD3d9 = GLFW_ANGLE_PLATFORM_TYPE_D3D9;
        constexpr auto AnglePlatformTypeD3d11 = GLFW_ANGLE_PLATFORM_TYPE_D3D11;
        constexpr auto AnglePlatformTypeVulkan = GLFW_ANGLE_PLATFORM_TYPE_VULKAN;
        constexpr auto AnglePlatformTypeMetal = GLFW_ANGLE_PLATFORM_TYPE_METAL;
        constexpr auto WaylandPreferLibdecor = GLFW_WAYLAND_PREFER_LIBDECOR;
        constexpr auto WaylandDisableLibdecor = GLFW_WAYLAND_DISABLE_LIBDECOR;
        constexpr auto AnyPosition = GLFW_ANY_POSITION;
    }

    namespace Key
    {
        constexpr auto Space = GLFW_KEY_SPACE;
        constexpr auto Apostrophe = GLFW_KEY_APOSTROPHE;
        constexpr auto Comma = GLFW_KEY_COMMA;
        constexpr auto Minus = GLFW_KEY_MINUS;
        constexpr auto Period = GLFW_KEY_PERIOD;
        constexpr auto Slash = GLFW_KEY_SLASH;
        constexpr auto Key0 = GLFW_KEY_0;
        constexpr auto Key1 = GLFW_KEY_1;
        constexpr auto Key2 = GLFW_KEY_2;
        constexpr auto Key3 = GLFW_KEY_3;
        constexpr auto Key4 = GLFW_KEY_4;
        constexpr auto Key5 = GLFW_KEY_5;
        constexpr auto Key6 = GLFW_KEY_6;
        constexpr auto Key7 = GLFW_KEY_7;
        constexpr auto Key8 = GLFW_KEY_8;
        constexpr auto Key9 = GLFW_KEY_9;
        constexpr auto Semicolon = GLFW_KEY_SEMICOLON;
        constexpr auto Equal = GLFW_KEY_EQUAL;
        constexpr auto A = GLFW_KEY_A;
        constexpr auto B = GLFW_KEY_B;
        constexpr auto C = GLFW_KEY_C;
        constexpr auto D = GLFW_KEY_D;
        constexpr auto E = GLFW_KEY_E;
        constexpr auto F = GLFW_KEY_F;
        constexpr auto G = GLFW_KEY_G;
        constexpr auto H = GLFW_KEY_H;
        constexpr auto I = GLFW_KEY_I;
        constexpr auto J = GLFW_KEY_J;
        constexpr auto K = GLFW_KEY_K;
        constexpr auto L = GLFW_KEY_L;
        constexpr auto M = GLFW_KEY_M;
        constexpr auto N = GLFW_KEY_N;
        constexpr auto O = GLFW_KEY_O;
        constexpr auto P = GLFW_KEY_P;
        constexpr auto Q = GLFW_KEY_Q;
        constexpr auto R = GLFW_KEY_R;
        constexpr auto S = GLFW_KEY_S;
        constexpr auto T = GLFW_KEY_T;
        constexpr auto U = GLFW_KEY_U;
        constexpr auto V = GLFW_KEY_V;
        constexpr auto W = GLFW_KEY_W;
        constexpr auto X = GLFW_KEY_X;
        constexpr auto Y = GLFW_KEY_Y;
        constexpr auto Z = GLFW_KEY_Z;
        constexpr auto LeftBracket = GLFW_KEY_LEFT_BRACKET;
        constexpr auto Backslash = GLFW_KEY_BACKSLASH;
        constexpr auto RightBracket = GLFW_KEY_RIGHT_BRACKET;
        constexpr auto GraveAccent = GLFW_KEY_GRAVE_ACCENT;
        constexpr auto World1 = GLFW_KEY_WORLD_1;
        constexpr auto World2 = GLFW_KEY_WORLD_2;
        constexpr auto Escape = GLFW_KEY_ESCAPE;
        constexpr auto Enter = GLFW_KEY_ENTER;
        constexpr auto Tab = GLFW_KEY_TAB;
        constexpr auto Backspace = GLFW_KEY_BACKSPACE;
        constexpr auto Insert = GLFW_KEY_INSERT;
        constexpr auto Delete = GLFW_KEY_DELETE;
        constexpr auto Right = GLFW_KEY_RIGHT;
        constexpr auto Left = GLFW_KEY_LEFT;
        constexpr auto Down = GLFW_KEY_DOWN;
        constexpr auto Up = GLFW_KEY_UP;
        constexpr auto PageUp = GLFW_KEY_PAGE_UP;
        constexpr auto PageDown = GLFW_KEY_PAGE_DOWN;
        constexpr auto Home = GLFW_KEY_HOME;
        constexpr auto End = GLFW_KEY_END;
        constexpr auto CapsLock = GLFW_KEY_CAPS_LOCK;
        constexpr auto ScrollLock = GLFW_KEY_SCROLL_LOCK;
        constexpr auto NumLock = GLFW_KEY_NUM_LOCK;
        constexpr auto PrintScreen = GLFW_KEY_PRINT_SCREEN;
        constexpr auto Pause = GLFW_KEY_PAUSE;
        constexpr auto F1 = GLFW_KEY_F1;
        constexpr auto F2 = GLFW_KEY_F2;
        constexpr auto F3 = GLFW_KEY_F3;
        constexpr auto F4 = GLFW_KEY_F4;
        constexpr auto F5 = GLFW_KEY_F5;
        constexpr auto F6 = GLFW_KEY_F6;
        constexpr auto F7 = GLFW_KEY_F7;
        constexpr auto F8 = GLFW_KEY_F8;
        constexpr auto F9 = GLFW_KEY_F9;
        constexpr auto F10 = GLFW_KEY_F10;
        constexpr auto F11 = GLFW_KEY_F11;
        constexpr auto F12 = GLFW_KEY_F12;
        constexpr auto F13 = GLFW_KEY_F13;
        constexpr auto F14 = GLFW_KEY_F14;
        constexpr auto F15 = GLFW_KEY_F15;
        constexpr auto F16 = GLFW_KEY_F16;
        constexpr auto F17 = GLFW_KEY_F17;
        constexpr auto F18 = GLFW_KEY_F18;
        constexpr auto F19 = GLFW_KEY_F19;
        constexpr auto F20 = GLFW_KEY_F20;
        constexpr auto F21 = GLFW_KEY_F21;
        constexpr auto F22 = GLFW_KEY_F22;
        constexpr auto F23 = GLFW_KEY_F23;
        constexpr auto F24 = GLFW_KEY_F24;
        constexpr auto F25 = GLFW_KEY_F25;
        constexpr auto Kp0 = GLFW_KEY_KP_0;
        constexpr auto Kp1 = GLFW_KEY_KP_1;
        constexpr auto Kp2 = GLFW_KEY_KP_2;
        constexpr auto Kp3 = GLFW_KEY_KP_3;
        constexpr auto Kp4 = GLFW_KEY_KP_4;
        constexpr auto Kp5 = GLFW_KEY_KP_5;
        constexpr auto Kp6 = GLFW_KEY_KP_6;
        constexpr auto Kp7 = GLFW_KEY_KP_7;
        constexpr auto Kp8 = GLFW_KEY_KP_8;
        constexpr auto Kp9 = GLFW_KEY_KP_9;
        constexpr auto KpDecimal = GLFW_KEY_KP_DECIMAL;
        constexpr auto KpDivide = GLFW_KEY_KP_DIVIDE;
        constexpr auto KpMultiply = GLFW_KEY_KP_MULTIPLY;
        constexpr auto KpSubtract = GLFW_KEY_KP_SUBTRACT;
        constexpr auto KpAdd = GLFW_KEY_KP_ADD;
        constexpr auto KpEnter = GLFW_KEY_KP_ENTER;
        constexpr auto KpEqual = GLFW_KEY_KP_EQUAL;
        constexpr auto LeftShift = GLFW_KEY_LEFT_SHIFT;
        constexpr auto LeftControl = GLFW_KEY_LEFT_CONTROL;
        constexpr auto LeftAlt = GLFW_KEY_LEFT_ALT;
        constexpr auto LeftSuper = GLFW_KEY_LEFT_SUPER;
        constexpr auto RightShift = GLFW_KEY_RIGHT_SHIFT;
        constexpr auto RightControl = GLFW_KEY_RIGHT_CONTROL;
        constexpr auto RightAlt = GLFW_KEY_RIGHT_ALT;
        constexpr auto RightSuper = GLFW_KEY_RIGHT_SUPER;
        constexpr auto Menu = GLFW_KEY_MENU;
        constexpr auto Last = GLFW_KEY_LAST;
    };

    namespace KeyAction
    {
        constexpr auto Press = GLFW_PRESS;
        constexpr auto Repeat = GLFW_REPEAT;
        constexpr auto Release = GLFW_RELEASE;
    };
    
    namespace KeyModifier
    {
        constexpr auto Shift = GLFW_MOD_SHIFT;
        constexpr auto Control = GLFW_MOD_CONTROL;
        constexpr auto Alt = GLFW_MOD_ALT;
        constexpr auto Super = GLFW_MOD_SUPER;
        constexpr auto CapsLock = GLFW_MOD_CAPS_LOCK;
        constexpr auto NumLock = GLFW_MOD_NUM_LOCK;
    };
}