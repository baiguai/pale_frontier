#include <vector>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include "items.hpp"
namespace fs = std::filesystem;

namespace space_loop
{
    std::vector<Texture2D> itemTextures;

    void loadItemTextures()
    {
        itemTextures.clear();
        std::string dir = "data/space/assets/items/";
        if (!fs::exists(dir))
        {
            std::cout << "ERROR:: Item assets DIR does not exist.\n";
            return;
        }

        std::vector<std::string> files;
        for (const auto& entry : fs::directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                auto path = entry.path().string();
                if (path.ends_with(".png"))
                {
                    files.push_back(path);
                }
            }
        }

        std::sort(files.begin(), files.end());

        for (const auto& path : files)
        {
            itemTextures.push_back(LoadTexture(path.c_str()));
        }
    }

    void unloadItemTextures()
    {
        for (auto& tex : itemTextures)
        {
            UnloadTexture(tex);
        }
        itemTextures.clear();
    }

    GameScreen drawItems(int sectorX, int sectorY, float drawX, float drawY, const Point& globalSector)
    {
        if (itemTextures.empty()) return GameScreen::SPACE;

        int texIndex = frand.randInteger(0, itemTextures.size());
    }
}
