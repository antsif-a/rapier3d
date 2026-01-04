#pragma once

#include "rapier3d/render/gl/Buffer.hpp"

// --- Buffer ---
template<typename T>
Buffer<T>::Buffer(GLbuffer buffer)
    : buffer(std::move(buffer)) {};

template<typename T>
Buffer<T>::Buffer()
    : buffer(glCreateBuffer()) {}

template<typename T>
Buffer<T>::Buffer(GLsizeiptr size, GLenum usage)
    : Buffer()
{
    Data(nullptr, size, usage);
}

template<typename T>
Buffer<T>::Buffer(T * data, GLsizeiptr size, GLenum usage)
    : Buffer()
{
    Data(data, size, usage);
}

template<typename T>
Buffer<T>::Buffer(std::span<T> data, GLenum usage)
    : Buffer()
{
    Data(data, usage);
}

template<typename T>
void Buffer<T>::Data(T * data, GLsizeiptr size, GLenum usage) {
    glNamedBufferData(buffer, size, data, usage);
}

template<typename T>
void Buffer<T>::Data(std::span<T> data, GLenum usage) {
    glNamedBufferData(buffer, data.size_bytes(), data.data(), usage);
}

template<typename T>
template<typename U>
void Buffer<T>::Update(U * data, GLsizeiptr size, GLsizeiptr offset) {
    glNamedBufferSubData(buffer, offset, size, data);
}

template<typename T>
GLbuffer const & Buffer<T>::GetBuffer() const {
    return buffer;
}

template<typename T>
GLbuffer Buffer<T>::ReleaseBuffer() {
    return std::exchange(buffer, {});
}

// --- BufferStorage ---
template<typename T>
BufferStorage<T>::BufferStorage()
    : buffer(glCreateBuffer()) {}

template<typename T>
BufferStorage<T>::BufferStorage(GLsizeiptr size, GLbitfield access)
    : BufferStorage()
{
    Storage(nullptr, size, access);
}

template<typename T>
BufferStorage<T>::BufferStorage(T * data, GLsizeiptr size, GLbitfield access)
    : BufferStorage()
{
    Storage(data, size, access);
}

template<typename T>
void BufferStorage<T>::Storage(T * data, GLsizeiptr size, GLbitfield access) {
    glNamedBufferStorage(buffer, size, data, access);
}

template<typename T>
T * BufferStorage<T>::Map(GLbitfield access) {
    return static_cast<T *>(glMapNamedBuffer(buffer, access));
}

template<typename T>
T * BufferStorage<T>::Map(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return static_cast<T *>(glMapNamedBufferRange(buffer, offset, length, access));
}

template<typename T>
void BufferStorage<T>::Unmap() {
    glUnmapNamedBuffer(buffer);
}

template<typename T>
GLbuffer const & BufferStorage<T>::GetBuffer() const {
    return buffer;
}

template<typename T>
GLbuffer BufferStorage<T>::ReleaseBuffer() {
    return std::exchange(buffer, {});
}