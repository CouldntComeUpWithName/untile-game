#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Graphics/Framebuffer.h>

namespace utd
{
    class gl_framebuffer : public framebuffer
    {
    public:
        gl_framebuffer() = default;
        gl_framebuffer(u32 width, u32 height);

        virtual void bind() override;
        virtual void unbind() override;
        virtual ~gl_framebuffer() override;
    private:
        virtual void _attach() override;
        virtual void _resize() override;
        virtual i32 _read_pixel(u32 attachment_index, i32 x, i32 y) override;
        virtual void _erase_attachment(u32 attachment_index, int value) override;
        virtual u32 _at(int index) const override;
        void _release();
        bool _has_depth();
        bool _has_depth_only();
    private:
        std::vector<u32> m_attachment_ids;
    };
}