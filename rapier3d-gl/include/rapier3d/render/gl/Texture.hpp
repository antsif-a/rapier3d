#pragma once

#include <memory>
#include <rapier3d/render/gl/GraphicLibrary.hpp>

struct TextureImage
{
    int width, height, channels;
    std::unique_ptr<unsigned char> pixels;
};

// an immutable texture
class Texture
{
    public:
        int width, height, channels;

        Texture(TextureImage data);

        void MakeResident();
        void MakeNonResident();

        GLuint64 GetHandle() const;
        GLtexture const & GetTexture() const;
        GLtexture ReleaseTexture();
    private:
        GLuint64 handle;
        GLtexture texture;
};