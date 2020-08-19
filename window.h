#ifndef HANGAR2_WINDOW_H
#define HANGAR2_WINDOW_H

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
        const char* title;
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

        Warlock::Context* openglContext;

        void setVsync(const bool a_vsync)
        {
            this->vsync = a_vsync;
            this->openglContext->makeCurrent(a_vsync);
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
                XWarpPointer(this->xDisplay, None, this->xWindow, 0, 0, 0, 0, a_posX, XHeightOfScreen(this->xScreen) - a_posY);
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
                        XGetGeometry(this->xDisplay, this->xWindow, &this->xRoot, &this->position[0], &this->position[1], &this->width, &this->height, &this->borderWidth, &this->depth);
                        if (this->resizeViewportToMatchWindowSize)
                        {
                            glCall(glViewport(0, 0, this->width, this->height));
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
                            this->keysDown.push_back(convertedKeycode);
                    }
                        break;
                    case KeyRelease:
                    {
                        auto itr = std::remove(this->keysDown.begin(), this->keysDown.end(), convertKeycode[XLookupKeysym(&this->xe.xkey, 0)]);
                        this->keysDown.erase(itr, this->keysDown.end());
                    }
                        break;
                    case ButtonPress:
                        switch (this->xe.xbutton.button)
                        {
                            case 1: // left click
                                mouseButtonsDown[0] = true;
                                break;
                            case 2: // middle click
                                mouseButtonsDown[1] = true;
                                break;
                            case 3: // right click
                                mouseButtonsDown[2] = true;
                                break;
                            case 4: // mouse wheel up
                                this->mouseWheelEvents.push_back(true);
                                break;
                            case 5: // mouse wheel down
                                this->mouseWheelEvents.push_back(false);
                                break;
                        }
                        break;
                    case ButtonRelease:
                        switch (this->xe.xbutton.button)
                        {
                            case 1: // right click
                                mouseButtonsDown[0] = false;
                                break;
                            case 2: // middle click
                                mouseButtonsDown[1] = false;
                                break;
                            case 3: // left click
                                mouseButtonsDown[2] = false;
                                break;
                        }
                        break;
                }
            }

            // Update Mouse
            this->mouseHasWarped = false;
            this->fetchMouse();
            if (this->mouseIsEndless)
            {
                if (this->mousePositionAbsolute[0] >= static_cast<int>(this->width - 1))
                {
                    this->setMousePosition(0, this->mousePositionAbsolute[1]);
                    this->mouseHasWarped = true;
                }else if (this->mousePositionAbsolute[0] <= 1){
                    this->setMousePosition(this->width, this->mousePositionAbsolute[1]);
                    this->mouseHasWarped = true;
                }

                if (this->mousePositionAbsolute[1] >= static_cast<int>(this->height - 1))
                {
                    this->setMousePosition(this->mousePositionAbsolute[0], 0);
                    this->mouseHasWarped = true;
                }else if (this->mousePositionAbsolute[1] <= 1){
                    this->setMousePosition(this->mousePositionAbsolute[0], this->height);
                    this->mouseHasWarped = true;
                }
                this->fetchMouse();
            }
            #endif // __linux__
        }

        void swapBuffers()
        {
            this->openglContext->swapBuffers();
            if (this->vsync)
            {
                GJGO::wait(this->m_frametimeCap - this->m_frametimeTracker.elapsed());
            }
            std::cout << this->m_frametimeTracker.elapsed() << std::endl;
            this->deltaTime = this->m_frametimeTracker.elapsed();
            this->m_frametimeTracker.reset();
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
                this->xScreen = XDefaultScreenOfDisplay(this->xDisplay);
                this->xWindow = XCreateSimpleWindow(this->xDisplay, XRootWindow(this->xDisplay, this->xScreenNum), 10, 10, a_config.width, a_config.height, 1,
                                                    BlackPixel(this->xDisplay, this->xScreenNum), WhitePixel(this->xDisplay, this->xScreenNum));
                XSetStandardProperties(this->xDisplay, this->xWindow, a_config.title, a_config.title, None, NULL, 0, NULL);
                XSelectInput(this->xDisplay, this->xWindow, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
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

                this->openglContext = new Warlock::Context(this->xDisplay, this->xWindow);
                this->openglContext->makeCurrent(a_config.vsync);
            #endif // __linux__
        }

        ~Window()
        {
            delete this->openglContext;

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
                XQueryPointer(this->xDisplay, this->xWindow, &this->xMouseRoot, &this->xMouseChild, &this->mousePositionAbsolute[0], &this->mousePositionAbsolute[1], &this->mousePosition[0], &this->mousePosition[1], &this->xMouseMaskReturn);
                this->mousePositionAbsolute[1] = XHeightOfScreen(this->xScreen) - this->mousePositionAbsolute[1] - 1;
                if (this->fullscreen)
                {
                    this->mousePosition[1] = XHeightOfScreen(this->xScreen) - this->mousePosition[1] - 1;
                }else{
                    this->mousePosition[1] = this->height - this->mousePosition[1] - 1;
                }
            #endif // __linux__
        }
    };
}

#endif
