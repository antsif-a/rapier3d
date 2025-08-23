#pragma once

#include <vector>
#include <array>
#include <algorithm>

#include "render/gl/GraphicLibrary.hpp"

constexpr auto max_textures = 8;
constexpr auto max_shaders = 8;

class RenderQueue
{
    public:
        void put(std::array<GLuint, 2> key, GLuint value)
        {
            queue.push_back(std::make_pair(key, value));
            std::sort(queue.begin(), queue.end());
        }
        void draw()
        {
            GLuint program = 0, texture = 0, vertexarray = 0;
            for (const auto &p : queue)
            {
                if (program != p.first[0]) {
                    program = p.first[0];
                    glUseProgram(program);
                }

                if (texture != p.first[1]) {
                    texture = p.first[1];
                    glBindTexture(GL_TEXTURE_2D, texture);
                }

                if (vertexarray != p.second) {
                    vertexarray = p.second;
                    glBindVertexArray(vertexarray);
                }
                
                // glDrawArrays(GL_TRIANGLES, 0, 0);
            }
        }
    private:
        std::vector<std::pair<std::array<GLuint, 2>, GLuint>> queue;
};