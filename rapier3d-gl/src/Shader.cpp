#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include <filesystem>
#include <ios>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string_view>
#include <format>
#include <cassert>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rapier3d/render/gl/Shader.hpp>

static std::string read_file(std::filesystem::path path)
{
    
    std::ifstream stream(path.c_str(), std::ios_base::ate | std::ios_base::binary);
    stream.exceptions(std::ios_base::badbit);

    if (!stream)
        throw std::ios_base::failure(std::format("Cannot open {}", path.c_str()));

    std::streamsize size = stream.tellg();
    std::string out(size, '\0');
    stream.seekg(0, std::ios::beg);
    stream.read(out.data(), size);
    return out;
}

static void assert_shader(GLuint const shader)
{
    GLint result = GL_FALSE;
    int log_length;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0)
    {
        std::vector<char> error_message(log_length + 1);
        glGetShaderInfoLog(shader, log_length, nullptr, &error_message[0]);
        std::cerr << std::format("Shader.cpp::assert_shader: {}", &error_message[0]) << std::endl;
        assert(log_length <= 0);
    }
}

static void assert_program(GLuint const program)
{
    GLint result = GL_FALSE;
    int log_length;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        std::vector<char> error_message(log_length + 1);
        glGetProgramInfoLog(program, log_length, nullptr, &error_message[0]);
        std::cerr << &error_message[0] << std::endl;
        assert(log_length <= 0);
    }
}

static void MakeUniformMap(GLprogram & program, std::unordered_map<std::string, GLuint> & map) {
    GLint uniform_count;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count == 0) 
        return;

    GLint maxlen;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlen);
    
    GLsizei length;
    GLint size;
    GLenum type;
    GLchar name[maxlen];

    for (GLint i = 0; i < uniform_count; i++) {
        glGetActiveUniform(program, i, maxlen, &length, &size, &type, name);
        map[std::string(name)] = glGetUniformLocation(program, name);
    }
}

static void MakeProgram(
    std::string_view vert_shader_source, 
    std::string_view frag_shader_source,
    GLprogram & program,
    std::unordered_map<std::string, GLuint> & uniform_map
) {
    const char * data = vert_shader_source.data();
    GLint length = static_cast<GLint>(vert_shader_source.length());
    GLshader vert_shader(glCreateShader(GL_VERTEX_SHADER)),
        frag_shader(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(vert_shader, 1, &data, &length);
    glCompileShader(vert_shader);
    assert_shader(vert_shader);
    glAttachShader(program, vert_shader);

    data = frag_shader_source.data();
    length = static_cast<GLint>(frag_shader_source.length());

    glShaderSource(frag_shader, 1, &data, &length);
    glCompileShader(frag_shader);
    assert_shader(frag_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    assert_program(program);

    MakeUniformMap(program, uniform_map);
}

ShaderProgram::ShaderProgram(
    std::string_view vert_shader_source, 
    std::string_view frag_shader_source
) : program(glCreateProgram()) {
    MakeProgram(vert_shader_source, frag_shader_source, program, uniform_map);
}

ShaderProgram::ShaderProgram(
    std::filesystem::path vert_shader_path,
    std::filesystem::path frag_shader_path
) : program(glCreateProgram()) {
    std::string vert_shader_source = read_file(vert_shader_path);
    std::string frag_shader_source = read_file(frag_shader_path);
    MakeProgram(vert_shader_source, frag_shader_source, program, uniform_map);
}

void ShaderProgram::Use()
{
    glUseProgram(program);
}

GLprogram & ShaderProgram::GetProgram() {
    return program;
}