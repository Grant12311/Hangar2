#ifndef HANGAR2_WINDOW_H
#define HANGAR2_WINDOW_H

#include <string>

namespace Hangar
{
    class Window
    {
    private:
        float m_frametimeCap;
        GJGO::Timer m_frametimeTracker;

        unsigned int m_oldWindowWidth;
        unsigned int m_oldWindowHeight;
    public:
        std::string title;
        bool isOpen;
        unsigned int width, height, borderWidth, depth;
        std::array<int, 2> position = {0, 0};
        bool fullscreen;
        bool vsync;
        float fpsLimit;
        float deltaTime = 0;

        bool mouseVisible;
        bool mouseIsEndless;

        bool resizeViewportToMatchWindowSize;

        #ifdef __linux__
            Display* xDisplay;
            int xScreenNum = 0;
            Screen* xScreen;
            ::Window xWindow, xRoot;

            XEvent xe;
            Atom wmDelete;
            Atom wmState;
            Atom wmFullscreen;

            GLXFBConfig* xFramebufferConfigs;
            XVisualInfo* xVisualInfo;
            Colormap xColorMap;
            XSetWindowAttributes xSetWindowAttributes;
            GLXContext xOpenGLContext;
        #endif // __linux__

        //Keyboard
        std::vector<int> keysDown;
        bool closeOnEscape = true;

        std::array<int, 2> mousePosition;
        std::array<int, 2> mousePositionAbsolute;
        std::array<bool, 3> mouseButtonsDown;
        std::vector<bool> mouseWheelEvents;
        bool mouseHasWarped;
        #ifdef __linux__
            ::Window xMouseRoot, xMouseChild;
            unsigned int xMouseMaskReturn;
        #endif // __linux__

        Beacon::Event<const int, const int> onMoveEvent;
        Beacon::Event<const unsigned int, const unsigned int> onResizeEvent;
        Beacon::Event<const int> onKeyUpEvent;
        Beacon::Event<const int> onKeyDownEvent;
        Beacon::Event<const int, const int, const int, const int> onMouseMoveEvent;
        Beacon::Event<const unsigned char> onMouseButtonDownEvent;
        Beacon::Event<const unsigned char> onMouseButtonUpEvent;
        Beacon::Event<const bool> onMouseWheelScrollEvent;

        void setTitle(const std::string &a_title)
        {
            this->title = a_title;
            XStoreName(this->xDisplay, this->xWindow, a_title.c_str());
        }

        void setTitle(const float a_title)
        {
            this->title = std::to_string(a_title);
            XStoreName(this->xDisplay, this->xWindow, this->title.c_str());
        }

        void setVsync(const bool a_vsync)
        {
            //this->vsync = a_vsync;
            //this->openglContext->makeCurrent(a_vsync);
        }

        bool keyIsDown(const int a_key)
        {
            for (int &var : this->keysDown)
                if (var == a_key)
                    return true;

            return false;
        }

        void setMousePosition(const int a_posX, const int a_posY)
        {
            #ifdef __linux__
                XWarpPointer(this->xDisplay, this->xRoot, this->xRoot, 0, 0, 0, 0, a_posX, XHeightOfScreen(this->xScreen) - a_posY);
                this->fetchMouse();
            #endif
        }

        void removeMouse()
        {
            #ifdef __linux__
                Cursor invisibleCursor;
                Pixmap bitmapNoData;
                XColor black;
                static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
                black.red = black.green = black.blue = 0;

                bitmapNoData = XCreateBitmapFromData(this->xDisplay, xWindow, noData, 8, 8);
                invisibleCursor = XCreatePixmapCursor(this->xDisplay, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
                XDefineCursor(this->xDisplay, this->xWindow, invisibleCursor);
                XFreeCursor(this->xDisplay, invisibleCursor);
                XFreePixmap(this->xDisplay, bitmapNoData);
            #endif
        }

        void restoreMouse()
        {
            #ifdef __linux__
                this->mouseVisible = false;
                XUndefineCursor(this->xDisplay, this->xWindow);
                this->mouseVisible = true;
            #endif // __linux__
        }

        void clear()
        {
            glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT  | GL_STENCIL_BUFFER_BIT));
        }

