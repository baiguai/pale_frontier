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
            std::cout << "ERROR:: Items assets DIR does not exist.\n";
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

    GameScreen drawItems()
    {
        if (itemTextures.empty())
        {
            std::cout << "ERROR:: Items textures are empty.\n";
            return GameScreen::SPACE;
        }

        int sec_num_x = GetScreenWidth() / item_sector_size;
        int sec_num_y = GetScreenHeight() / item_sector_size;

        int cam_x_int = static_cast<int>(floor(space_camera.x));
        int cam_y_int = static_cast<int>(floor(space_camera.y));
        float cam_x_frac = space_camera.x - floor(space_camera.x);
        float cam_y_frac = space_camera.y - floor(space_camera.y);

        for (int x = 0; x < sec_num_x; x++)
        {
            for (int y = 0; y < sec_num_y; y++)
            {
                Point global_sector { cam_x_int + x, cam_y_int + y };

                bool alreadyTaken = false;
                for (const auto& t : taken_items)
                    if (t.first == global_sector.x && t.second == global_sector.y) { alreadyTaken = true; break; }
                if (alreadyTaken) continue;

                frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y, game_init_seed);

                if (frand.randInteger(0, item_distance) == 1)
                {
                    int texIndex = frand.randInteger(0, itemTextures.size());

                    float draw_x = (x - cam_x_frac) * item_sector_size;
                    float draw_y = (y - cam_y_frac) * item_sector_size;

                    DrawTexture(itemTextures[texIndex], draw_x, draw_y, WHITE);

                    float centerX = GetScreenWidth() / 2.0f;
                    float centerY = GetScreenHeight() / 2.0f;
                    float texW = itemTextures[texIndex].width;
                    float texH = itemTextures[texIndex].height;
                
                    // Check if the center of the screen is within the item's texture bounds
                    if (draw_x <= centerX && centerX <= draw_x + texW &&
                        draw_y <= centerY && centerY <= draw_y + texH)
                    {
                        std::cout << "\n\n";
                        std::cout << "Taken Item\n\n";

                        configureItem(global_sector.x, global_sector.y, texIndex);

                        space_camera.x = space_camera.x - item_sector_size;
                    }
                }
            }
        }

        return GameScreen::SPACE;
    }

    // ---- Configuration ------------------------------------------------------

    void configureItem(int sector_x, int sector_y, int texIndex)
    {
        taken_items.push_back({sector_x, sector_y});
        saveTakenItems();

        std::string itemPath = "data/space/items/item_" + std::to_string(texIndex) + ".json";
        if (fileExists(itemPath))
        {
            json j;
            std::ifstream in(itemPath);
            in >> j;

            for (auto& [action, data] : j.items())
            {
                if (action == "refuel")
                {
                    int value = data.value("value", 0);
                    player_space_fuel += value;
                    if (player_space_fuel > 100)
                        player_space_fuel = 100;
                }
            }
        }
    }

    //--------------------------------------------------------------------------
}
