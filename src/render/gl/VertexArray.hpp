#pragma once

#include "render/gl/GraphicLibrary.hpp"
#include "render/gl/Buffer.hpp"

struct VertexArrayAttribute {
    GLuint index;
    GLuint relativeoffset;
    GLuint binding = 0;
    GLboolean normalized = GL_FALSE;
};

class VertexArray
{
    public:
        VertexArray();

        void VertexBuffer(BufferObject & buffer, GLsizei stride, GLuint bindingindex);
        void ElementBuffer(BufferObject & buffer);

        void EnableAttribute(GLuint index);
        void AttributeFormat(GLuint index, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
        void AttributeBinding(GLuint index, GLuint bindingindex);

        template<typename type, GLint count>
        void SetupAttribute(VertexArrayAttribute const& attribute)
        {
            EnableAttribute(attribute.index);
            AttributeFormat(attribute.index, count, GLtype<type>::value, attribute.normalized, attribute.relativeoffset);
            AttributeBinding(attribute.index, attribute.binding);
        }

        void Bind();
    private:
        GLvertexarray vertexarray;
};