        void update()
        {
            #ifdef __linux__
            this->mouseWheelEvents.clear();
            while (XPending(this->xDisplay))
            {
                XNextEvent(this->xDisplay, &this->xe);

                switch (this->xe.type)
                {
                    case ConfigureNotify:
                        if (this->width != xe.xconfigure.width || this->height != xe.xconfigure.height)
                        {
                            this->width = xe.xconfigure.width;
                            this->height = xe.xconfigure.height;
                            this->onResizeEvent.call(this->width, this->height);

                            if (this->resizeViewportToMatchWindowSize)
                            {
                                glCall(glViewport(0, 0, this->width, this->height));
                            }
                        }
                        if (this->position[0] != xe.xconfigure.x || this->position[1] != xe.xconfigure.y)
                        {
                            this->position[0] = xe.xconfigure.x;
                            this->position[1] = XHeightOfScreen(this->xScreen) - xe.xconfigure.y - this->height;
                            this->onMoveEvent.call(this->position[0], this->position[1]);
                        }
                        break;
                    case ClientMessage:
                        if (this->xe.xclient.data.l[0] == static_cast<long int>(wmDelete))
                            this->isOpen = false;
                        break;
                    case KeyPress:
                    {
                        if (XLookupKeysym(&this->xe.xkey, 0) == XK_Escape && this->closeOnEscape)
                            this->isOpen = false;
                        int convertedKeycode = convertKeycode[XLookupKeysym(&this->xe.xkey, 0)];
                        if (!this->keyIsDown(convertedKeycode))
                        {
                            this->keysDown.push_back(convertedKeycode);
                            this->onKeyDownEvent.call(convertedKeycode);
                        }
                    }
                        break;
                    case KeyRelease:
                    {
                        const int convertedKeycode = convertKeycode[XLookupKeysym(&this->xe.xkey, 0)];
                        auto itr = std::remove(this->keysDown.begin(), this->keysDown.end(), convertedKeycode);
                        this->keysDown.erase(itr, this->keysDown.end());
                        this->onKeyUpEvent.call(convertedKeycode);
                    }
                        break;
                    case MotionNotify:
                        this->onMouseMoveEvent.call(xe.xmotion.x, xe.xmotion.y, xe.xmotion.x_root, xe.xmotion.y_root);
                        break;
                    case ButtonPress:
                        switch (this->xe.xbutton.button)
                        {
                            case 1: // left click
                                mouseButtonsDown[0] = true;
                                this->onMouseButtonDownEvent.call(0);
                                break;
                            case 2: // middle click
                                mouseButtonsDown[1] = true;
                                this->onMouseButtonDownEvent.call(1);
                                break;
                            case 3: // right click
                                mouseButtonsDown[2] = true;
                                this->onMouseButtonDownEvent.call(2);
                                break;
                            case 4: // mouse wheel up
                                this->mouseWheelEvents.push_back(true);
                                this->onMouseWheelScrollEvent.call(true);
                                break;
                            case 5: // mouse wheel down
                                this->mouseWheelEvents.push_back(false);
                                this->onMouseWheelScrollEvent.call(false);
                                break;
                        }
                        break;
                    case ButtonRelease:
                        switch (this->xe.xbutton.button)
                        {
                            case 1: // right click
                                mouseButtonsDown[0] = false;
                                this->onMouseButtonUpEvent.call(0);
                                break;
                            case 2: // middle click
                                mouseButtonsDown[1] = false;
                                this->onMouseButtonUpEvent.call(1);
                                break;
                            case 3: // left click
                                mouseButtonsDown[2] = false;
                                this->onMouseButtonUpEvent.call(2);
                                break;
                        }
                        break;
                }
            }

            // Update Mouse
            this->fetchMouse();
            #endif // __linux__
        }

        void swapBuffers()
        {
            #ifdef __linux__
                glXSwapBuffers(this->xDisplay, this->xWindow);
                if (this->vsync)
                {
                    GJGO::wait(this->m_frametimeCap - this->m_frametimeTracker.elapsed());
                }
                this->deltaTime = this->m_frametimeTracker.elapsed();
                this->m_frametimeTracker.reset();
            #endif // __linux__
        }

