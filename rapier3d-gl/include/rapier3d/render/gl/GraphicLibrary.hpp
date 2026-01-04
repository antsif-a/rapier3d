#pragma once

#include <glad/gl.h>
#include <utility>

// Add CreateFn ?
template <void(*DeleteFn)(GLuint)>
struct GLObject
{
    GLuint name = 0;

    GLObject() noexcept = default; 
    explicit GLObject(GLuint name) noexcept
        : name(name) {}

    ~GLObject() { Reset(); };

    GLObject(GLObject const &) = delete;
    GLObject & operator=(GLObject const &) = delete;

    GLObject(GLObject && object) noexcept 
        : name(std::exchange(object.name, 0)) {};
        
    GLObject & operator=(GLObject && object) noexcept
    {
        if (this != &object)
        {
            Reset();
            name = std::exchange(object.name, 0);
        }
        return * this;
    }

    void Reset() noexcept
    {
        if (name)
        {
            DeleteFn(name);
            name = 0;
        }
    }

    operator GLuint() const { return name; }

    friend bool operator == (GLObject x, GLObject y) { return x.name == y.name; }
    friend bool operator != (GLObject x, GLObject y) { return x.name != y.name; }
};

// todo these are deleted by one, implement multiple?
inline void DeleteBuffer(GLuint name) { glDeleteBuffers(1, &name); }
inline void DeleteVertexArray(GLuint name) { glDeleteVertexArrays(1, &name); }
inline void DeleteTexture(GLuint name) { glDeleteTextures(1, &name); }
inline void DeleteFramebuffer(GLuint name) {glDeleteFramebuffers(1, &name); }

inline void DeleteShader(GLuint name) { glDeleteShader(name); }
inline void DeleteProgram(GLuint name) { glDeleteProgram(name); }

using GLbuffer = GLObject<DeleteBuffer>;
using GLvertexarray = GLObject<DeleteVertexArray>;
using GLtexture = GLObject<DeleteTexture>;
using GLshader = GLObject<DeleteShader>;
using GLprogram = GLObject<DeleteProgram>;
using GLframebuffer = GLObject<DeleteFramebuffer>;

// helpers to create certain objects by one
inline GLuint glCreateBuffer() { GLuint name; glCreateBuffers(1, &name); return name; }
inline GLuint glCreateVertexArray() { GLuint name; glCreateVertexArrays(1, &name); return name; }
inline GLuint glCreateTexture(GLenum target) { GLuint name; glCreateTextures(target, 1, &name); return name; }
inline GLuint glCreateFramebuffer() { GLuint name; glCreateFramebuffers(1, &name); return name; }

/*
type indicates the type of the data.
If type is one of GL_BYTE, GL_SHORT, GL_INT, GL_FIXED, GL_FLOAT, GL_HALF_FLOAT, and GL_DOUBLE
    indicate types GLbyte, GLshort, GLint, GLfixed, GLfloat, GLhalf, and GLdouble, respectively;
the values GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, and GL_UNSIGNED_INT
    indicate types GLubyte, GLushort, and GLuint, respectively;
*/
template<typename T>
struct GLtype;

#define associate_type(Type, Value)                         \
    template<> struct GLtype<Type> {                        \
        using value_type = GLenum;                          \
        static constexpr value_type value = Value;          \
    };

associate_type(GLbyte, GL_BYTE);
associate_type(GLubyte /* GLboolean */, GL_UNSIGNED_BYTE); 

associate_type(GLshort, GL_SHORT);
associate_type(GLushort /* GLhalf */, GL_UNSIGNED_SHORT);

associate_type(GLint /* GLfixed */, GL_INT);
associate_type(GLuint, GL_UNSIGNED_INT);

associate_type(GLfloat, GL_FLOAT);
associate_type(GLdouble, GL_DOUBLE);

#undef associate_type

struct DrawElementsIndirectCommand {
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLint  baseVertex;
    GLuint baseInstance;
};

void DefaultGLMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void * data
);