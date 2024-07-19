#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Utils/object_traits.h"

#include <string_view>

#include <glm/glm.hpp>

namespace utd
{
    class shader
    {
    public:
        using id = u32;
    public:
        shader() = default;
        virtual ~shader() = default;

        virtual void bind()   = 0;
        virtual void unbind() = 0;

        virtual void set_int(const std::string_view name, int value)                 = 0;
        virtual void set_array(const std::string_view name, int* values, u32 count)  = 0;
        virtual void set_float(const std::string_view name, float value)             = 0;
        virtual void set_float2(const std::string_view name, const glm::vec2& value) = 0;
        virtual void set_float3(const std::string_view name, const glm::vec3& value) = 0;
        virtual void set_float4(const std::string_view name, const glm::vec4& value) = 0;
        virtual void set_mat4(const std::string_view name, const glm::mat4& matrix)  = 0;
        
        virtual void source(const std::string&)   = 0;
        virtual void filepath(const std::string&) = 0;

        virtual void source(const std::string&, const std::string&)   = 0;
        virtual void filepath(const std::string&, const std::string&) = 0;

        inline id get_id(){ return m_id; }
    public:
        static std::uptr<shader> create(const std::string& vertex, const std::string& fragment);
        static std::uptr<shader> create(const std::string& source);

        static std::uptr<shader> load(const std::string& filepath);
        static std::uptr<shader> load(const std::string& vertex_path, const std::string& frag_path);

    protected:
        id m_id;
    };

    class shader_compiler : utility
    {

    };

    class shader_pack
    {

    };

} /* namespace utd */