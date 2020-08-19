#ifndef HANGAR2_EVENT_H
#define HANGAR2_EVENT_H

namespace Hangar
{
    enum class KeyEventType
    {
        up,
        down
    };

    struct SizeChangeEvent
    {
        const unsigned int width;
        const unsigned int height;
    };

    struct KeyEvent
    {
        const KeyEventType type;
        const int keycode;
    };
}

#endif
