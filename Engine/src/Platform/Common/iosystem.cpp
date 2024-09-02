#include <upch.h>
#include "iosystem.h"

#include <stb_image.h>

#include <Engine/Core/Buffer.h>
#include <Engine/Profiling/profile.h>

utd::buffer utd::io::read_file(const std::filesystem::path& filepath, std::ios_base::openmode mode)
{
    UTD_PROFILE_FUNC(utd::profile::color::deepgreen);

	std::ifstream file;
	file.open(filepath, mode);
	buffer source;

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		source.resize(size);
		
		file.read(source.as<char>(), size);
	}

	return source;
}
