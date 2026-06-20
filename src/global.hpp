#ifndef GLOBAL_H
#define GLOBAL_H

#include "../libs/raylib/raylib.h"
#include "fastrand.hpp"

const int window_height { 720 };
const int window_width { 1080 };
const char* const title { "Pale Frontier" };
inline Vector2 space_camera{ 0, 0 };
inline float space_speed {0.5f };
const int planet_sector_size { 8 };
const int star_sector_size_01 { 4 };
const int star_sector_size_02 { 5 };
const int star_sector_size_03 { 6 };
inline int planet_distance { 0 };
inline int star_distance_01 { 0 };
inline int star_distance_02 { 0 };
inline int star_distance_03 { 0 };

inline Frand::FastRand frand{};

#include "main.hpp"

#endif
