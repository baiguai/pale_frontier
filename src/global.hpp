#ifndef GLOBAL_H
#define GLOBAL_H

#include "main.hpp"
#include "fastrand.hpp"

const int window_height { 720 };
const int window_width { 1080 };
const char* const title { "Pale Frontier" };
inline Point camera{ 0, 0 };
const int planet_sector_size { 8 };
const int star_sector_size_01 { 1 };
const int star_sector_size_02 { 2 };
const int star_sector_size_03 { 3 };
const int planet_distance { 15000 };
const int star_distance_01 { 9000 };
const int star_distance_02 { 4000 };
const int star_distance_03 { 20000 };

inline Frand::FastRand frand{};

#endif
