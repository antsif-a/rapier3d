#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include "rapier3d/render/gl/Buffer.hpp"
#include "rapier3d/render/gl/VertexArray.hpp"

struct GLobjmodel
{
    VertexArray vao;

    GLbuffer vbo;
    GLbuffer ebo;
    std::vector<GLtexture> textures;

    glm::mat4 normalized_model_matrix;
    Buffer<DrawElementsIndirectCommand> indirect_draws_buffer;
    Buffer<GLuint64> textures_ssbo;
    int draw_count;
};

GLobjmodel LoadObjModel(std::filesystem::path filepath);
