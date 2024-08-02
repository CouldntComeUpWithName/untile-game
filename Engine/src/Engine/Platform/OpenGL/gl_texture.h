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

        const specs& get_specs() override;

        u32 width() const override;
        u32 height() const override;
        glm::vec2 size() const override;
        const std::string& path() const override;
        void set_data(u32 slot) override;
        bool is_loaded() const override;
        bool operator==(const texture& other) const override;
    private:
        bool m_loaded;
        std::uptr<std::string> m_path;
        texture::specs m_specifications;
    };
}