#include "main.hpp"

int main(void)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);
    HideCursor();

    bool running {true };

    createDirectories("data/player");

    // Restore the player's position
    camera.x = readJsonValue("data/player/stats.json", "game.location.x", 0);
    camera.y = readJsonValue("data/player/stats.json", "game.location.y", 0);

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

            if (IsKeyDown(KEY_D))
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

    // Store the player's position
    saveJsonValue("data/player/stats.json", "game.location.x", static_cast<int>(floor(camera.x)));
    saveJsonValue("data/player/stats.json", "game.location.y", static_cast<int>(floor(camera.y)));


    CloseWindow();
    return 0;
}

void drawSpace()
{
    drawStars();
    drawPlanets();
}
