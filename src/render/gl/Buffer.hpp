#pragma once

#include <span>
#include <string_view>
#include "render/gl/GraphicLibrary.hpp"

class BufferObject {
    public:
        void Bind(GLenum target)
        {
            glBindBuffer(target, GetBuffer());
        }

        void Bind(GLenum target, GLuint index)
        {
            glBindBufferBase(target, index, GetBuffer());
        }

        void SetLabel(std::string_view label)
        {
            glObjectLabel(GL_BUFFER, GetBuffer(), label.size(), label.data());
        }

        virtual ~BufferObject() = default;
        virtual GLbuffer const & GetBuffer() const = 0;
};

template<typename T>
class Buffer : public BufferObject
{
    public:
        // Assign
        Buffer(GLbuffer buffer);

        // Create
        Buffer();
        
        // Create + Allocate
        Buffer(GLsizeiptr size, GLenum usage);

        // Create + Allocate + Data (from a pointer)
        Buffer(T * data, GLsizeiptr size, GLenum usage);

        // Create + Allocate + Data (from a contiguous view over a sequence of objects)
        Buffer(std::span<T> data, GLenum usage);

        // Allocate + Data (from a pointer)
        void Data(T * data, GLsizeiptr size, GLenum usage);

        // Allocate + Data (from a contiguous view over a sequence of objects)
        void Data(std::span<T> data, GLenum usage);

        // Update (from a pointer)
        template<typename U>
        void Update(U * data, GLsizeiptr size, GLsizeiptr offset);

        GLbuffer const & GetBuffer() const override;
        GLbuffer ReleaseBuffer();
    private:
        GLbuffer buffer;
};

template<typename T>
class BufferStorage : public BufferObject 
{
    public:
        // Create
        BufferStorage();
        
        // Create + Data Store
        BufferStorage(GLsizeiptr size, GLbitfield access);
        
        // Create + Data Store (from a pointer)
        BufferStorage(T * data, GLsizeiptr size, GLbitfield access);

        // Data Store (from a pointer)
        void Storage(T * data, GLsizeiptr size, GLbitfield access);

        // Map data store to a local address space (without offset)
        T * Map(GLbitfield flags);

        // Map data store to a local address space (with offset)
        T * Map(GLintptr offset, GLsizeiptr length, GLbitfield access);

        // Unmap data store from a local address space
        void Unmap();

        GLbuffer const & GetBuffer() const override;
        GLbuffer ReleaseBuffer();
    private:
        GLbuffer buffer;
};

#include "render/gl/Buffer.inl"
