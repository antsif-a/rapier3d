#pragma once

#include "window/framework/WindowFramework.hpp"
#include "render/gl/GraphicLibrary.hpp"

void GLDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void * data
);

void WindowErrorCallback(int error, const char * description);

void SetupRender(int window_width, int window_height);
void Render(float frame_time, float delta_time);

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
void CursorCallback(GLFWwindow * window, double xpos, double ypos);
void WindowSizeCallback(GLFWwindow * window, int width, int height);
