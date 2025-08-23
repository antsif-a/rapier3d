#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

in Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fragment;

out vec4 Color;

// layout (bindless_sampler) uniform sampler2D computer_sampler;
layout (std430, binding = 0) readonly buffer TextureHandles {
    uint64_t handles[];
};
uniform uint handles_count;

flat in int vBaseInstance;

void main()
{
    uint64_t handle = handles[vBaseInstance];
    if (handle == 0 || vBaseInstance >= handles_count) {
        Color = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        Color = texture(sampler2D(handle), fragment.TexCoords);
    }
}