#pragma once

//UTD
namespace utd
{
    class entity;
    class texture;
    class image;
    class shader;
    class window;
    class buffer;
    
    struct transform;
    struct line;
    struct circle;
    struct rectangle;
    struct triangle;
    struct sprite;
    
    class render_string;
    class multi_camera;
    class camera;
    class editor_camera;
}

//STD
namespace std
{
    template<class> class allocator;
    template<class> struct char_traits;
    
    template<class, class, class> 
    class basic_string;

    typedef basic_string<char, char_traits<char>, allocator<char>> string;
    
    template <class _Fty>
    class function;

    namespace filesystem
    {
        class path;
    }
}
