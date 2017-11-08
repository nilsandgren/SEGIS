//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  MainController: 
//      Implementation.
//

#include <SEGIS/Types.h>
#include <SEGIS/Events.h>
#include <SEGIS/MainController.h>
#include <SEGIS/EventManager.h>
#include <SEGIS/DisplayManager.h>
#include <SEGIS/FontController.h>

#include <sys/time.h>

#include <locale>

// private c-callbacks for different gl events

void
reshapeCallback(int w, int h)
{
    MainController* lMainController = MainController::getMainController();
    if (lMainController->isInitialized())
    {
        if (lMainController->isStarted())
        {
            // glViewport(0, 0, (GLsizei)w, (GLsizei)h);
            lMainController->getCurrentWindow()->resize(w, h);
            glutPostRedisplay();
        }
        else
        {
            fprintf(stderr, "MainController::reshapeCallback - Not started yet.\n");
        }
    }
    else
    {
        fprintf(stderr, "MainController::reshapeCallback - Not initialized yet.\n");
        fprintf(stderr, "MainController::reshapeCallback - Use MainController::init()\n");
    }
}

static void
displayCallback(void)
{
    MainController* lMainController = MainController::getMainController();
    if (lMainController->isInitialized())
    {
        if (lMainController->isStarted())
        {
            glClear(GL_COLOR_BUFFER_BIT);
            lMainController->getCurrentWindow()->draw();
            glutSwapBuffers();
        }
        else
        {
            fprintf(stderr, "MainController::display Main Controller not started.\n");
        }
    }
    else
    {
        fprintf(stderr, "MainController not initialized. Use MainController::init()\n");
    }
}

void 
keyboardCallback(unsigned char aKey, int /* aX */, int /* aY */)
{
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();
    KeyEvent lKeyEvent;
    lKeyEvent.mKeyEventType = kKeyPressed;
    lKeyEvent.mKeyType = kKeyTypeNormal;
    lKeyEvent.mKey = aKey;
    lEventManager->distributeKeyEvent(lCurrentWindow, &lKeyEvent);
}

void 
keyboardUpCallback(unsigned char aKey, int /* aX */, int /* aY */)
{
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();
    KeyEvent lKeyEvent;
    lKeyEvent.mKeyEventType = kKeyReleased;
    lKeyEvent.mKeyType = kKeyTypeNormal;
    lKeyEvent.mKey = aKey;
    lEventManager->distributeKeyEvent(lCurrentWindow, &lKeyEvent);
}

void 
keyboardSpecialCallback(int aKey, int /* aX */, int /* aY */)
{
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();
    KeyEvent lKeyEvent;
    lKeyEvent.mKeyEventType = kKeyPressed;
    lKeyEvent.mKeyType = kKeyTypeSpecial;
    lKeyEvent.mKey = aKey;
    lEventManager->distributeKeyEvent(lCurrentWindow, &lKeyEvent);
}

void 
keyboardSpecialUpCallback(int aKey, int /* aX */, int /* aY */)
{
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();
    KeyEvent lKeyEvent;
    lKeyEvent.mKeyEventType = kKeyReleased;
    lKeyEvent.mKeyType = kKeyTypeSpecial;
    lKeyEvent.mKey = aKey;
    lEventManager->distributeKeyEvent(lCurrentWindow, &lKeyEvent);
}

void 
mouseClickCallback(int aButton, int aState, int aX, int aY)
{
    MouseEvent lMouseEvent;
    lMouseEvent.mDoubleClick = false;
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();

    if (!lCurrentWindow)
    {
        return;
    }

    switch (aState)
    {
        case GLUT_DOWN:
            lMouseEvent.mMouseEventType = kMouseDown;
            break;
        case GLUT_UP:
            lMouseEvent.mMouseEventType = kMouseUp;
            break;
        default:
            lMouseEvent.mMouseEventType = kMouseDown;
            break;
    }

    SInt32 lX = aX;
    SInt32 lY = lCurrentWindow->getSize().y - aY;
    lMouseEvent.mMousePosition.x = lMouseEvent.mMouseOriginalPosition.x = lX;
    // mouse position is in window coordinates (y=0 at top of window)
    lMouseEvent.mMousePosition.y = lMouseEvent.mMouseOriginalPosition.y = lY;
    // save this position
    lMainController->setMouseDragOrigin(lX, lY);

    MouseButton lMouseButton;
    switch (aButton)
    {
        case GLUT_LEFT_BUTTON:
            lMouseButton = kMouseLeftButton;
            break;
        case GLUT_MIDDLE_BUTTON:
            lMouseButton = kMouseMiddleButton;
            break;
        case GLUT_RIGHT_BUTTON:
            lMouseButton = kMouseRightButton;
            break;
        default:
            lMouseButton = kMouseLeftButton;
            break;
    }

    lMouseEvent.mAcceptor = 0;

    const bool* lMouseButtons = lEventManager->setMouseChange(lMouseButton, 
                                                              lMouseEvent.mMouseEventType);
    lMouseEvent.mMouseButtons = lMouseButtons;
    if (lEventManager->isDoubleClick())
    {
        lMouseEvent.mDoubleClick = true;
    }
    else
    {
        lMouseEvent.mDoubleClick = false;
    }
    
    lEventManager->distributeMouseEvent(lCurrentWindow, &lMouseEvent);
}

