#include "main.hpp"
#include "global.hpp"
#include "planets.hpp"
#include "stars.hpp"

int main(void)
{
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);

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


            // DRAW METHODS
            drawSpace();


            if (IsKeyDown(KEY_D))
            {
                camera.y += 1; // KEYPAD_SENSE - 0.15f;
            }
            if (IsKeyDown(KEY_E))
            {
                camera.y -= 1;
            }
            if (IsKeyDown(KEY_S))
            {
                camera.x -= 1;
            }
            if (IsKeyDown(KEY_F))
            {
                camera.x += 1;
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
