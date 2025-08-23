#pragma once

#include <memory>
#include "window/framework/WindowFramework.hpp"

struct GLFWwindowDeleter
{
    void operator () (GLFWwindow * window) const {
        glfwDestroyWindow(window);
        // glfwTerminate();
    }
};

class Window
{
    public:

        Window(int width, int height, const char * title);
        Window(const Window&) = delete;

        void Close();
        bool ShouldClose();
        void SwapBuffers();

        WindowFramework::KeyCallback SetKeyCallback(WindowFramework::KeyCallback callback);
        WindowFramework::CursorCallback SetCursorCallback(WindowFramework::CursorCallback callback);
        WindowFramework::WindowSizeCallback SetWindowSizeCallback(WindowFramework::WindowSizeCallback callback);

        template <typename T> void SetState(T * state)
        {
            glfwSetWindowUserPointer(window.get(), state);
        }
        template <typename T> static T * GetState(GLFWwindow * window)
        {
            return static_cast<T *>(glfwGetWindowUserPointer(window));
        }
    private:
        std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
};