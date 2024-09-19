#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Core/Assert.h>

namespace utd
{
    class framebuffer
    {
    public:
        enum class attachment
        {
            NONE = 0,
            RGBA8,
            RED_INT,
            DEPTH24_STENCIL8,
            DEPTH = DEPTH24_STENCIL8
        };

        static constexpr u32 MAX_WIDTH = 8192U;
        static constexpr u32 MAX_HEIGHT = 8192U;
        
    public:
        framebuffer() = default;
        framebuffer(u32 width, u32 height);

        virtual void bind() = 0;
        virtual void unbind() = 0;
        
        template<typename... t_attachments>
        void attach(t_attachments&&... attachments);
        void resize(u32 width, u32 height);
        i32 read_pixel(u32 attachment_index, i32 x, i32 y);
        void erase(u32 attachment_index, int value);
        
        inline u32 width() const { return m_width; }
        inline u32 height() const { return m_height; }
        inline u32 at(int index) const { return _at(index); }
        u32 operator[](int index) const { return _at(index); }

        virtual ~framebuffer() = default;

    public:
        static std::uptr<framebuffer> create();
        static std::uptr<framebuffer> create(u32 width, u32 height);
    
    private:
        virtual void _attach() = 0;
        virtual void _resize() = 0;
        virtual i32 _read_pixel(u32 attachment_index, i32 x, i32 y) = 0;
        virtual void _erase_attachment(u32 attachment_index, int value) = 0;
        virtual u32 _at(int) const = 0;
    
    protected:
        u32 m_id = 0;
        u32 m_width = 0U;
        u32 m_height = 0U;
        u32 m_samples = 1U;
        bool m_swap_chain_target = false;
        std::vector<attachment> m_formats;
    };

    template<typename... t_attachments>
    inline void framebuffer::attach(t_attachments&&... attachments)
    {
        m_formats = { std::forward<t_attachments>(attachments)... };
        
        for (auto& attachment : m_formats)
        {
            if (bool is_depth = attachment >= attachment::DEPTH)
            {
                UTD_ENGINE_ASSERT(m_formats.back() <= attachment, "framebuffer has more than two depth attachments");
                std::swap(attachment, m_formats.back());
            }
        }
        
        this->_attach();
    }
}