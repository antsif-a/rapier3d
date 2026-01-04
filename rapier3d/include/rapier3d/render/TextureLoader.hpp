#pragma once

#include <filesystem>
#include <rapier3d/render/gl/Texture.hpp>

TextureImage LoadTextureImage(std::filesystem::path path);
Texture LoadTexture(std::filesystem::path path);