void 
mouseDragCallback(int aX, int aY)
{
    MouseEvent lMouseEvent;
    lMouseEvent.mDoubleClick = false;
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();

    if (!lCurrentWindow)
    {
        return;
    }

    Point2D lMouseDragOrigin = lMainController->getMouseDragOrigin();
    lMouseEvent.mMouseButtons =  lEventManager->getPressedMouseButtons();
    lMouseEvent.mMousePosition.x = aX;
    lMouseEvent.mMousePosition.y = lCurrentWindow->getSize().y - aY;
    lMouseEvent.mMouseOriginalPosition.x = lMouseDragOrigin.x;
    lMouseEvent.mMouseOriginalPosition.y = lMouseDragOrigin.y;
    lMouseEvent.mMouseEventType = kMouseDragged;
    lMouseEvent.mAcceptor = 0;

    lEventManager->distributeMouseEvent(lCurrentWindow, &lMouseEvent);
}

void 
mousePassiveMoveCallback(int aX, int aY)
{
    MouseEvent lMouseEvent;
    lMouseEvent.mDoubleClick = false;
    MainController* lMainController = MainController::getMainController();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    EventManager* lEventManager = EventManager::getEventManager();

    if (!lCurrentWindow)
    {
        return;
    }

    SInt32 lX = aX;
    SInt32 lY = lCurrentWindow->getSize().y - aY;
    lMouseEvent.mMouseButtons =  lEventManager->getPressedMouseButtons();
    lMouseEvent.mMousePosition.x = lMouseEvent.mMouseOriginalPosition.x = lX;
    lMouseEvent.mMousePosition.y = lMouseEvent.mMouseOriginalPosition.y = lY;
    lMouseEvent.mMouseEventType = kMouseMoved;
    lMouseEvent.mAcceptor = 0;
    // save this position
    lMainController->setMouseDragOrigin(lX, lY);

    lEventManager->distributeMouseEvent(lCurrentWindow, &lMouseEvent);
}

void
idleCallback(void)
{
    MainController* lMainController = MainController::getMainController();
    DisplayManager* lDisplayManager = DisplayManager::getDisplayManager();
    Window* lCurrentWindow = lMainController->getCurrentWindow();
    struct timeval lCurrentTimeStruct;
    TimeStamp lCurrentTime;
    TimeStamp lLastUpdateTime;

    gettimeofday(&lCurrentTimeStruct, NULL);
    lCurrentTime = Float64(lCurrentTimeStruct.tv_sec) + 
                   Float64(lCurrentTimeStruct.tv_usec) / 1000000.0;
    lLastUpdateTime = lCurrentWindow->getLastAnimationTime();

    // animation stuff
    if (lCurrentTime - lLastUpdateTime > 0.03333) // 30 fps
    {
        lCurrentWindow->updateAnimation();
        lCurrentWindow->setLastAnimationTime(lCurrentTime);
        lDisplayManager->requestUpdate(lCurrentWindow->getRootGUIElement());
    }

    // update attached timed observers
    deque<TimedObserver*> * lTimedObservers = lMainController->getTimeObservers();
    auto lTimedObserverIter = lTimedObservers->begin();
    for (; lTimedObserverIter != lTimedObservers->end(); ++lTimedObserverIter)
    {
        TimedObserver* lObserver = *lTimedObserverIter;
        if (lCurrentTime - lObserver->getLastUpdateTime() > lObserver->getUpdateInterval())
        {
            lObserver->timedUpdate();
            lObserver->setLastUpdateTime(lCurrentTime);
        }
    }

    // sleep a little while
    usleep(3000);
}

void
visibilityCallback(int aVisibility)
{
    if (GLUT_VISIBLE == aVisibility)
    {
        glutIdleFunc(idleCallback);
    }
    else
    {
        // stop idle callback when window is not visible
        glutIdleFunc(NULL);
    }
}

// MainController c++ implementation

MainController::MainController()
    : mIsInitalized(false)
    , mIsStarted(false)
    , mCurrentWindow(0)
    , mOKToGo(false)
{
    mMouseDragOrigin.x = 0;
    mMouseDragOrigin.y = 0;
}

