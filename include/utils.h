#pragma once

#include <iostream>

#include "glm.hpp"

void BindStdHandlesToConsole();

struct Settings
{
    // Cloth Settings

    static const size_t Density; // count of points in one row

    static const float Durability; // strength of joints WROOOOOOOOOOOOOONG
                                   //(how many times the length between two points can be increased before the connection between them is destroyed)

    static const size_t Height; // rows in cloth

    static const glm::fvec3 LeftTop; // defines offset between points, points will be placed between left and right tops
    static const glm::fvec3 RightTop;

    //

    static const glm::uvec2 Resolution;
};