#ifndef FILEIO_H
#define FILEIO_H

#include <filesystem>
#include <string>

inline bool createDirectories(const std::string& path)
{
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    return !ec;
}

#endif
