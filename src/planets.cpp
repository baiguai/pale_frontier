#include "planets.hpp"

void drawPlanets()
{
    const Color planet_color_01 = Color{ 174, 187, 213, 255 };

    int sec_num_x = GetScreenWidth() / planet_sector_size;
    int sec_num_y = GetScreenHeight() / planet_sector_size;

    float radius = planet_sector_size * 3.0;

    int cam_x_int = static_cast<int>(floor(camera.x));
    int cam_y_int = static_cast<int>(floor(camera.y));
    float cam_x_frac = camera.x - floor(camera.x);
    float cam_y_frac = camera.y - floor(camera.y);

    for (int x = 0; x < sec_num_x; x++)
    {
        for (int y = 0; y < sec_num_y; y++)
        {
            Point global_sector { cam_x_int + x, cam_y_int + y };

            frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

            if (frand.randInteger(0, planet_distance) == 1)
            {
                DrawCircle((x * planet_sector_size) + radius, (y * planet_sector_size) + radius, radius, planet_color_01);

                float draw_x = (x - cam_x_frac) * planet_sector_size + radius;
                float draw_y = (y - cam_y_frac) * planet_sector_size + radius;
                DrawCircle(draw_x, draw_y, radius, planet_color_01);
            }
        }
    }
}
