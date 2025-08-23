#pragma once

#include <filesystem>
#include <concepts>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/gl/GraphicLibrary.hpp"
#include "render/gl/Texture.hpp"

// --- glUniform ---
template <typename T>
concept GLuniformtype = std::same_as<T, GLfloat>
    || std::same_as<T, GLint>
    || std::same_as<T, GLuint>
    || std::same_as<T, GLboolean>;

template <GLuniformtype Type, unsigned int N>
requires (1 <= N and N <= 4)
struct GLuniform;

#define declare_uniform(Type, N, Params) \
    template<> struct GLuniform<Type, N> { \
        static constexpr auto glUniform =  &glUniform##N##Params; \
        static constexpr auto glUniformArray = &glUniform##N##Params##v; \
    };

#define uniform_n(Type, Params) \
    declare_uniform(Type, 1, Params); \
    declare_uniform(Type, 2, Params); \
    declare_uniform(Type, 3, Params); \
    declare_uniform(Type, 4, Params);

uniform_n(GLfloat, f);
uniform_n(GLint, i);
uniform_n(GLuint, ui);
uniform_n(GLboolean, ui);

#undef uniform
#undef uniform_n

// --- glUniformMatrix ---
template <unsigned int N, unsigned int M = N>
requires (2 <= N && N <= 4 && 2 <= M && M <= 4)
struct GLuniform_matrix;

#define uniform_mat_n_n(N) \
    template<> struct GLuniform_matrix<N> { \
        static constexpr auto glUniformMatrix = &glUniformMatrix##N##fv; \
    };
#define uniform_mat_n_m(N, M) \
    template<> struct GLuniform_matrix<N, M> { \
        static constexpr auto glUniformMatrix = &glUniformMatrix##N##x##M##fv; \
    };

uniform_mat_n_n(2);
uniform_mat_n_m(2, 3);
uniform_mat_n_m(2, 4);
uniform_mat_n_n(3);
uniform_mat_n_m(3, 2);
uniform_mat_n_m(3, 4);
uniform_mat_n_n(4);
uniform_mat_n_m(4, 2);
uniform_mat_n_m(4, 3);

#undef uniform_mat_n_n
#undef uniform_mat_n_

using namespace glm;

//todo: implement uniform buffer objects
class ShaderProgram
{
    public:
        // ShaderProgram(Shader vert_shader, Shader frag_shader);
        ShaderProgram(const char * vertex_shader_source, const char * fragment_shader_source);
        ShaderProgram(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);

        void Use();

        template <GLuniformtype T>
        void Uniform(const char * name, T value)
        {
            const auto glUniform = *GLuniform<T, 1>::glUniform;
            glUniform(uniform_map[name], value);
        }

        template <GLuniformtype T>
        void Uniform(const char * name, std::span<T> value)
        {
            const auto glUniform = *GLuniform<T, 1>::glUniformArray;
            glUniform(uniform_map[name], value.size(), value.data());
        }

        template <GLuniformtype T, int N>
        void Uniform(const char * name, vec<N, T> value)
        {
            const auto glUniform = *GLuniform<T, N>::glUniformArray;
            glUniform(uniform_map[name], 1, glm::value_ptr(value));
        }

        template <GLuniformtype T, int N, int M>
        void Uniform(const char * name, mat<N, M, T> value) 
        {
            const auto glUniformMatrix = *GLuniform_matrix<N, M>::glUniformMatrix;
            glUniformMatrix(uniform_map[name], 1, GL_FALSE, glm::value_ptr(value));
        }

        void UniformTexture(const char * name, Texture & texture) {
            glProgramUniformHandleui64ARB(program, uniform_map[name], texture.GetHandle());
        }

        GLprogram & GetProgram();
    private:
        GLprogram program;
        std::unordered_map<std::string, GLuint> uniform_map;
};
