#include <upch.h>
#include "image.h"
#include <Platform/OpenGL/gl_image.h>

utd::image::format utd::image::retrive_format(int channels)
{
    switch(channels)
    {
        case 1: return utd::image::format::RGB8;
        case 3: return utd::image::format::RGB8;
        case 4: return utd::image::format::RGBA8;
    }
    return  utd::image::format::NONE;
}

std::uptr<utd::image> utd::image::load(const std::filesystem::path &path)
{
    return std::make_unique<gl_image>(path);
}