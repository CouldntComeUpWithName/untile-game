#pragma once 
#include<Engine/Core/Base.h>
#include "shader.h"

namespace utd
{
    struct vertex_attrib
    {
        shader::data_type datatype;
        std::string name;
        u16 size;
        u16 offset;
        bool normalized;

        vertex_attrib(shader::data_type type, std::string name, bool normalized = false)
            : datatype(type), name(name), size(shader::data_type_size_of(datatype)), offset(0), normalized(normalized)
        { }
        
        // TODO: come up with a better name
        inline u32 count() const
        {
            auto result = static_cast<u32>(datatype) % 4;
            return (result ? result : 4);
        }

    };

} /* namespace utd */