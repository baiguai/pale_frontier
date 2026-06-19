#include "main.hpp"

int main(void)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);
    HideCursor();

    bool running {true };

    createDirectories("data/player");
    createDirectories("data/config");


    // ---- Move to a config load/save maybe -----------------------------------

    planet_distance = readJsonValue("data/config/space.json", "planets.distance", 25000);
    star_distance_01 = readJsonValue("data/config/space.json", "stars.lvl1.distance", 9000);
    star_distance_02 = readJsonValue("data/config/space.json", "stars.lvl2.distance", 3000);
    star_distance_03 = readJsonValue("data/config/space.json", "stars.lvl3.distance", 18000);

    // Restore the player's position
    camera.x = readJsonValue("data/player/stats.json", "game.location.x", 0);
    camera.y = readJsonValue("data/player/stats.json", "game.location.y", 0);

    // -------------------------------------------------------------------------

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


            // DRAW METHODS
            drawSpace();


            float moveAmount = speed * 60.0f * GetFrameTime();

            if (IsKeyDown(KEY_C))
            {
                camera.y += moveAmount;
            }
            if (IsKeyDown(KEY_E))
            {
                camera.y -= moveAmount;
            }
            if (IsKeyDown(KEY_S))
            {
                camera.x -= moveAmount;
            }
            if (IsKeyDown(KEY_F))
            {
                camera.x += moveAmount;
            }


// -----------------------------------------------------------------------------

        EndDrawing();
    }


    // ---- Move to a config load/save maybe -----------------------------------

    saveJsonValue("data/config/space.json", "planets.distance", planet_distance);
    saveJsonValue("data/config/space.json", "stars.lvl1.distance", star_distance_01);
    saveJsonValue("data/config/space.json", "stars.lvl2.distance", star_distance_02);
    saveJsonValue("data/config/space.json", "stars.lvl3.distance", star_distance_03);

    // Store the player's position
    saveJsonValue("data/player/stats.json", "game.location.x", static_cast<int>(floor(camera.x)));
    saveJsonValue("data/player/stats.json", "game.location.y", static_cast<int>(floor(camera.y)));

    // -------------------------------------------------------------------------


    CloseWindow();
    return 0;
}

void drawSpace()
{
    drawStars();
    drawPlanets();
}