        Window(const Config &a_config = Config()) :
            m_frametimeCap{1.0f / a_config.fpsLimit * 1000}, title{a_config.title}, isOpen{true}, height{a_config.height}, width{a_config.width}, borderWidth{a_config.borderWidth},
            depth{a_config.depth}, fullscreen{a_config.fullscreen}, vsync{a_config.vsync}, fpsLimit{a_config.fpsLimit}, mouseVisible{a_config.mouseVisible}, mouseIsEndless{a_config.mouseIsEndless},
            resizeViewportToMatchWindowSize{a_config.resizeViewportToMatchWindowSize}, closeOnEscape{a_config.closeOnEscape}
        {
            #ifdef __linux__
                this->xDisplay = XOpenDisplay(NULL);
                if (this->xDisplay == NULL)
                {
                    std::cerr << "[HANGAR2]{ERROR} Cannot open display" << std::endl;
                    return;
                }

                this->xScreenNum = XDefaultScreen(this->xDisplay);

                int doubleBufferAttributes[] =
                {
                    GLX_X_RENDERABLE, True,
                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                    GLX_RENDER_TYPE, GLX_RGBA_BIT,
                    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
                    GLX_RED_SIZE, 8,
                    GLX_GREEN_SIZE, 8,
                    GLX_BLUE_SIZE, 8,
                    GLX_ALPHA_SIZE, 8,
                    GLX_DEPTH_SIZE, 24,
                    GLX_STENCIL_SIZE, 8,
                    GLX_DOUBLEBUFFER, True,
                    None
                };

                int numReturned = 0;
                this->xFramebufferConfigs = glXChooseFBConfig(this->xDisplay, this->xScreenNum, doubleBufferAttributes, &numReturned);
                if (this->xFramebufferConfigs == NULL)
                {
                    std::cout << "No double buffered config available" << std::endl;
                    exit(1);
                }

                this->xVisualInfo = glXGetVisualFromFBConfig(this->xDisplay, this->xFramebufferConfigs[0]);
                if(this->xVisualInfo == NULL)
                {
                    std::cout << "No appropriate visual found" << std::endl;
                    exit(1);
                }

                this->xRoot = DefaultRootWindow(this->xDisplay);
                this->xColorMap = XCreateColormap(this->xDisplay, this->xRoot, this->xVisualInfo->visual, AllocNone);
                this->xSetWindowAttributes.colormap = this->xColorMap;
                this->xSetWindowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

                this->xScreen = XDefaultScreenOfDisplay(this->xDisplay);
                this->xWindow = XCreateWindow(this->xDisplay, this->xRoot, 0, 0, a_config.width, a_config.height, 1, this->xVisualInfo->depth, InputOutput, this->xVisualInfo->visual,
                                              CWColormap | CWEventMask, &this->xSetWindowAttributes);
                XStoreName(this->xDisplay, this->xWindow, a_config.title);
                XAutoRepeatOff(this->xDisplay);

                this->wmDelete = XInternAtom(this->xDisplay, "WM_DELETE_WINDOW", true);
                XSetWMProtocols(this->xDisplay, this->xWindow, &this->wmDelete, 1);

                this->wmState = XInternAtom(this->xDisplay, "_NET_WM_STATE", true);

                if (a_config.fullscreen)
                {
                    this->wmFullscreen = XInternAtom(this->xDisplay, "_NET_WM_STATE_FULLSCREEN", true);

                    XChangeProperty(this->xDisplay, this->xWindow,this-> wmState, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wmFullscreen, 1);
                }

                XMapWindow(this->xDisplay, this->xWindow);

                XGetGeometry(this->xDisplay, this->xWindow, &this->xRoot, &this->position[0], &this->position[1], &this->width, &this->height, &this->borderWidth, &this->depth);
                if (a_config.fullscreen)
                {
                    this->height = XHeightOfScreen(this->xScreen);
                    this->width = XWidthOfScreen(this->xScreen);
                }

                int glContextTypeBit;
                if (a_config.useOpenGLES)
                {
                    glContextTypeBit = GLX_CONTEXT_ES_PROFILE_BIT_EXT;
                }else{
                    glContextTypeBit = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
                }

                int contextAttribs[] =
                {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, a_config.openglMajorVersion,
                    GLX_CONTEXT_MINOR_VERSION_ARB, a_config.openglMinorVersion,
                    GLX_CONTEXT_PROFILE_MASK_ARB, glContextTypeBit,
                    None
                };

                typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
                glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

                this->xOpenGLContext = glXCreateContextAttribsARB(this->xDisplay, xFramebufferConfigs[0], 0, GL_TRUE, contextAttribs);
                glXMakeCurrent(this->xDisplay, this->xWindow, this->xOpenGLContext);
            #endif // __linux__
        }

        ~Window()
        {
            #ifdef __linux__
                XAutoRepeatOn(this->xDisplay);
                XDestroyWindow(this->xDisplay, this->xWindow);
                XCloseDisplay(this->xDisplay);
            #endif // __linux__
        }
    private:
        void fetchMouse()
        {
            #ifdef __linux__
                std::array<int, 2> mousePositionTemp;

                XQueryPointer(this->xDisplay, this->xWindow, &this->xMouseRoot, &this->xMouseChild, &this->mousePositionAbsolute[0], &this->mousePositionAbsolute[1], &mousePositionTemp[0], &mousePositionTemp[1], &this->xMouseMaskReturn);
                this->mousePositionAbsolute[1] = XHeightOfScreen(this->xScreen) - this->mousePositionAbsolute[1] - 1;
                if (this->fullscreen)
                {
                    mousePositionTemp[1] = XHeightOfScreen(this->xScreen) - mousePositionTemp[1] - 1;
                }else{
                    mousePositionTemp[1] = this->height - mousePositionTemp[1] - 1;
                }

                if (this->mousePosition != mousePositionTemp)
                {
                    this->mousePosition = mousePositionTemp;
                    this->onMouseMoveEvent.call(this->mousePosition[0], this->mousePosition[1], this->mousePositionAbsolute[0], this->mousePositionAbsolute[1]);
                }
            #endif // __linux__
        }
    };
}

#endif
