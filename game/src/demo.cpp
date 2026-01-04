#include <filesystem>
#include <glm/glm.hpp>

#include <rapier3d/window/Window.hpp>
#include <rapier3d/window/framework/WindowFramework.hpp>
#include <rapier3d/render/gl/GraphicLibrary.hpp>
#include <rapier3d/render/gl/Texture.hpp>
#include <rapier3d/render/gl/Buffer.hpp>
#include <rapier3d/render/gl/Vertex.hpp>
#include <rapier3d/render/gl/VertexArray.hpp>
#include <rapier3d/render/gl/Shader.hpp>
#include <rapier3d/render/TextureLoader.hpp>

namespace fs = std::filesystem;
using namespace WindowFramework;
using namespace glm;


std::array<GLvertex, 4> square = {
    GLvertex(vec3(-1, 1, 0), vec3(0), vec2(0, 1)),
    GLvertex(vec3(1, 1, 0), vec3(0), vec2(1, 1)),
    GLvertex(vec3(-1, -1, 0), vec3(0), vec2(0, 0)),
    GLvertex(vec3(1, -1, 0), vec3(0), vec2(1, 0)),
};
std::array<GLuint, 6> square_indices = {
    0, 1, 2,
    1, 2, 3
};

std::vector<std::string> images {
    "wallhaven-3l1ew9.jpg",
    "wallhaven-7j2kv9.jpg",
    "wallhaven-7jp25v.jpg",
    "wallhaven-e87qgw.jpg",
    "wallhaven-gwp7yl.jpg",
    "wallhaven-zpyygy.jpg",
    "wallhaven-1qkgzv.jpg",
    "wallhaven-5y7q17.jpg",
};

namespace r {
    namespace shaders {
        constexpr char simple_frag[] = {
            #embed "../assets/shaders/simple.frag"
            , 0
        };

        constexpr char simple_vert[] = {
            #embed "../assets/shaders/simple.vert"
            , 0
        };
    };
};

int main() {
    Window window(1920 * 0.5, 1200 * 0.5, "hot sexy girls");
    
    VertexArray va;
    Buffer<GLvertex> vb(square, GL_STATIC_DRAW);
    va.VertexBuffer(vb, sizeof(GLvertex), 0);
    Buffer<GLuint> eb(square_indices, GL_STATIC_DRAW);
    va.ElementBuffer(eb);
    va.SetupAttribute<GLfloat, 3>({ .index = 0, .relativeoffset = offsetof(GLvertex, position) });
    va.SetupAttribute<GLfloat, 3>({ .index = 1, .relativeoffset = offsetof(GLvertex, normal) });
    va.SetupAttribute<GLfloat, 2>({ .index = 2, .relativeoffset = offsetof(GLvertex, texcoords) });

    auto shader = ShaderProgram(
        std::string_view(r::shaders::simple_vert),
        std::string_view(r::shaders::simple_frag)
    );

    std::vector<Texture> textures = {};
    int texture_idx = 0;
    for (auto p : images) {
        Texture t = LoadTexture(fs::relative("models/" + p));
        textures.push_back(std::move(t));
    }

    shader.Use();
    shader.UniformTexture("milf", textures[0]); 
    shader.Uniform("TextureSize", vec2(textures[0].width, textures[0].height));
    shader.Uniform("ScreenSize", vec2(1920 * 0.5, 1200 * 0.5));
    window.input().RegisterResizeEventCallback([&] (ResizeEvent e) {
        shader.Uniform("ScreenSize", vec2(e.width, e.height));
        glViewport(0, 0, e.width, e.height);
    });
    window.input().RegisterKeyEventCallback({Key::Escape, KeyAction::Press}, [&](KeyEvent) {
        window.Close();
    });
    window.input().RegisterKeyEventCallback({Key::F11, KeyAction::Press}, [&](KeyEvent) {
        window.ToggleFullscreen();
    });

    window.input().RegisterKeyCallback([&] (KeyEvent e) {
        if (e.action == KeyAction::Press && (e.key == Key::Left || e.key == Key::Right)) {
            if (e.key == Key::Left)
                texture_idx = (texture_idx - 1 + textures.size()) % textures.size();
            else if (e.key == Key::Right)
                texture_idx = (texture_idx + 1) % textures.size();
            auto & t = textures[texture_idx];
            shader.UniformTexture("milf", t); 
            shader.Uniform("TextureSize", vec2(t.width, t.height));
        }
    }); 

    while (!window.ShouldClose()) {
        PollEvents();
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        va.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        window.SwapBuffers();
    }
}