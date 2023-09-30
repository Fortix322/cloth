#include "utils.h"

void BindStdHandlesToConsole()
{
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stderr);
    freopen("CONOUT$", "w", stdout);

    std::clog.clear();
    std::wclog.clear();
    std::cout.clear();
    std::wcout.clear();
    std::cerr.clear();
    std::cin.clear();
    std::wcin.clear();
}

const size_t Settings::Density = 40; // count of points in one row

const float Settings::Durability = 2.0f; // strength of joints            WRONGGGGGGGGGGGGGGGG
                                         //(how many times the length between two points can be increased before the connection between them is destroyed)

const size_t Settings::Height = 40; // rows in cloth

const glm::fvec3 Settings::LeftTop = { -0.8f, 1.0f, 0.0f }; // defines offset between points, points will be placed between left and right tops
const glm::fvec3 Settings::RightTop = { 0.8f, 1.0f, 0.0f };

const glm::uvec2 Settings::Resolution = { 1280, 720 };