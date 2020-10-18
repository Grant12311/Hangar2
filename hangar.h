#ifndef HANGAR2_h
#define HANGAR2_h

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xatom.h>
#endif // __linux__

#include <GJGO/timer.h>
#include <GJGO/wait.h>
#include <Druid/druid.h>
#include <Beacon/beacon.h>

#include "keycodes.h"
#include "keycode_converter.h"
#include "config.h"
#include "window.h"

#endif
