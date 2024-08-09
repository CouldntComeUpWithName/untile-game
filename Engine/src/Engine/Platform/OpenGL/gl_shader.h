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

        void set_int(const std::string_view name, int value) override;

        void set_array(const std::string_view name, int* values, u32 count) override;

        void set_float(const std::string_view name, float value) override;

        void set_float2(const std::string_view name, const glm::vec2& value) override;

        void set_float3(const std::string_view name, const glm::vec3& value) override;

        void set_float4(const std::string_view name, const glm::vec4& value) override;

        void set_mat4(const std::string_view name, const glm::mat4& value) override;

        void source(const std::string&) override;

        void filepath(const std::string&) override;

        void source(const std::string&, const std::string&) override;

        void filepath(const std::string&, const std::string&) override;

    };
};
