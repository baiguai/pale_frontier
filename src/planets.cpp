#include "planets.hpp"

void drawPlanets()
{
    const Color planet_color_01 = Color{ 174, 187, 213, 255 };

    int sec_num_x = GetScreenWidth() / planet_sector_size;
    int sec_num_y = GetScreenHeight() / planet_sector_size;

    float radius = planet_sector_size * 3.0;

    for (int x = 0; x < sec_num_x; x++)
    {
        for (int y = 0; y < sec_num_y; y++)
        {
            Point global_sector = Point{static_cast<int>(camera.x + x), static_cast<int>(camera.y + y)};

            frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

            if (frand.randInteger(0, planet_distance) == 1)
            {
                DrawCircle((x * planet_sector_size) + radius, (y * planet_sector_size) + radius, radius, planet_color_01);
            }
        }
    }
}
