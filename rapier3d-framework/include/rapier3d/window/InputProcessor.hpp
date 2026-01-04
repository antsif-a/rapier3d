#pragma once


#include <array>
#include <functional>
#include <vector>
#include <cstdint>

#include "rapier3d/window/framework/WindowFramework.hpp"

struct KeyEvent {
    WindowFramework::Key key;
    WindowFramework::KeyAction action;
    WindowFramework::KeyModifier modifier;

    uint32_t pack() const;
};

struct CursorEvent {
    double xpos, ypos;
};

struct ResizeEvent {
    int width, height;
};

using KeyEventCallback = std::function<void (KeyEvent)>;
using CursorEventCallback = std::function<void (CursorEvent)>;
using ResizeEventCallback = std::function<void (ResizeEvent)>;

constexpr size_t KEY_EVENTS = 1 << 17;

class InputProcessor
{
    public:

    static void WindowKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
    static void WindowCursorCallback(GLFWwindow * window, double xpos, double ypos);
    static void WindowResizeCallback(GLFWwindow * window, int width, int height);
    
    void KeyCallback(KeyEvent event);
    void RegisterKeyEventCallback(KeyEvent event, KeyEventCallback callback);
    void RegisterKeyCallback(KeyEventCallback callback);
    
    void ResizeCallback(ResizeEvent event);
    void RegisterResizeEventCallback(ResizeEventCallback callback);

    void CursorCallback(CursorEvent event);
    void RegisterCursorEventCallback(CursorEventCallback callback);

    private:
    // around 3MB
    std::array<std::vector<KeyEventCallback>, KEY_EVENTS> key_listeners;
    std::vector<KeyEventCallback> direct_key_listeners;

    std::vector<CursorEventCallback> cursor_listeners;
    std::vector<ResizeEventCallback> resize_listeners;
};
