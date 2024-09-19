#pragma once
#include<Engine/Core/Base.h>
#include <Engine/Core/Buffer.h>
namespace utd
{
    class image : noncopyable
    {
    public:
        enum class format
        {
            //        size of pixel/texel
            NONE = sizeof(byte) * 0,
            R8 = sizeof(byte) * 1,
            RGB8 = sizeof(byte) * 3,
            RGBA8 = sizeof(byte) * 4,
            RGBA32F = sizeof(float) * 4
        };
        struct specs
        {
            u32 width = 1;
            u32 height = 2;
            format format = format::RGBA8;
        };
        static format retrive_format(int channels);
    public:
        image() = default;
        image(const specs specifications);
        image(const buffer& buffer);
        image(buffer&& buffer);

        virtual void source(const buffer& buffer) = 0;
        virtual void filepath(const std::filesystem::path&) = 0;

        virtual ~image() = default;

        void clear_buffer() { m_data.clear(); }
        void set_data(const buffer& buffer);
        
        virtual void reload() {}
        
        buffer& data() { return m_data; } 
        byte* raw_data() { return m_data.data(); }
        
        const buffer& data() const { return m_data; }
        const byte* raw_data() const { return m_data.data(); }

        const std::string& path() { return *m_path.get(); }
        inline u32 width() const { return m_specs.width; }
        inline u32 height() const { return m_specs.height; }
        format get_format() const { return m_specs.format; };
    public:
        static std::uptr<image> load(const std::filesystem::path& path);
        static std::uptr<image> create(const buffer& source);
    protected:
        buffer m_data;
        specs m_specs;
        std::uptr<std::string> m_path;
    };
}