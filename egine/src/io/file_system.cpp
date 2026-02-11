#include "file_system.h"

#include "config.h"
#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif // __Win32

std::filesystem::path engine::FileSystem::GetExecutablePath() const
{
#ifdef _WIN32
    // Windows 平台实现
    wchar_t buf[MAX_PATH];
    if (GetModuleFileNameW(NULL, buf, MAX_PATH) > 0)
    {
        return std::filesystem::path(buf).remove_filename();
    }
    return std::filesystem::current_path();

#elif defined(__APPLE__)
    // macOS 平台实现
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string tmp(size, '\0');
    if (_NSGetExecutablePath(tmp.data(), &size) == 0)
    {
        return std::filesystem::weakly_canonical(std::filesystem::path(tmp)).remove_filename();
    }
    return std::filesystem::current_path();

#elif defined(__linux__)
    // Linux 平台实现
    try
    {
        return std::filesystem::weakly_canonical(std::filesystem::read_symlink("/proc/self/exe")).remove_filename();
    }
    catch (...)
    {
        return std::filesystem::current_path();
    }

#else
	return std::filesystem::current_path();
#endif
}

std::filesystem::path engine::FileSystem::GetAssetsPath() const
{
#ifdef ASSETS_ROOT
    // 如果预定义了 ASSETS_ROOT，优先使用这个路径
    auto path = std::filesystem::path(std::string(ASSETS_ROOT));
    if (std::filesystem::exists(path))
    {
		spdlog::info("Using ASSETS_ROOT path: {}", path.string());
        return path;
    }
    else
     {
        spdlog::warn("ASSETS_ROOT path does not exist: {}", path.string());
	}

#endif
    path = std::filesystem::weakly_canonical(GetExecutablePath() / "assets");
	spdlog::error("Using default assets path based on executable location:{}",path.string());
    return path;
}
