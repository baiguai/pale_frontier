#ifndef FILEIO_H
#define FILEIO_H

#include <filesystem>
#include <string>
#include <fstream>
#include "../libs/nlohmann/json.hpp"

using json = nlohmann::json;

// ---- Tools ------------------------------------------------------------------
inline bool fileExists(const std::string& path)
{
    std::ifstream in(path);
    return in.good();
}
// -----------------------------------------------------------------------------





// ---- Create Directory -------------------------------------------------------
inline bool createDirectories(const std::string& path)
{
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    return !ec;
}
// -----------------------------------------------------------------------------


// ---- Write JSON Entry -------------------------------------------------------
/*
    Examples:
    setJsonValue("saves/player.json", "position.x", 100.5);
    setJsonValue("saves/player.json", "position.y", 200.3);
    setJsonValue("saves/player.json", "health", 100);
    setJsonValue("saves/player.json", "weapons.laser", 5);
*/
inline bool saveJsonValue(const std::string& filePath, const std::string& jsonPath, const json& value)
{
    json j;

    // Load file if it exists
    std::ifstream in(filePath);
    if (in.good())
    {
        in >> j;
        in.close();
    }

    // Navigate/set the path (example: "player.position.x")
    json* current = &j;
    size_t start = 0, end;
    while ((end = jsonPath.find('.', start)) != std::string::npos)
    {
        std::string key = jsonPath.substr(start, end - start);
        if (!current->contains(key))
        {
            (*current)[key] = json::object();
        }
        current = &(*current)[key];
        start = end + 1;
    }
    (*current)[jsonPath.substr(start)] = value;

    // Write to the json file
    std::ofstream out(filePath);
    if (!out.good())
    {
        return false;
    }
    out << j.dump(2) << std::endl;
    return true;
}
// -----------------------------------------------------------------------------


// ---- Read JSON Entry --------------------------------------------------------
/*
    Examples:
    double x = getJsonValue("saves/player.json", "position.x", 0.0);
    int health = getJsonValue("saves/player.json", "health", 100);
    std::string name = getJsonValue("saves/player.json", "player.name", "unknown");
*/
template<typename T>
inline T readJsonValue(const std::string& filePath, const std::string& jsonPath, const T& defaultValue)
{
    std::ifstream in(filePath);
    if (!in.good())
    {
        return defaultValue;
    }

    json j;
    in >> j;

    // Navigate the json path
    json* current = &j;
    size_t start = 0, end;
    while ((end = jsonPath.find('.', start)) != std::string::npos)
    {
        std::string key = jsonPath.substr(start, end - start);
        if (!current->contains(key) || !(*current)[key].is_object())
        {
            return defaultValue;
        }
        current = &(*current)[key];
        start = end + 1;
    }

    return current->value(jsonPath.substr(start), defaultValue);
}
// -----------------------------------------------------------------------------

#endif
