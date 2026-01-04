#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>

#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include "rapier3d/window/InputProcessor.hpp"
#include "rapier3d/window/framework/WindowFramework.hpp"
#include "rapier3d/window/Window.hpp"

using namespace WindowFramework;

void abort(const char * reason) {
    std::cerr << reason << std::endl;
    std::abort();
}

GLFWwindow * create_window(int width, int height, const char * title, std::map<int, int> hints)
{
    if (!glfwGetErrorCallback())
        glfwSetErrorCallback(DefaultErrorCallback);

    if (!glfwInit())
        abort("Could not initialize window framework");

    for (const auto [hint, value] : hints)
        glfwWindowHint(hint, value);

    auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) 
        abort("Could not create window");

    return window;
}

Window::Window(int width, int height, const char * title, std::map<int, int> hints)
    : window(create_window(width, height, title, hints))
{
    glfwMakeContextCurrent(window.get());
    if (!gladLoadGL(glfwGetProcAddress))
        abort("Could not load graphic library");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DefaultGLMessageCallback, window.get());

    _context = std::unique_ptr<WindowContext>(
        new WindowContext(
            InputProcessor()
        )
    );
    glfwSetWindowUserPointer(window.get(), _context.get());

    glfwSetKeyCallback(window.get(), InputProcessor::WindowKeyCallback);
    glfwSetWindowSizeCallback(window.get(), InputProcessor::WindowResizeCallback);
    glfwSetCursorPosCallback(window.get(), InputProcessor::WindowCursorCallback);
}

void Window::Close()
{
    glfwSetWindowShouldClose(window.get(), true);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window.get());
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window.get());
}

void Window::Maximize() {
    glfwMaximizeWindow(window.get());
}

void Window::ToggleFullscreen() {
    if (GLFWmonitor * m = glfwGetWindowMonitor(window.get())) {
        const GLFWvidmode * vidmode = glfwGetVideoMode(m); 
        glfwSetWindowMonitor(window.get(), NULL, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    } else {
        GLFWmonitor * monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * vidmode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window.get(), monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    }
}


void Window::SetCursorMode(WindowFramework::CursorMode mode) {
    glfwSetInputMode(window.get(), GLFW_CURSOR, static_cast<int>(mode));
}

glm::ivec2 Window::GetSize() {
    glm::ivec2 size;
    glfwGetFramebufferSize(window.get(), &size.x, &size.y);
    return size;
}

glm::ivec2 Window::GetPosition() {
    glm::ivec2 position;
    glfwGetWindowPos(window.get(), &position.x, &position.y);
    return position;
}


InputProcessor & Window::input() {
    return Context(window.get())->input;
}

WindowContext * Window::Context(GLFWwindow * window)
{
    return WindowFramework::GetState<WindowContext>(window);
}

