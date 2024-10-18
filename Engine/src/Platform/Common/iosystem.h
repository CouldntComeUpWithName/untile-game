#pragma once

#include <Engine/Core/Base.h>
#include <fstream>

namespace utd::io
{
    buffer read_file(const std::filesystem::path&, std::ios_base::openmode mode);
    buffer load_image(const std::filesystem::path&);
}