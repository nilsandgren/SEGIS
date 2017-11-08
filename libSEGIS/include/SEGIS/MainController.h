//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  MainController: 
//      Since OpenGL and GLUT are highly state based we need a central
//      module that handles the current state so this is kept away from
//      all other components as much as possible.
//      Observers that need updating at a specific rate (see TimedObserver)
//      can add themself here using the addTimedObserver function.
//

#pragma once

#include "Types.h"
#include "Window.h"
#include "TimedObserver.h"
#include "TimedObservable.h"

#include <deque>

using namespace std;

class MainController : public TimedObservable
{
    // variables
    private:
        static MainController*    mTheMainController;
        deque<TimedObserver*>     mTimedObservers;

        bool                      mIsInitalized;
        bool                      mIsStarted;
        Window*                   mCurrentWindow;
        bool                      mOKToGo;
        Point2D                   mMouseDragOrigin;

    // functions
    public:
        static MainController*    getMainController();
        // static void            destroyMainController();
        ~MainController();

        Status   init(int argc, char** argv, Window* aWindow);
        Status   start();
        bool     isInitialized();
        bool     isWindowSet();
        bool     isStarted();

        Status   setCurrentWindow(Window* aWindow);
        Window*  getCurrentWindow(void);

        void    setMouseDragOrigin(SInt32 aX, SInt32 aY);
        Point2D getMouseDragOrigin(void);

        // TimedObservable interface
        Status addTimedObserver(TimedObserver* aObserver);
        Status removeTimedObserver(TimedObserver* aObserver);
        deque<TimedObserver*> * getTimeObservers(void) { return &mTimedObservers; }

    private:
        MainController();
};
