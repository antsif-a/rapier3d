#pragma once

#include "rapier3d/window/framework/WindowFramework.hpp"
#include <map>
#include <memory>
#include <rapier3d/window/InputProcessor.hpp>
#include <glm/glm.hpp>

struct GLFWwindowDeleter
{
    void operator () (GLFWwindow * window) const {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

struct WindowContext {
    InputProcessor input;
};

struct WindowGeometry {
    glm::vec2 position;
    glm::vec2 size;
};

class Window
{
    public:
        Window(int width, int height, const char * title, std::map<int, int> hints = {});
        Window(const Window&) = delete;

        void Close();
        void Maximize();
        void ToggleFullscreen();
        bool ShouldClose();
        void SwapBuffers();

        void SetCursorMode(WindowFramework::CursorMode mode);

        glm::ivec2 GetSize();
        glm::ivec2 GetPosition();

        InputProcessor & input();
        static WindowContext * Context(GLFWwindow * window);
    private:
        std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
        std::unique_ptr<WindowContext> _context;
};
