#pragma once

#include <filesystem>


namespace engine
{
	class FileSystem
	{
	public:
		std::filesystem::path GetExecutablePath() const;
		std::filesystem::path GetAssetsPath()const;
	};
}