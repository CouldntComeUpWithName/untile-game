#include <upch.h>
#include "texture.h"

#include <stb_image.h>

#include <Platform/OpenGL/gl_texture.h>

std::uptr<utd::texture> utd::texture::load(const std::string& filepath)
{
    return std::make_unique<gl_texture>(filepath);
}

std::uptr<utd::texture> utd::texture::create()
{
    return std::make_unique<gl_texture>(texture::specs());
}
