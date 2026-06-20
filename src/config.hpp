#ifndef CONFIG_H
#define CONFIG_H

inline void loadVarsFromConfig()
{
    planet_distance = readJsonValue("data/config/space.json", "planets.distance", 64000);
    star_distance_01 = readJsonValue("data/config/space.json", "stars.lvl1.distance", 9000);
    star_distance_02 = readJsonValue("data/config/space.json", "stars.lvl2.distance", 3000);
    star_distance_03 = readJsonValue("data/config/space.json", "stars.lvl3.distance", 18000);

    // Restore the player's position
    space_camera.x = readJsonValue("data/player/stats.json", "game.location.x", 0);
    space_camera.y = readJsonValue("data/player/stats.json", "game.location.y", 0);
}

inline void saveVarsFromConfig()
{
    saveJsonValue("data/config/space.json", "planets.distance", planet_distance);
    saveJsonValue("data/config/space.json", "stars.lvl1.distance", star_distance_01);
    saveJsonValue("data/config/space.json", "stars.lvl2.distance", star_distance_02);
    saveJsonValue("data/config/space.json", "stars.lvl3.distance", star_distance_03);

    // Store the player's position
    saveJsonValue("data/player/stats.json", "game.location.x", static_cast<int>(floor(space_camera.x)));
    saveJsonValue("data/player/stats.json", "game.location.y", static_cast<int>(floor(space_camera.y)));
}

#endif
