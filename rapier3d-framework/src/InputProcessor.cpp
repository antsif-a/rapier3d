#include "rapier3d/window/Window.hpp"
#include "rapier3d/window/framework/WindowFramework.hpp"
#include <print>
#include <rapier3d/window/InputProcessor.hpp>

uint32_t KeyEvent::pack() const
{
    return static_cast<uint32_t>(static_cast<uint16_t>(key)       & 0b111111111)
        | (static_cast<uint16_t>(static_cast<uint8_t>(action)     & 0b111111) << (15))
        | (static_cast<uint16_t>(static_cast<uint8_t>(modifier)   & 0b11) << 9);
}

/// static
void InputProcessor::WindowKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    auto * state = Window::Context(window); assert(state != nullptr);
    auto event = KeyEvent(
        static_cast<WindowFramework::Key>(key),
        static_cast<WindowFramework::KeyAction>(action),
        static_cast<WindowFramework::KeyModifier>(mods)
    );
    state->input.KeyCallback(event);
}

void InputProcessor::WindowCursorCallback(GLFWwindow * window, double xpos, double ypos) {
    auto * state = Window::Context(window); assert(state != nullptr);
    auto event = CursorEvent(xpos, ypos);
    state->input.CursorCallback(event);
}
void InputProcessor::WindowResizeCallback(GLFWwindow * window, int width, int height) {
    auto * state = Window::Context(window); assert(state != nullptr);
    auto event = ResizeEvent(width, height);
    state->input.ResizeCallback(event);
}
///

void InputProcessor::KeyCallback(KeyEvent event) {
    for (auto & listener : key_listeners[event.pack()]) {
        listener(event);
    }
    for (auto & listener : direct_key_listeners) {
        listener(event);
    }
}
void InputProcessor::RegisterKeyCallback(KeyEventCallback callback) {
    direct_key_listeners.push_back(callback);
}
void InputProcessor::RegisterKeyEventCallback(KeyEvent event, KeyEventCallback callback) {
    key_listeners[event.pack()].push_back(callback);
}

void InputProcessor::ResizeCallback(ResizeEvent event) {
    for (auto & listener : resize_listeners) {
        listener(event);
    }
}
void InputProcessor::RegisterResizeEventCallback(ResizeEventCallback callback) {
    resize_listeners.push_back(callback);
}

void InputProcessor::CursorCallback(CursorEvent event) {
    for (auto & listener : cursor_listeners) {
        listener(event);
    }
}
void InputProcessor::RegisterCursorEventCallback(CursorEventCallback callback) {
    cursor_listeners.push_back(callback);
}
