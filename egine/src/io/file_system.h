#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

namespace engine
{
	class FileSystem
	{
	public:
		std::filesystem::path GetExecutablePath() const;
		std::filesystem::path GetAssetsPath()const;

		std::vector<char> LoadFile(const std::filesystem::path& filePath) ;
		std::vector<char> LoadAssetsFile(const std::string& filePath);
		std::string LoadAssetsFileText(const std::string& filePath);
	};
}