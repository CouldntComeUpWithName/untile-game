#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Graphics/image.h>

namespace utd
{
    class gl_image : public image
    {
    public:
        gl_image() = default;
        gl_image(const std::filesystem::path&);
        gl_image(const buffer&);
        
        void filepath(const std::filesystem::path&) override;
        void source(const buffer&) override;
        void reload() override;
    private:
        void _filepath(const std::filesystem::path&);
        void _source(const buffer&);
    };
}