MainController::~MainController()
{
    FontController * lFontController = FontController::getFontController();
    delete lFontController;
}

MainController*
MainController::mTheMainController = 0;

MainController*
MainController::getMainController()
{
    if (0 == mTheMainController)
    {
        mTheMainController = new MainController();
    }
    return mTheMainController;
}

bool
MainController::isInitialized()
{
    return mIsInitalized;
}

bool
MainController::isWindowSet()
{
    return (mCurrentWindow != 0);
}

bool
MainController::isStarted()
{
    return mIsStarted;
}

Status   
MainController::init(int argc, char** argv, Window* aWindow)
{
    if (!isInitialized())
    {
        // std::locale::global(std::locale("sv_SE"));
        // std::setlocale(LC_ALL, "sv_SE");
        // std::setlocale(LC_ALL, "En_US");
        
        if (!aWindow)
        {
            fprintf(stderr, "MainController::init called with NULL Window.\n");
            return kFailed;
        }
        aWindow->updateTabOrder();
        mCurrentWindow = aWindow;
        glutInit(&argc,argv);
        // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // we dont need depth test
        glutCreateWindow(aWindow->getName());
        glutInitWindowSize (aWindow->getSize().x, aWindow->getSize().y);
        glViewport(0, 0, aWindow->getSize().x, aWindow->getSize().y);
        glLineWidth(1.0);
        glPointSize(1.0);
        glClearColor(0.0,0.0,0.0,0.0);

        // callback functions
        // display
        glutDisplayFunc(displayCallback);    
        glutVisibilityFunc(visibilityCallback);    
        // reshape
        glutReshapeFunc(reshapeCallback);
        // keyboard
        glutKeyboardFunc(keyboardCallback);
        glutKeyboardUpFunc(keyboardUpCallback);
        glutSpecialFunc(keyboardSpecialCallback);
        glutSpecialUpFunc(keyboardSpecialUpCallback);
        // mouse
        glutMouseFunc(mouseClickCallback);
        glutMotionFunc(mouseDragCallback);
        glutPassiveMotionFunc(mousePassiveMoveCallback);

        glLoadIdentity();
        // prepare for blending without enabling it
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        // anti-alias
        // glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
        // glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
        // glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        // glEnable (GL_LINE_SMOOTH);
        // glEnable (GL_POINT_SMOOTH);
        // glEnable (GL_POLYGON_SMOOTH);

        glutReshapeWindow(aWindow->getSize().x, aWindow->getSize().y);

        mIsInitalized = true;
    }
    return kSucceeded;
}

Status
MainController::setCurrentWindow(Window* aWindow)
{
    if (!isInitialized())
    {
        fprintf(stderr, "MainController::setCurrentWindow Controller not initialized.\n");
        return kFailed;
    }
    if (aWindow)
    {
        glutSetWindow(aWindow->getID());
        mCurrentWindow = aWindow;
        return kSucceeded;
    }
    else
    {
        fprintf(stderr, "MainController::setCurrentWindow aWindow is NULL\n");
        return kFailed;
    }
}

Window*
MainController::getCurrentWindow()
{
    return mCurrentWindow;
}

Status   
MainController::start()
{
    if (!isStarted())
    {
        if (!isInitialized())
        {
            fprintf(stderr, "MainController::setCurrentWindow Controller not initialized.\n");
            return kFailed;
        }
        if (isWindowSet())
        {
            mIsStarted = true;
            glutMainLoop();
        }
        else
        {
            fprintf(stderr, "MainController::start Must set current window before start.\n");
            fprintf(stderr, "MainController::start Use MainController::setCurrentWindow()\n");
            return kFailed;
        }
    }
    return kSucceeded;
}

void
MainController::setMouseDragOrigin(SInt32 aX, SInt32 aY)
{
    mMouseDragOrigin.x = aX;
    mMouseDragOrigin.y = aY;
}

Point2D
MainController::getMouseDragOrigin(void)
{
    return mMouseDragOrigin;
}

Status
MainController::addTimedObserver(TimedObserver* aObserver)
{
    if (aObserver)
    {
        mTimedObservers.push_back(aObserver);
        return kSucceeded;
    }
    return kFailed;
}

Status
MainController::removeTimedObserver(TimedObserver* aObserver)
{
    auto lObserverIterator = mTimedObservers.begin();
    for (; lObserverIterator != mTimedObservers.end(); ++lObserverIterator)
    {
        if (*lObserverIterator == aObserver)
        {
            mTimedObservers.erase(lObserverIterator);
            return kSucceeded;
        }
    }
    return kFailed;
}
