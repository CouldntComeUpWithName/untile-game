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
        enum class data_type
        {
            /* UNKNOWN */
            UNDEFINED = 0,
            
            /* BOOLEAN */
            BOOL      = 1,

            /* FLOATING_POINT */
            FLOAT     = 5, 
            FLOAT2    = 6, 
            FLOAT3    = 7, 
            FLOAT4    = 8, 
            
            /* INTEGER */
            INT       = 9, 
            INT2      = 10, 
            INT3      = 11,
            INT4      = 12, 
            
            /* MATRIX */
            MAT3      = 15, 
            MAT4      = 16
        };

        enum base_type
        {
            UNKNOWN        = 0,
            BOOLEAN        = 1,
            FLOATING_POINT = 2,
            INTEGER        = 3,
            MATRIX         = 4,
            COUNT          = 5
        };

        //TODO: maybe a better name
        static constexpr int BASE_TYPE_MAX_SIZE = sizeof(int) | sizeof(float);
        
        //TODO: improve if possible
        static int data_type_size_of(data_type type)
        {
            switch (type)
            {
            case data_type::BOOL:   return sizeof(bool);
            case data_type::FLOAT:  return sizeof(float);
            case data_type::FLOAT2: return sizeof(float) * 2;
            case data_type::FLOAT3: return sizeof(float) * 3;
            case data_type::FLOAT4: return sizeof(float) * 4;
            case data_type::INT:    return sizeof(int);
            case data_type::INT2:   return sizeof(int) * 2;
            case data_type::INT3:   return sizeof(int) * 3;
            case data_type::INT4:   return sizeof(int) * 4;
            case data_type::MAT3:   return sizeof(float) * 3 * 3;
            case data_type::MAT4:   return sizeof(float) * 4 * 4;

            }

            return 0;
        }
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

} /* namespace utd */