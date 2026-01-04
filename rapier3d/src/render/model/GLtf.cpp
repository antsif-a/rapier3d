#include <filesystem>
#include <print>
#include <span>
#include <vector>

#include <tiny_gltf.h>

#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include "rapier3d/render/gl/Buffer.hpp"
#include "rapier3d/render/model/GLtf.hpp"

namespace tinygltf
{
    constexpr auto POSITION = std::string("POSITION");
    constexpr auto NORMAL = std::string("NORMAL");
    constexpr auto TEXCOORD_0 = std::string("TEXCOORD_0");
}

static std::vector<Buffer<unsigned char>> SetupBuffers(
    std::span<tinygltf::Buffer> buffers,
    std::span<tinygltf::BufferView> bufferViews
) {
    std::vector<Buffer<unsigned char>> glBuffers;
    for (size_t i = 0; i < bufferViews.size(); i++) {
        auto & view = bufferViews[i];
        if (view.target == 0) {
            std::println("WARN: bufferView.target is zero");
            continue;
        }

        tinygltf::Buffer & buffer = buffers[view.buffer];

        // glBuffers[i] = Buffer(
        //     std::span(buffer.data.begin() + view.byteOffset, view.byteLength),
        //     view.target
        // );
    }

    return glBuffers;
}

GLtfmodel LoadGLtfModel(std::filesystem::path filepath) 
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err, warn;
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filepath);

    if (!warn.empty())
        std::println(stderr, "{}", warn);
    if (!err.empty())
        std::println(stderr, "{}", err);
    if (!ret)
        throw std::runtime_error("Failed to load GLtf");
    
    auto & accessors = model.accessors;
    auto & buffers = model.buffers;
    auto & bufferViews = model.bufferViews;

    auto glBuffers = SetupBuffers(buffers, bufferViews);

    uint scene_i = 0;
    for (auto & scene : model.scenes) {
        std::println("scene #{}", scene_i);
        uint node_i = 0;
        for (auto & nodeId : scene.nodes) {
            auto & node = model.nodes[nodeId];
            std::println("node #{}, mesh = {}", node_i, node.mesh);
            if (node.mesh != -1) {
                auto & mesh = model.meshes[node.mesh];
                std::println("\tmesh \"{}\", {} primitives", mesh.name, mesh.primitives.size());
                for (auto & primitive : mesh.primitives) {
                    tinygltf::Material material = model.materials[primitive.material];

                    tinygltf::Accessor indices = model.accessors[primitive.indices];

                    tinygltf::Accessor vertices = accessors[primitive.attributes[tinygltf::POSITION]];
                    assert(vertices.type == TINYGLTF_TYPE_VEC3);
                    assert(vertices.componentType == GL_FLOAT);
                    int verticesByteStride = vertices.ByteStride(bufferViews[vertices.bufferView]);

                    tinygltf::Accessor normals = accessors[primitive.attributes[tinygltf::NORMAL]];
                    assert(vertices.type == TINYGLTF_TYPE_VEC3);
                    assert(vertices.componentType == GL_FLOAT);
                    int normalsByteStride = normals.ByteStride(bufferViews[normals.bufferView]);
                }
            }
            node_i++;
        }
        scene_i++;
    }

    return {};
}