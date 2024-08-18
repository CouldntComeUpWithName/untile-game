#include <upch.h>
#include "graphics_api.h"

#include <Engine/Platform/OpenGL/opengl_api.h>
#include <Engine/Core/assert.h>

std::uptr<utd::graphics_api> utd::graphics_api::create(utd::graphics_api::type _API)
{
    switch(_API)
    {
    case graphics_api::type::OPENGL:
        return std::make_unique<utd::opengl_api>();
    case graphics_api::type::DX11:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support DX11 yet!");
        return std::uptr<graphics_api>();
    case graphics_api::type::DX12:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support DX12 yet!");
        return std::uptr<graphics_api>();
    case graphics_api::type::VULKAN:
        UTD_ENGINE_ASSERT(false, "Untiled Engine does NOT support VULKAN yet!");
        return std::uptr<graphics_api>();
    case graphics_api::type::UNKNOWN:
        UTD_ENGINE_ASSERT(false, "Unknown graphics API!");
        return std::uptr<graphics_api>();
    }
    
    return std::uptr<graphics_api>();
}