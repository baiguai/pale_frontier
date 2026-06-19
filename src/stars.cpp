#include "stars.hpp"

void drawStars()
{
    const Color star_color = Color{ 30, 30, 30, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_01;
    int sec_num_y = GetScreenHeight() / star_sector_size_01;

    float radius = 1;

    renderStars(star_color, star_sector_size_01, sec_num_x, sec_num_y, radius);

    drawStars2();
}

void drawStars2()
{
    const Color star_color = Color{ 60, 60, 60, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_02;
    int sec_num_y = GetScreenHeight() / star_sector_size_02;

    float radius = 1;

    renderStars(star_color, star_sector_size_02, sec_num_x, sec_num_y, radius);

    drawStars3();
}

void drawStars3()
{
    const Color star_color = Color{ 110, 110, 110, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_03;
    int sec_num_y = GetScreenHeight() / star_sector_size_03;

    float radius = frand.randInteger(0, 3);

    renderStars(star_color, star_sector_size_03, sec_num_x, sec_num_y, radius);
}

void renderStars(Color star_color, int sector_size, int sec_num_x, int sec_num_y, float radius)
{
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

            if (frand.randInteger(0, star_distance_01) == 1)
            {
                float draw_x = (x - cam_x_frac) * sector_size + radius;
                float draw_y = (y - cam_y_frac) * sector_size + radius;
                DrawCircle(draw_x, draw_y, radius, star_color);
            }
        }
    }
}
