#pragma once

#include <Engine/Graphics/texture.h>
#include <string>

namespace utd
{
    class gl_texture : public texture
    {

    public:
        gl_texture() = default;
        gl_texture(const specs&);
        gl_texture(const std::string& filepath);
        ~gl_texture() override;

        const specs& get_specs() override;

        u32 width() const override;
        u32 height() const override;
        glm::vec2 size() const override;
        const std::string& path() const override;
        void set_data(void* data, u32 size) override;
        void set_data(const image&) override;
        bool is_loaded() const override;
        bool operator==(const texture& other) const override;

        void bind(u32 slot) override;
        
    private:
        bool m_loaded = false;
        std::uptr<std::string> m_path = nullptr;
        texture::specs m_specifications = {};
    };
}