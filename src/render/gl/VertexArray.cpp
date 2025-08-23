#include "render/gl/GraphicLibrary.hpp"
#include "render/gl/Buffer.hpp"
#include "render/gl/VertexArray.hpp"

// --- VertexArray ---
VertexArray::VertexArray()
    : vertexarray(glCreateVertexArray()) {}

void VertexArray::EnableAttribute(GLuint index) {
    glEnableVertexArrayAttrib(vertexarray, index);
}

void VertexArray::AttributeFormat(
    GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLuint relativeoffset
) {
    glVertexArrayAttribFormat(vertexarray, index, size, type, normalized, relativeoffset);
}

void VertexArray::AttributeBinding(GLuint index, GLuint bindingindex) {
    glVertexArrayAttribBinding(vertexarray, index, bindingindex);
}

void VertexArray::Bind() {
    glBindVertexArray(vertexarray);
}

void VertexArray::VertexBuffer(BufferObject & buffer, GLsizei stride, GLuint bindingindex) {
    glVertexArrayVertexBuffer(vertexarray, bindingindex, buffer.GetBuffer(), 0, stride);
}

void VertexArray::ElementBuffer(BufferObject & buffer) {
    glVertexArrayElementBuffer(vertexarray, buffer.GetBuffer());
}