#include <cassert>

#include "render/gl/GraphicLibrary.hpp"
#include "window/framework/WindowFramework.hpp"
#include "window/Window.hpp"

using namespace WindowFramework;

GLFWwindow * create_window(int width, int height, const char * title)
{
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Window::Window(int width, int height, const char * title)
    : window(create_window(width, height, title))
{
    assert(window.get());
    glfwMakeContextCurrent(window.get());
    assert(gladLoadGL(glfwGetProcAddress));
    printf("GL Vendor: %s\n", glGetString(GL_VENDOR));
    
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

KeyCallback Window::SetKeyCallback(KeyCallback callback)
{

    return glfwSetKeyCallback(window.get(), callback);
}

CursorCallback Window::SetCursorCallback(CursorCallback callback)
{
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return glfwSetCursorPosCallback(window.get(), callback);
}

WindowSizeCallback Window::SetWindowSizeCallback(WindowSizeCallback callback)
{
    return glfwSetWindowSizeCallback(window.get(), callback);
}
