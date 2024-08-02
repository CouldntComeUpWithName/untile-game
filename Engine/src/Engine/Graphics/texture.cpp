#include <upch.h>
#include "texture.h"

#include <stb_image.h>

#include <Engine/Platform/OpenGL/gl_texture.h>

std::uptr<utd::texture> utd::texture::load(const std::string& filepath)
{
    return std::make_unique<gl_texture>(filepath);
}
