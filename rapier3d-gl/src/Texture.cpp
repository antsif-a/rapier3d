#include <cassert>
#include <print>

#include "rapier3d/render/gl/GraphicLibrary.hpp"
#include "rapier3d/render/gl/Texture.hpp"

static GLenum channels_to_format(int channels)
{
    switch (channels)
    {
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default:
            return GL_NONE;
    }
}

Texture::Texture(TextureImage image)
    : texture(glCreateTexture(GL_TEXTURE_2D))
    , width(image.width)
    , height(image.height)
    , channels(image.channels)
{
    auto format = channels_to_format(image.channels);
    if (format == GL_NONE) {
        std::println("Couldn't get format for channels", image.channels);
        assert(format);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureStorage2D(texture.name, 1, GL_RGBA8, image.width, image.height);
    glTextureSubImage2D(texture.name, 0, 0, 0, image.width, image.height,
                    format, GL_UNSIGNED_BYTE, image.pixels.get());

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateTextureMipmap(texture);

    if (!GLAD_GL_ARB_bindless_texture)
        throw std::runtime_error("Bindless textures not supported!");
    
    assert(handle = glGetTextureHandleARB(texture));
}

void Texture::MakeResident() {
    glMakeTextureHandleResidentARB(handle);
}

void Texture::MakeNonResident() {
    glMakeTextureHandleNonResidentARB(handle);
}

GLuint64 Texture::GetHandle() const {
    return handle;
}

GLtexture const & Texture::GetTexture() const {
    return texture;
}

GLtexture Texture::ReleaseTexture() {
    return std::exchange(texture, {});
}