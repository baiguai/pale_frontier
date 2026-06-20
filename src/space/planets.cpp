#include <vector>
#include <filesystem>
#include <algorithm>
#include "planets.hpp"
namespace fs = std::filesystem;

namespace space_loop
{
    std::vector<Texture2D> planetTextures;

    void loadPlanetTextures()
    {
        planetTextures.clear();
        std::string dir = "data/space/assets/";
        if (!fs::exists(dir))
        {
            std::cout << "ERROR:: Assets DIR does not exist.\n";
            return;
        }

        std::vector<std::string> files;
        for (const auto& entry : fs::directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                auto path = entry.path().string();
                if (path.find("planet_") && path.ends_with(".png"))
                {
                    files.push_back(path);
                }
            }
        }

        std::sort(files.begin(), files.end());

        for (const auto& path : files)
        {
            planetTextures.push_back(LoadTexture(path.c_str()));
        }
    }

    void unloadPlanetTextures()
    {
        for (auto& tex : planetTextures)
        {
            UnloadTexture(tex);
        }
        planetTextures.clear();
    }

    void drawPlanets()
    {
        if (planetTextures.empty())
        {
            std::cout << "ERROR:: Planet textures are empty.\n";
            return;
        }

        const Color planet_color_01 = Color{ 174, 187, 213, 255 };

        int sec_num_x = GetScreenWidth() / planet_sector_size;
        int sec_num_y = GetScreenHeight() / planet_sector_size;

        float radius = planet_sector_size * 3.0;

        int cam_x_int = static_cast<int>(floor(space_camera.x));
        int cam_y_int = static_cast<int>(floor(space_camera.y));
        float cam_x_frac = space_camera.x - floor(space_camera.x);
        float cam_y_frac = space_camera.y - floor(space_camera.y);

        for (int x = 0; x < sec_num_x; x++)
        {
            for (int y = 0; y < sec_num_y; y++)
            {
                Point global_sector { cam_x_int + x, cam_y_int + y };

                frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

                if (frand.randInteger(0, planet_distance) == 1)
                {
                    int texIndex = frand.randInteger(0, planetTextures.size());

                    float draw_x = (x - cam_x_frac) * planet_sector_size;
                    float draw_y = (y - cam_y_frac) * planet_sector_size;

                    DrawTexture(planetTextures[texIndex], draw_x, draw_y, WHITE);
                }
            }
        }
    }
}
