#include "main.hpp"
#include "global.hpp"
#include "planets.hpp"
#include "stars.hpp"
#include "fileio.hpp"

int main(void)
{
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);

    bool running {true };

    createDirectories("data/player");

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


    CloseWindow();
    return 0;
}

void drawSpace()
{
    drawStars();
    drawPlanets();
}
