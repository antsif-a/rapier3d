#include "rapier3d/render/gl/Texture.hpp"
#include <filesystem>

TextureImage LoadTextureImage(std::filesystem::path path) {
    TextureImage image;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char * pixels = stbi_load(
            path.c_str(),
            &image.width,
            &image.height,
            &image.channels,
            STBI_default
    );
    image.pixels = std::unique_ptr<unsigned char>(pixels);
    return image;
}

Texture LoadTexture(std::filesystem::path path) {
    return Texture(LoadTextureImage(path));
}
