#include "main.hpp"

//// NOTES /////////////////////////////////////////////////////////////////////
//
// __| Procedural Notes |__
//
// * Build everything off of the Frand.
// * Each screen needs its own global variables, such as camera, speed, etc.
// * Those are all stored in the config.
// * To do so - consistently seed it and do random things off of that:
//      int cam_x_int = static_cast<int>(floor(space_camera.x));
//      int cam_y_int = static_cast<int>(floor(space_camera.y));
//      Point global_sector { cam_x_int + x, cam_y_int + y };
//      frand.seed = Frand::PerfectHash(global_sector.x, global_sector.y);
//
//  * Planet texture indexes must be tied to the surface seed.
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);
    HideCursor();

    bool running {true };

    createConfigDirectories();

    loadVarsFromConfig();


    while (running)
    {
        if (IsKeyPressed(KEY_Q))
        {
            running = false;
            break;
        }

        switch (currentScreen)
        {
            case GameScreen::SPACE:
                currentScreen = space_loop::runGameLoop();
                break;
        }

        if (currentScreen == GameScreen::QUIT)
        {
            running = false;
            break;
        }
    }


    saveVarsFromConfig();


    CloseWindow();
    return 0;
}

