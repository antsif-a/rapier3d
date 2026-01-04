#include <print>
#include <chrono>

#include <rapier3d/window/Window.hpp>
#include <rapier3d/window/framework/WindowFramework.hpp>
#include <rapier3d/render/gl/GraphicLibrary.hpp>
#include <rapier3d/render/gl/Shader.hpp>
#include <rapier3d/render/gl/VertexArray.hpp>
#include <rapier3d/render/model/Obj.hpp>
#include <rapier3d/scene/ViewCamera.hpp>

// #include <imgui.h>
// #include <imgui_impl_glfw.h>
// #include <imgui_impl_opengl3.h>

using namespace std::chrono;
using namespace WindowFramework;
using namespace WindowFramework::WindowHint;

namespace resources {
    namespace shaders {
        namespace computer {
            const char vertex[] = {
                #embed "../assets/shaders/computer.vert"
                , 0
            };
            const char fragment[] = {
                #embed "../assets/shaders/computer.vert"
                , 0
            };
        };
    }
}

// global gl objects
struct Objects {
    std::array<GLobjmodel, 3> models;
    std::array<glm::mat4, 3> model_matrices;
    ShaderProgram model_shader;

    VertexArray axis_vao;
    ShaderProgram axis_shader;

    ViewCamera camera;
}; std::unique_ptr<Objects> objs(nullptr);
//

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

void SetupWindow(Window & window) {
    auto & input = window.input();

    input.RegisterKeyEventCallback({Key::Escape, KeyAction::Press}, [&](auto) {
        window.Close();
    });
    input.RegisterKeyEventCallback({Key::F11, KeyAction::Press}, [&](auto) {
        window.ToggleFullscreen();
    });

    input.RegisterKeyCallback([] (KeyEvent e) {
        objs->camera.InvokeKeyCallback(e.key, e.action);
    });

    input.RegisterCursorEventCallback([] (CursorEvent e) {
        objs->camera.InvokeCursorCallback(e.xpos, e.ypos);
    });

    input.RegisterResizeEventCallback([] (ResizeEvent e) {
        glViewport(0, 0, e.width, e.height);
        auto projection = GetProjectionMatrix(e.width, e.height);
        objs->axis_shader.Uniform("projection", projection);
        objs->model_shader.Uniform("projection", projection);
    });
}

// init objects after context creation
void SetupRender(int window_width, int window_height)
{
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DefaultGLMessageCallback, nullptr);

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
            resources::shaders::computer::fragment,
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

constexpr const char * window_title = "project2025";
constexpr float window_ratio = 9.0 / 16.0;
constexpr float window_width = 1920 * 0.7;
constexpr float window_height = window_width * window_ratio;

constexpr seconds target_s_per_update(1 / 60);
constexpr seconds max_s_per_frame(1 / 60); // unused, frame rate is controlled by swap interval provided by glfw

template<typename Duration>
static constexpr auto steady_clock_now() {
    return duration_cast<Duration>(steady_clock::now().time_since_epoch());
}

int main()
{
    // LoadGLtfModel(std::filesystem::relative("models/medieval_sword_pack_1.0_low-poly__game-ready.glb"));
    // return 0;

    std::println("bobr kurwa !");

    // init
    SetErrorCallback(DefaultErrorCallback);
    /// set up window
    Window window(window_width, window_height, window_title, {
        {ContextDebug, true},
        {OpenglProfile, OpenglCoreProfile},
        {ContextVersionMajor, 4},
        {ContextVersionMinor, 6},
        {Samples, 16}
    });
    window.SetCursorMode(CursorMode::Disabled);
    SetSwapInterval(1);

    /*
    by specifying block layout we are sure that objects,
    which depend on window framework and graphic library,
    are destructed earlier, so these libraries can be safely disposed after
    */
    {
        /// set up render engine
        SetupRender(window_width, window_height);
        SetupWindow(window);
        
        /// set up main loop
        auto last_update_time = steady_clock_now<nanoseconds>();
        auto last_frame_time = steady_clock_now<nanoseconds>();

        auto last_frame_time_check = steady_clock_now<seconds>();
        auto frames_counted = 0;
        auto average_frame_time = 0;
        while (!window.ShouldClose())
        {
            // process input
            /// poll input events from driver
            PollEvents();
            /// handle input events (these are set up by callbacks)
            // update
            auto update_time = steady_clock_now<milliseconds>();
            if (update_time - last_update_time > target_s_per_update)
            {
                /// update player camera
                /// update physics, entity positions
                last_update_time = update_time;
            }
            // render
            auto frame_time = steady_clock_now<nanoseconds>();
            auto delta_time = (frame_time - last_frame_time).count();
            if (frame_time - last_frame_time_check >= seconds(1)) {
                std::println("frames per second: {}, average frame time: {}ms",
                    frames_counted, average_frame_time / 1'000'000.0f);
                last_frame_time_check = duration_cast<seconds>(frame_time);
                frames_counted = 1;
                average_frame_time = 0;
            } else {
                frames_counted++;
                average_frame_time = average_frame_time + ((delta_time - average_frame_time) / frames_counted);
            }
            Render(frame_time.count(), delta_time / 1'000'000'000.0f);
            last_frame_time = frame_time;

            /// swap buffers
            window.SwapBuffers();
        }
    }

    return 0;
}
