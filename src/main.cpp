#include <Windows.h>

#include "GL/glew.h"
#include "window.h"
#include "renderer.h"

#include "utils.h"
#include "log.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG

    AllocConsole();

    BindStdHandlesToConsole();

#endif 

    glm::uvec2 resolution = Settings::Resolution;
    Window window(hInstance, L"test", glm::vec2(resolution.x, resolution.y));

    RendererConfigurations config = { 25000, "res/basic.shader", resolution };
    Renderer renderer(config);

    LARGE_INTEGER ticksPerSecond, lastTickCount, currentTickCount;
    QueryPerformanceFrequency(&ticksPerSecond);
    QueryPerformanceCounter(&lastTickCount);

    float dt;

    while (window.IsRunning())
    {
        renderer.DrawQuad({ 0.0, 0.0, 0.0 }, { 0.8f, 0.2f, 0.4f, 1.0f }, 1);
        renderer.DrawCall();
        renderer.Flush();

        // Evaludate Delta Time
        {
            QueryPerformanceCounter(&currentTickCount);

            uint64_t elapsedTicks = currentTickCount.QuadPart - lastTickCount.QuadPart;

            // Convert to Microseconds to not loose precision, by deviding a small numbner by a large one
            uint64_t elapsedTimeInMicroseconds = (elapsedTicks * 1000000) / ticksPerSecond.QuadPart;

            lastTickCount = currentTickCount;

            // Time in milliseconds
            dt = (float)elapsedTimeInMicroseconds / 1000.0f;

            // Time in Seconds
            dt /= 1000.0f;
        }

        window.SwapBuffers();
        window.ClearScreen();
        window.OnUpdate();
    }

    WAIT;
    return 0;
}