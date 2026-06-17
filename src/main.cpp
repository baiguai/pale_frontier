#include "main.hpp"

int main(void)
{
    InitWindow(900, 800, "Pale Frontier");
    SetTargetFPS(75);

    bool running {true };

    while (running)
    {
        if (IsKeyPressed(KEY_Q))
        {
            running = false;
            break;
        }

        BeginDrawing();

            ClearBackground(BLACK);

// ---- Move this into the Playing Game UI Loop --------------------------------

            Frand::FastRand frand{};

            Point camera{ 0, 0 };

            const int sector_size { 32 };
            const Color line_color = Color{ 105, 105, 105, 255 };
            const Color planet_color_01 = Color{ 174, 187, 213, 255 };

            int sec_num_x = GetScreenWidth() / sector_size;
            int sec_num_y = GetScreenHeight() / sector_size;

            float radius = sector_size / 2.0;

            for (size_t x = 0; x < sec_num_x; x++)
            {
                DrawLine(x * sector_size, 0, x * sector_size, GetScreenHeight(), line_color);

                for (size_t y = 0; y < sec_num_y; y++)
                {
                    DrawLine(0, y * sector_size, GetScreenWidth(), y * sector_size, line_color);

                    Point global_sector = Point{static_cast<int>(camera.x + x), static_cast<int>(camera.y + y)};

                    DrawCircle((x * sector_size) + radius, (y * sector_size) + radius, radius, planet_color_01);
                }
            }


// -----------------------------------------------------------------------------

        EndDrawing();
    }


    CloseWindow();
    return 0;
}
