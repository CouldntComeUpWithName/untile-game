#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Utils/object_traits.h"

#include <string_view>

#include <glm/glm.hpp>

namespace std::filesystem
{
    class path;
} /* namespace std::filesystem */

namespace utd
{
    class shader
    {
    public:
        
        enum base_type
        {
            UNKNOWN        = 0,
            BOOLEAN        = 1,
            FLOATING_POINT = 2,
            INTEGER        = 3,
            MATRIX         = 4,
            COUNT          = 5
        };

        typedef enum class data_type
        {
            /* base_type::UNKNOWN */
            UNDEFINED = 0,
            
            /* base_type::BOOLEAN */
            BOOL      = 1,

            /* base_type::FLOATING_POINT */
            FLOAT     = 5, 
            FLOAT2    = 6, 
            FLOAT3    = 7, 
            FLOAT4    = 8, 
            
            /* base_type::INTEGER */
            INT       = 9, 
            INT2      = 10, 
            INT3      = 11,
            INT4      = 12, 
            
            /* base_type::MATRIX */
            MAT3      = 15, 
            MAT4      = 16
        } datatype;



        //TODO: maybe a better name
        static constexpr int BASE_TYPE_MAX_SIZE = sizeof(int) | sizeof(float);
        
        //TODO: improve if possible
        static int data_type_size_of(data_type type)
        {
            switch (type)
            {
            case datatype::BOOL:   return sizeof(bool);
            case datatype::FLOAT:  return sizeof(float);
            case datatype::FLOAT2: return sizeof(float) * 2;
            case datatype::FLOAT3: return sizeof(float) * 3;
            case datatype::FLOAT4: return sizeof(float) * 4;
            case datatype::INT:    return sizeof(int);
            case datatype::INT2:   return sizeof(int) * 2;
            case datatype::INT3:   return sizeof(int) * 3;
            case datatype::INT4:   return sizeof(int) * 4;
            case datatype::MAT3:   return sizeof(float) * 3 * 3;
            case datatype::MAT4:   return sizeof(float) * 4 * 4;

            }

            return 0;
        }

    public:
        using id = u32;

    public:
        shader() = default;
        virtual ~shader() = default;
        
        virtual void bind()   const = 0;
        virtual void unbind() const = 0;

        virtual void integer(const std::string_view name, int value)            const = 0;
        virtual void array(const std::string_view name, int* values, u32 count) const = 0;
        virtual void real(const std::string_view name, float value)             const = 0;
        virtual void vec2(const std::string_view name, const glm::vec2& value)  const = 0;
        virtual void vec3(const std::string_view name, const glm::vec3& value)  const = 0;
        virtual void vec4(const std::string_view name, const glm::vec4& value)  const = 0;
        virtual void mat3(const std::string_view name, const glm::mat3& matrix) const {/* TODO: get this implemented for derived classes */ }
        virtual void mat4(const std::string_view name, const glm::mat4& matrix) const = 0;
        
        virtual void source(const std::string&)   = 0;
        virtual void filepath(const std::string&) = 0;

        virtual void source(const std::string&, const std::string&)   = 0;
        
        virtual void filepath(const std::string&, const std::string&) = 0;

        inline id get_id() const { return m_id; }

    public:
        static std::uptr<shader> create(const std::string& vertex, const std::string& fragment);
        static std::uptr<shader> create(const std::string& source);

        static std::uptr<shader> load(const std::string& filepath);
        static std::uptr<shader> load(const std::string& vertex_path, const std::string& frag_path);
        
    protected:
        id m_id;
    };

    class shader_compiler
    {
    public:

    public:
        shader_compiler() = default;
        shader_compiler(const std::string& vertex_src, const std::string& fragment_src);

        //void filepath(std::filesystem::path& vertex_path, std::filesystem::path& fragment_path);
        void source(const std::string& vertex_src, const std::string& fragment_src);
        
        virtual shader::id build() = 0;
        virtual void compile()     = 0;
        virtual void link()        = 0;
    private:
        shader::id m_program_id = -1;
        
        u32 m_vertex_module_id;
        u32 m_fragment_module_id;
        
        std::string m_vertex_source;
        std::string m_fragment_source;
    };

} /* namespace utd */