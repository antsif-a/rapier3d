#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} vertex;

flat out int vBaseInstance;

void main()
{
    vertex.Position = vec3(model * vec4(Position, 1.0));
    // vertex.Normal = mat3(transpose(inverse(model))) * Normal;
    vertex.TexCoords = TexCoords;
    vBaseInstance = gl_BaseInstance;
    
    gl_Position = projection * view * model * vec4(Position + gl_InstanceID * 10, 1.0);
}
