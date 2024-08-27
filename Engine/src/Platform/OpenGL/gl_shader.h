#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Graphics/shader.h>

namespace utd
{
    class gl_shader : public shader
    {
    public:
        gl_shader()
            : shader()
        { }

        // Inherited via shader
        void bind() override;

        void unbind() override;

        void integer(const std::string_view name, int value) override;

        void array(const std::string_view name, int* values, u32 count) override;
        void real(const std::string_view name, float value)             override;
        void vec2(const std::string_view name, const glm::vec2& value)  override;
        void vec3(const std::string_view name, const glm::vec3& value)  override;
        void vec4(const std::string_view name, const glm::vec4& value)  override;
        void mat4(const std::string_view name, const glm::mat4& value)  override;

        void source(const std::string&) override;

        void filepath(const std::string&) override;

        void source(const std::string&, const std::string&) override;

        void filepath(const std::string&, const std::string&) override;
    private:
        void _filepath(const std::filesystem::path&, const std::filesystem::path&);
    };
};
