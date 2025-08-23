#include <print>
#include <array>

// #include <imgui.h>
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>

#include "window/framework/WindowFramework.hpp"
#include "render/gl/GraphicLibrary.hpp"
#include "render/gl/Shader.hpp"
#include "render/gl/VertexArray.hpp"
#include "render/model/Obj.hpp"
#include "scene/ViewCamera.hpp"
#include "RenderDriver.hpp"

using namespace WindowFramework;

struct Objects {
    std::array<GLobjmodel, 3> models;
    std::array<glm::mat4, 3> model_matrices;
    ShaderProgram model_shader;

    VertexArray axis_vao;
    ShaderProgram axis_shader;

    ViewCamera camera;
};

std::unique_ptr<Objects> objs(nullptr);

static glm::mat4 GetProjectionMatrix(float width, float height)
{
    return glm::perspective(
        glm::radians(45.0f),
        width / height,
        0.1f,
        10000.0f
    );
}

constexpr auto sword = "models/sword/SwordMinecraft.obj";
constexpr auto tv = "models/79571/79571.obj";
constexpr auto computer = "models/computer/78023.obj";

// init objects after context creation
void SetupRender(int window_width, int window_height)
{
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, window_width, window_height);

    objs = std::unique_ptr<Objects>(new Objects {
        .models = {
            LoadObjModel(std::filesystem::absolute(tv)),
            LoadObjModel(std::filesystem::absolute(computer)),
            LoadObjModel(std::filesystem::absolute(sword))
        },
        .model_matrices = {
            glm::translate(glm::mat4(1), glm::vec3(-1, 0, 0)),
            glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)),
            glm::translate(glm::mat4(1), glm::vec3(1, 0, 0)),
        },
        .model_shader = ShaderProgram(
            std::filesystem::relative("shaders/computer.vert"),
            std::filesystem::relative("shaders/computer.frag")
        ),
        .axis_vao = VertexArray(),
        .axis_shader = ShaderProgram(
            std::filesystem::relative("shaders/axis.vert"),
            std::filesystem::relative("shaders/axis.frag")
        ),
        .camera = ViewCamera(vec3(0, 0, 10)),
    });

    auto projection = GetProjectionMatrix(window_width, window_height);

    // --- axis ---
    objs->axis_shader.Use();
    objs->axis_shader.Uniform("projection", projection);
    objs->axis_shader.Uniform("model", mat4(1.0));

    // --- model ---
    objs->model_shader.Use();
    objs->model_shader.Uniform("projection", projection);
}

void Render(float frame_time, float delta_time)
{
    auto& [models, model_matrices, model_shader, axis_vao, axis_shader, camera] = *objs;

    glClearColor(.001, .001, .001, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    auto view_matrix = camera.ComputeViewMatrix(delta_time);

    // --- axis ---
    axis_shader.Use();
    axis_shader.Uniform("view", view_matrix);
    axis_vao.Bind();
    glDrawArrays(GL_LINES, 0, 6);

    // --- model ---
    model_shader.Use();
    model_shader.Uniform("view", view_matrix);
    for (unsigned int i = 0; i < models.size(); i++) {
        auto & model = models[i];
        auto & model_matrix = model_matrices[i];

        model_shader.Uniform<GLfloat, 4, 4>("model", model_matrix * model.normalized_model_matrix);
        model_shader.Uniform<GLuint>("handles_count", model.draw_count);
        // model.normalized_model_matrix = glm::rotate(model.normalized_model_matrix, glm::radians(20.0f * delta_time), glm::vec3(0, 1, 0));

        model.vao.Bind();
        model.indirect_draws_buffer.Bind(GL_DRAW_INDIRECT_BUFFER);
        model.textures_ssbo.Bind(GL_SHADER_STORAGE_BUFFER, 0);

        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, model.draw_count, sizeof(DrawElementsIndirectCommand));
    }
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == Key::Escape && action == KeyAction::Press) {
        glfwSetWindowShouldClose(window, true);
    }

    objs->camera.InvokeKeyCallback(key, action);
}

void CursorCallback(GLFWwindow * window, double xpos, double ypos) {
    objs->camera.InvokeCursorCallback(xpos, ypos);
}

void WindowSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
    auto projection = GetProjectionMatrix(width, height);
    objs->axis_shader.Uniform("projection", projection);
    objs->model_shader.Uniform("projection", projection);
}

void GLDebugCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void * data
) {
    
}

void WindowErrorCallback(int error, const char * description) {
    std::println(stderr, "GLFW | {}", description);
}