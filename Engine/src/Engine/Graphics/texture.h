#pragma once
#include <string>
#include <glm/vec2.hpp>

#include <Engine/Core/Base.h>
#include <Engine/Core/Log.h>

#include <string>
namespace utd
{
    enum class image_format
    {
        NONE = 0,
        R8,
        RGB8,
        RGBA8,
        RGBA32F
    };

    class texture : noncopyable
    {
        using id = u32;
    public:
        struct specs
        {
            i32 width           = 1;
            i32 height          = 1;
            image_format format = image_format::RGBA8;
            bool mips           = true;
        };

    public:
        virtual ~texture()
        {
            UTD_ENGINE_DEBUG(__FUNCTION__ " for resource ID{0} {1}", m_id, "has been called");
        }
        texture() = default;
        virtual const specs& get_specs() = 0;
        virtual u32 width() const        = 0;
        virtual u32 height() const       = 0;
        virtual glm::vec2 size() const   = 0;
        
        virtual void bind(u32 slot = 0) = 0;

        virtual const std::string& path() const = 0;

        virtual void set_data(void* data, u32 slot = 0) = 0;
        virtual bool is_loaded() const = 0;
        inline id get_id() const { return m_id; }
        virtual bool operator==(const texture& other) const = 0;
    public:
        static std::uptr<texture> load(const std::string& filepath);
        static std::uptr<texture> create();
    private:
        // virtual void release() = 0;
    protected:
        id m_id = 0;
    };

    
    
} /* namespace utd */