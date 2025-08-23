#pragma once

#include <filesystem>

#include "render/gl/GraphicLibrary.hpp"

struct TextureImage
{
    int width, height, channels;
    unsigned char * pixels;

    TextureImage(std::filesystem::path path) {
        pixels = stbi_load(
            path.c_str(),
            &width,
            &height,
            &channels,
            STBI_default
        );
    }

    ~TextureImage() {
        stbi_image_free(pixels);
    }
};

// an immutable texture
class Texture
{
    public:
        Texture(TextureImage data);
        Texture(std::filesystem::path path);

        void MakeResident();
        void MakeNonResident();

        GLuint64 GetHandle() const;
        GLtexture const & GetTexture() const;
        GLtexture ReleaseTexture();
    private:
        GLuint64 handle;
        GLtexture texture;
};