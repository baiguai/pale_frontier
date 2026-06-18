#include "stars.hpp"

void drawStars()
{
    const Color star_color = Color{ 30, 30, 30, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_01;
    int sec_num_y = GetScreenHeight() / star_sector_size_01;

    float radius = 1;

    for (int x = 0; x < sec_num_x; x++)
    {
        for (int y = 0; y < sec_num_y; y++)
        {
            Point global_sector = Point{static_cast<int>(camera.x + x), static_cast<int>(camera.y + y)};

            frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

            if (frand.randInteger(0, star_distance_01) == 1)
            {
                DrawCircle((x * star_sector_size_01) + radius, (y * star_sector_size_01) + radius, radius, star_color);
            }
        }
    }

    drawStars2();
}

void drawStars2()
{
    const Color star_color = Color{ 60, 60, 60, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_02;
    int sec_num_y = GetScreenHeight() / star_sector_size_02;

    float radius = 1;

    for (int x = 0; x < sec_num_x; x++)
    {
        for (int y = 0; y < sec_num_y; y++)
        {
            Point global_sector = Point{static_cast<int>(camera.x + x), static_cast<int>(camera.y + y)};

            frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

            if (frand.randInteger(0, star_distance_02) == 1)
            {
                DrawCircle((x * star_sector_size_02) + radius, (y * star_sector_size_02) + radius, radius, star_color);
            }
        }
    }

    drawStars3();
}

void drawStars3()
{
    const Color star_color = Color{ 110, 110, 110, 255 };

    int sec_num_x = GetScreenWidth() / star_sector_size_03;
    int sec_num_y = GetScreenHeight() / star_sector_size_03;

    float radius = frand.randInteger(0, 3);

    for (int x = 0; x < sec_num_x; x++)
    {
        for (int y = 0; y < sec_num_y; y++)
        {
            Point global_sector = Point{static_cast<int>(camera.x + x), static_cast<int>(camera.y + y)};

            frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);

            if (frand.randInteger(0, star_distance_03) == 1)
            {
                DrawCircle((x * star_sector_size_03) + radius, (y * star_sector_size_03) + radius, radius, star_color);
            }
        }
    }
}
