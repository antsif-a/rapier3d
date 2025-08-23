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

#include "render/gl/Shader.hpp"
#include "render/gl/Texture.hpp"

static_assert(sizeof(std::string("asd")));

std::string read_file(std::string_view path)
{
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (!stream)
        throw std::ios_base::failure(std::format("Cannot open {}", path.data()));

    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) 
        out.append(buf, 0, stream.gcount());
    out.append(buf, 0, stream.gcount());

    return out;
}

void assert_shader(GLuint const shader)
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

GLuint compile_shader(GLenum const type, const char* shaderSource)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    assert_shader(shader);
    return shader;
}

GLuint compile_shader_binary(GLenum const type, const void* data, GLsizei size)
{
    GLuint shader = glCreateShader(type);
    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data, size);
    glSpecializeShader(shader, "main", 0, nullptr, nullptr);
    assert_shader(shader);
    return shader;
}

void assert_program(GLuint const program)
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

GLuint link_program(GLuint vert_shader, GLuint frag_shader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    assert_program(program);
    return program;
}

GLuint link_program_from_files(const char* vert_shader_filename, const char* frag_shader_filename)
{
    return link_program(
        compile_shader(GL_VERTEX_SHADER, read_file(vert_shader_filename).c_str()),
        compile_shader(GL_FRAGMENT_SHADER, read_file(frag_shader_filename).c_str())
    );
}

void assert_uniform_location(GLuint location, const char* name)
{
    if (location == -1) {
        std::cerr << "uniform location not found: " << name << std::endl;
        assert(location != -1);
    }
}

void uniform_int(GLuint program, const char* name, GLint value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform1i(location, value);
}

void uniform_uint(GLuint program, const char* name, GLint value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform1ui(location, value);
}

void uniform_bool(GLuint program, const char * name, bool value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform1ui(location, value);
}

void uniform_float(GLuint program, const char* name, GLfloat value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform1f(location, value);
    
}

void uniform_vec2(GLuint program, const char* name, glm::vec2 value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform2f(location, value.x, value.y);
}

void uniform_vec3(GLuint program, const char* name, glm::vec3 value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform3f(location, value.x, value.y, value.z);
}

void uniform_vec4(GLuint program, const char* name, glm::vec4 value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void uniform_mat4(GLuint program, const char* name, glm::mat4 value)
{
    GLuint location = glGetUniformLocation(program, name);
    assert_uniform_location(location, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
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

ShaderProgram::ShaderProgram(
    const char * vert_shader_source, 
    const char * frag_shader_source
) : program(glCreateProgram())
{
    GLshader vert_shader(glCreateShader(GL_VERTEX_SHADER)),
        frag_shader(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(vert_shader, 1, &vert_shader_source, nullptr);
    glCompileShader(vert_shader);
    assert_shader(vert_shader);
    glAttachShader(program, vert_shader);

    glShaderSource(frag_shader, 1, &frag_shader_source, nullptr);
    glCompileShader(frag_shader);
    assert_shader(frag_shader);
    glAttachShader(program, frag_shader);

    glLinkProgram(program);
    assert_program(program);

    MakeUniformMap(program, uniform_map);
}

ShaderProgram::ShaderProgram(
    std::filesystem::path vert_shader_path,
    std::filesystem::path frag_shader_path
) : ShaderProgram(
    read_file(vert_shader_path.c_str()).c_str(),
    read_file(frag_shader_path.c_str()).c_str()
) {}

void ShaderProgram::Use()
{
    glUseProgram(program);
}

GLprogram & ShaderProgram::GetProgram() {
    return program;
}