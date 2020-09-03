#ifndef HANGAR2_h
#define HANGAR2_h

#ifdef HANGAR2_OPENGL_ES
    #define WARLOCK_OPENGL_ES
#elif defined HANGAR2_OPENGL_DESKTOP
    #error Hangar2 does not currently support desktop OpenGL!
#else
    #error Hangar2 you must choose an OpenGL context type!
#endif // HANGAR2_OPENGL_ES

#include <iostream>
#include <vector>
#include <algorithm>

#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xatom.h>
#endif // __linux__

#include <GJGO/timer.h>
#include <GJGO/wait.h>
#include <Warlock/warlock.h>
#include <Druid/druid.h>
#include <Beacon/beacon.h>

#include "keycodes.h"
#include "keycode_converter.h"
#include "config.h"
#include "window.h"

#endif
