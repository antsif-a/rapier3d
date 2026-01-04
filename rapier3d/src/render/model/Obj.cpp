#include <print>
#include <format>
#include <filesystem>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <tiny_obj_loader.h>

#include "rapier3d/render/TextureLoader.hpp"
#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include "rapier3d/render/gl/Texture.hpp"
#include "rapier3d/render/gl/Vertex.hpp"
#include "rapier3d/render/gl/Buffer.hpp"
#include "rapier3d/render/model/Obj.hpp"

static glm::mat4 GetNormalizedModelMatrix(glm::vec3 bbmin, glm::vec3 bbmax)
{
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / glm::distance(bbmax, bbmin)));
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0, 0));

    return S * R;
}

static tinyobj::ObjReader ReadObjModel(std::filesystem::path filepath)
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filepath)) 
        throw std::runtime_error(reader.Error());

    if (!reader.Warning().empty())
        std::println("tinyobjloader: {}", reader.Warning());

    return reader;
}

static std::map<std::string, Texture> LoadDiffuseTextures(
    std::filesystem::path base_path,
    std::vector<tinyobj::material_t> materials
) {
    std::map<std::string, Texture> textures;

    for (auto const & material : materials) 
    {
        if (material.diffuse_texname.empty()
                || textures.find(material.diffuse_texname) != textures.end())
            continue;

        std::filesystem::path texture_filepath = base_path / material.diffuse_texname;
        if (!std::filesystem::exists(texture_filepath))
        {
            std::println(stderr, "Unable to load texture: {} (file does not exist)", texture_filepath.c_str());
            continue;
        }

        TextureImage image = LoadTextureImage(texture_filepath);
        Texture texture(std::move(image));
        texture.MakeResident();
        textures.emplace(material.diffuse_texname, std::move(texture));
    }

    return textures;
}

// Inspired by https://github.com/tinyobjloader/tinyobjloader/blob/release/examples/viewer/viewer.cc
GLobjmodel LoadObjModel(std::filesystem::path filepath)
{
    auto reader = ReadObjModel(filepath);
    auto attrib = reader.GetAttrib();
    auto shapes = reader.GetShapes();
    auto materials = reader.GetMaterials();

    auto diffuse_textures = LoadDiffuseTextures(filepath.parent_path(), materials);

    // auto const materials = model.GetMaterials();
    // for (size_t i = 0; i < materials.size(); i++)
    //     printf("material[%i].diffuse_texname = %s", int(i), materials[i].diffuse_texname.c_str());

    std::vector<GLvertex> vertices;
    std::unordered_map<GLvertex, GLuint> vertex_map; // vertex -> vertex_index
    std::map<std::string, std::vector<GLuint>> material_map; // material -> indices

    auto bbmin = glm::vec3(
        attrib.vertices[0 + 0],
        attrib.vertices[0 + 1],
        attrib.vertices[0 + 2]
    );
    auto bbmax = glm::vec3(
        attrib.vertices[0 + 0],
        attrib.vertices[0 + 1],
        attrib.vertices[0 + 2]
    );

    auto shapes_size = shapes.size();
    for (size_t s = 0; s < shapes_size; s++)
    {
        auto& shape = shapes[s];

        auto faces_size = shape.mesh.indices.size() / 3;
        for (size_t f = 0; f < faces_size; f++)
        {
            auto material_id = shape.mesh.material_ids[f];

            for (int v = 0; v < 3; v++)
            {
                auto idx = shape.mesh.indices[3 * f + v];
                GLvertex vertex;
                vertex.position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };

                bbmin = glm::min(bbmin, vertex.position);
                bbmax = glm::max(bbmax, vertex.position);

                if (idx.normal_index >= 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }

                if (idx.texcoord_index >= 0)
                {
                    vertex.texcoords = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        // invert y axis
                        1 - attrib.texcoords[2 * idx.texcoord_index + 1]
                    };
                }

                auto v_it = vertex_map.find(vertex);
                GLuint vertex_index;
                if (v_it == vertex_map.end()) {
                    auto new_index = static_cast<GLuint>(vertices.size());

                    auto result = vertex_map.emplace(vertex, new_index);

                    vertices.push_back(vertex);
                    vertex_index = new_index;
                } else {
                    vertex_index = v_it->second;
                }

                auto texname = materials[material_id].diffuse_texname;
                auto m_it = material_map.find(texname);
                if (m_it == material_map.end()) {
                    material_map[texname] = { vertex_index };
                } else {
                    m_it->second.push_back(vertex_index);
                }
                
            }
        }
    }

    auto const draw_count = material_map.size();

    std::vector<GLuint64> texture_handles;
    // texture name holder
    std::vector<GLtexture> textures;
    std::vector<DrawElementsIndirectCommand> indirect_draws_buffer;
    std::vector<GLuint> all_indices;
    size_t current_offset = 0;
    size_t i = 0;
    for (auto& [mat_name, indices] : material_map) {
        indirect_draws_buffer.push_back({
            .count = static_cast<GLuint>(indices.size()),
            .instanceCount = 1,
            .firstIndex = static_cast<GLuint>(current_offset),
            .baseVertex = 0,
            .baseInstance = static_cast<GLuint>(i)
        });

        auto & tex = diffuse_textures.at(mat_name);
        texture_handles.push_back(tex.GetHandle());
        textures.push_back(tex.ReleaseTexture());

        all_indices.insert(all_indices.end(), indices.begin(), indices.end());
        current_offset += indices.size();

        i++;
    }

    assert(texture_handles.size() == draw_count);
    assert(indirect_draws_buffer.size() == draw_count);

    VertexArray vao;

    Buffer<GLvertex> vbo(vertices, GL_STATIC_DRAW);
    vao.VertexBuffer(vbo, sizeof(GLvertex), 0);

    Buffer<GLuint> ebo(all_indices, GL_STATIC_DRAW);
    vao.ElementBuffer(ebo);

    vao.SetupAttribute<GLfloat, 3>({ .index = 0, .relativeoffset = offsetof(GLvertex, position) });
    vao.SetupAttribute<GLfloat, 3>({ .index = 1, .relativeoffset = offsetof(GLvertex, normal) });
    vao.SetupAttribute<GLfloat, 2>({ .index = 2, .relativeoffset = offsetof(GLvertex, texcoords) });

    return {
        .vao = std::move(vao),
        .vbo = vbo.ReleaseBuffer(),
        .ebo = ebo.ReleaseBuffer(),
        .textures = std::move(textures),
        .normalized_model_matrix = GetNormalizedModelMatrix(bbmin, bbmax),
        .indirect_draws_buffer = Buffer<DrawElementsIndirectCommand>(indirect_draws_buffer, GL_STATIC_DRAW),
        .textures_ssbo = Buffer<GLuint64>(texture_handles, GL_STATIC_DRAW),
        .draw_count = static_cast<int>(draw_count),
    };
}
