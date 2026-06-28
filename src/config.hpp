#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <time.h>

inline int generalRand(int max)
{
    return rand() % max + 1;
}



inline void createConfigDirectories()
{
    createDirectories("data/player");
    createDirectories("data/config");
    createDirectories("data/space/planets");
}

inline const std::string getPlanetPath(int sector_x, int sector_y)
{
    return "data/space/planets/planet_" + std::to_string(sector_x) + "_" + std::to_string(sector_y) + ".json";
}

inline void getCurrentPlanet()
{
    current_planet.x = readJsonValue("data/config/game.json", "startup.planet.x", 0);
    current_planet.y = readJsonValue("data/config/game.json", "startup.planet.y", 0);
}

inline void setCurrentPlanet()
{
    saveJsonValue("data/config/game.json", "startup.planet.x", current_planet.x);
    saveJsonValue("data/config/game.json", "startup.planet.y", current_planet.y);
}

inline void loadVarsFromConfig()
{
    std::string_view t { "SPACE" };
    game_init_seed = readJsonValue("data/config/game.json", "game.init.seed", 23232323);
    std::string_view scr = readJsonValue("data/config/game.json", "game.screen", t);

    if (scr == "SPACE")
        currentScreen = GameScreen::SPACE;
    else if (scr == "SURFACE")
        currentScreen = GameScreen::SURFACE;
    else
        currentScreen = GameScreen::SPACE; // Later this may be the menu or similar.

    player_space_fuel = readJsonValue("data/config/space.json", "player.fuel", 100);
    item_distance = readJsonValue("data/config/space.json", "items.distance", 180000);
    planet_distance = readJsonValue("data/config/space.json", "planets.distance", 64000);
    star_distance_01 = readJsonValue("data/config/space.json", "stars.lvl1.distance", 9000);
    star_distance_02 = readJsonValue("data/config/space.json", "stars.lvl2.distance", 3000);
    star_distance_03 = readJsonValue("data/config/space.json", "stars.lvl3.distance", 18000);

    // Restore the current planet
    getCurrentPlanet();

    // Restore the player's position
    int max_dist = generalRand(25000);
    space_camera.x = readJsonValue("data/config/space.json", "game.location.x", max_dist);
    max_dist = generalRand(22000);
    space_camera.y = readJsonValue("data/config/space.json", "game.location.y", max_dist);
    surface_camera.x = readJsonValue("data/config/surface.json", "game.location.x", 0);
    surface_camera.y = readJsonValue("data/config/surface.json", "game.location.y", 0);
}

inline void saveVarsToConfig()
{
    saveJsonValue("data/config/game.json", "game.init.seed", game_init_seed);

    saveJsonValue("data/config/space.json", "player.fuel", player_space_fuel);
    saveJsonValue("data/config/space.json", "items.distance", item_distance);
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
