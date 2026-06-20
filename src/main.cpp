#include "main.hpp"

int main(void)
{
//    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(window_width, window_height, title);
    SetTargetFPS(60);
    HideCursor();

    bool running {true };

    createDirectories("data/player");
    createDirectories("data/config");


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

