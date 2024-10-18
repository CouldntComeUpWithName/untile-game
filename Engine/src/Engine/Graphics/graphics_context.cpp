#include "upch.h"
#include "graphics_context.h"

#include <Platform/OpenGL/opengl_context.h>

std::uptr<utd::graphics_context> utd::graphics_context::create()
{
    return std::make_unique<utd::opengl_context>();
}