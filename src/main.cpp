/* modern opengl
https://www.tartley.com/files/2010/03/opengl-draw-calls.png
https://computergraphics.stackexchange.com/questions/37/what-is-the-cost-of-changing-state/46#46
direct state access (so no more glBind)
bufferstorage (so no more glSubData, change memory from the pointer directly)
sparse bindless texture arrays (optimize texture bind calls)
buffer fragment (rather that buffer per object, share ubo's for many objects) + persistent mapping
for large amounts of data in shaders use SSBO (shader storage buffer object)
ARB_multi_draw_indirect (to optimize draw calls validation when nothing changes (?))
ARB_shader_draw_parameters (adds gl_DrawID to shaders, which command of multi_draw is executed)
 */

#include <print>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "window/Window.hpp"
#include "window/framework/WindowFramework.hpp"

constexpr const char * window_title = "project2025";
constexpr float window_ratio = 9.0 / 16.0;
constexpr float window_width = 1920 * 0.7;
constexpr float window_height = window_width * window_ratio;

using namespace std::chrono;

constexpr seconds target_s_per_update(1 / 60);
constexpr seconds max_s_per_frame(1 / 60); // unused, frame rate is controlled by swap interval provided by glfw

template<typename Duration>
static constexpr auto steady_clock_now() {
    return duration_cast<Duration>(steady_clock::now().time_since_epoch());
}

using namespace WindowFramework;
using namespace WindowFramework::WindowHint;

#include "RenderDriver.hpp"

// #include "render/model/GLtf.hpp"
// #include <filesystem>

int main()
{
    // LoadGLtfModel(std::filesystem::relative("models/medieval_sword_pack_1.0_low-poly__game-ready.glb"));
    // return 0;

    std::println("bobr kurwa !");
    // setenv("__NV_PRIME_RENDER_OFFLOAD", "1", 1);
    // setenv("__VK_LAYER_NV_optimus", "NVIDIA_only", 1);
    // setenv("__GLX_VENDOR_LIBRARY_NAME", "nvidia", 1);

    // init
    SetErrorCallback(WindowErrorCallback);
    Init();
    /// set up window
    SetWindowHint(ContextDebug, true);
    SetWindowHint(OpenglProfile, OpenglCoreProfile);
    SetWindowHint(ContextVersionMajor, 4);
    SetWindowHint(ContextVersionMinor, 6);
    SetWindowHint(Samples, 16);
    Window window(window_width, window_height, window_title);
    SetSwapInterval(1);

    window.SetKeyCallback(::KeyCallback);
    window.SetCursorCallback(::CursorCallback);
    window.SetWindowSizeCallback(::WindowSizeCallback);

    /*
    by specifying block layout we are sure that objects,
    which depend on window framework and graphic library,
    are destructed earlier, so these libraries can be safely disposed after
    */
    {
        /// set up render engine
        SetupRender(window_width, window_height);
        
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
