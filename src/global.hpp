#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.hpp"
#include "fastrand.hpp"

const int window_height { 720 };
const int window_width { 1080 };
const char* const title { "Pale Frontier" };
inline Vector2 camera{ 0, 0 };
inline float speed {0.5f };
const int planet_sector_size { 8 };
const int star_sector_size_01 { 3 };
const int star_sector_size_02 { 4 };
const int star_sector_size_03 { 5 };
inline int planet_distance { 25000 };
inline int star_distance_01 { 9000 };
inline int star_distance_02 { 3000 };
inline int star_distance_03 { 18000 };

inline Frand::FastRand frand{};

#endif
