#ifndef CONFIG_H
#define CONFIG_H

inline void createConfigDirectories()
{
    createDirectories("data/player");
    createDirectories("data/config");
    createDirectories("data/space/planets");
}

inline void loadVarsFromConfig()
{
    std::string_view t { "SPACE" };
    std::string_view scr = readJsonValue("data/config/game.json", "game.screen", t);

    if (scr == "SPACE")
        currentScreen = GameScreen::SPACE;
    else if (scr == "SURFACE")
        currentScreen = GameScreen::SURFACE;
    else
        currentScreen = GameScreen::SPACE; // Later this may be the menu or similar.

    planet_distance = readJsonValue("data/config/space.json", "planets.distance", 64000);
    star_distance_01 = readJsonValue("data/config/space.json", "stars.lvl1.distance", 9000);
    star_distance_02 = readJsonValue("data/config/space.json", "stars.lvl2.distance", 3000);
    star_distance_03 = readJsonValue("data/config/space.json", "stars.lvl3.distance", 18000);

    // Restore the player's position
    space_camera.x = readJsonValue("data/config/space.json", "game.location.x", 0);
    space_camera.y = readJsonValue("data/config/space.json", "game.location.y", 0);
    surface_camera.x = readJsonValue("data/config/surface.json", "game.location.x", 0);
    surface_camera.y = readJsonValue("data/config/surface.json", "game.location.y", 0);
}

inline void saveVarsFromConfig()
{
    saveJsonValue("data/config/space.json", "planets.distance", planet_distance);
    saveJsonValue("data/config/space.json", "stars.lvl1.distance", star_distance_01);
    saveJsonValue("data/config/space.json", "stars.lvl2.distance", star_distance_02);
    saveJsonValue("data/config/space.json", "stars.lvl3.distance", star_distance_03);

    // Store the player's position
    saveJsonValue("data/config/space.json", "game.location.x", static_cast<int>(floor(space_camera.x)));
    saveJsonValue("data/config/space.json", "game.location.y", static_cast<int>(floor(space_camera.y)));
    saveJsonValue("data/config/surface.json", "game.location.x", static_cast<int>(floor(surface_camera.x)));
    saveJsonValue("data/config/surface.json", "game.location.y", static_cast<int>(floor(surface_camera.y)));
}

inline void setCurrentPlanet(const std::string& file_loc)
{
    saveJsonValue("data/config/game.json", "startup.planet.x", current_planet.x);
    saveJsonValue("data/config/game.json", "startup.planet.y", current_planet.y);
    saveJsonValue(file_loc, "planet.x", current_planet.x);
    saveJsonValue(file_loc, "planet.y", current_planet.y);
    std::cout << "planet config: " << file_loc << "\n";
}

inline void setCurrentScreen(GameScreen scr)
{
    switch (scr)
    {
        case GameScreen::SPACE:
            saveJsonValue("data/config/game.json", "game.screen", "SPACE");
            break;
        case GameScreen::SURFACE:
            saveJsonValue("data/config/game.json", "game.screen", "SURFACE");
            break;
        default:
            saveJsonValue("data/config/game.json", "game.screen", "SPACE"); // Later this may be the menu or similar.
            break;
    }
}

#endif
