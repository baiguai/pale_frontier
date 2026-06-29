#include <vector>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include "planets.hpp"
namespace fs = std::filesystem;

namespace space_loop
{
    std::vector<Texture2D> planetTextures;

    void loadPlanetTextures()
    {
        planetTextures.clear();
        std::string dir = "data/space/assets/planets/";
        if (!fs::exists(dir))
        {
            std::cout << "ERROR:: Planets assets DIR does not exist.\n";
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

    GameScreen drawPlanets()
    {
        if (planetTextures.empty())
        {
            std::cout << "ERROR:: Planet textures are empty.\n";
            return GameScreen::SPACE;
        }

        int sec_num_x = GetScreenWidth() / planet_sector_size;
        int sec_num_y = GetScreenHeight() / planet_sector_size;

        int cam_x_int = static_cast<int>(floor(space_camera.x));
        int cam_y_int = static_cast<int>(floor(space_camera.y));
        float cam_x_frac = space_camera.x - floor(space_camera.x);
        float cam_y_frac = space_camera.y - floor(space_camera.y);

        for (int x = 0; x < sec_num_x; x++)
        {
            for (int y = 0; y < sec_num_y; y++)
            {
                Point global_sector { cam_x_int + x, cam_y_int + y };

                frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y, game_init_seed);

                if (frand.randInteger(0, planet_distance) == 1)
                {
                    if (frand.randInteger(0, item_distance) == 1)
                    {
                        return GameScreen::SPACE;
                    }

                    int texIndex = frand.randInteger(0, planetTextures.size());

                    float draw_x = (x - cam_x_frac) * planet_sector_size;
                    float draw_y = (y - cam_y_frac) * planet_sector_size;

                    DrawTexture(planetTextures[texIndex], draw_x, draw_y, WHITE);

                    float centerX = GetScreenWidth() / 2.0f;
                    float centerY = GetScreenHeight() / 2.0f;
                    float texW = planetTextures[texIndex].width;
                    float texH = planetTextures[texIndex].height;
                
                    // Check if the center of the screen is within the planet's texture bounds
                    if (draw_x <= centerX && centerX <= draw_x + texW &&
                        draw_y <= centerY && centerY <= draw_y + texH)
                    {
                        std::cout << "\n\n";
                        std::cout << "Landed on the planet: " << global_sector.x << ", " << global_sector.y << "\n\n";
                        std::cout << "\"game\": {\n";
                        std::cout << std::setw(2) << std::setfill(' ') << "\"location\": {\n";
                        std::cout << std::setw(4) << std::setfill(' ') << "\"x\": " << (global_sector.x - 20) << ",\n";
                        std::cout << std::setw(4) << std::setfill(' ') << "\"y\": " << global_sector.y << "\n";
                        std::cout << std::setw(2) << std::setfill(' ') << "}\n";
                        std::cout << "},\n\n";

                        space_camera.x = space_camera.x - planet_sector_size;
                        configurePlanet(global_sector.x, global_sector.y);
                        return GameScreen::SURFACE;
                    }
                }
            }
        }

        return GameScreen::SPACE;
    }


    // ---- Configuration ------------------------------------------------------

    void configurePlanet(int sector_x, int sector_y)
    {
        current_planet.x = sector_x;
        current_planet.y = sector_y;
        setCurrentPlanet();
    }

    //--------------------------------------------------------------------------
}
