#version 460 core
#extension GL_ARB_bindless_texture : require

in Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} fragment;

layout(bindless_sampler) uniform sampler2D milf;

out vec4 Color;

void main() {
    Color = texture(milf, fragment.TexCoords);
}
