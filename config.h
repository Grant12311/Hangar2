#ifndef HANGAR2_CONFIG_H
#define HANGAR2_CONFIG_H

namespace Hangar
{
    struct Config
    {
        const char* title = "Hangar2 Window";
        unsigned int height = 480, width = 640, borderWidth = 0, depth = 0;
        bool closeOnEscape = true;
        bool fullscreen = false;
        bool vsync = true;
        float fpsLimit = 60;

        bool mouseVisible = true;
        bool mouseIsEndless = false;

        bool resizeViewportToMatchWindowSize = true;
    };
}

#endif
