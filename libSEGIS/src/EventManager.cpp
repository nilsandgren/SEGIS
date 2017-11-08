//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  EventManager:
//      Implementation
//

#include <SEGIS/EventManager.h>
#include <SEGIS/MainController.h>
#include <SEGIS/Window.h>
#include <SEGIS/GUIElement.h>
#include <SEGIS/Events.h>
#include <SEGIS/Util.h>
#include <SEGIS/GraphicsUtil.h>
#include <sys/time.h>

EventManager::EventManager()
    : mLastLeftMouseDownTime(0)
    , mIsDoubleClick(false)
{
    memset(mCurrentlyPressedMouseButtons, 0, NUM_MOUSE_BUTTONS * sizeof(bool));
    memset(mCurrentlyPressedKeys, 0, NUM_KEYS * sizeof(bool));
}

EventManager::~EventManager()
{
}

EventManager*
EventManager::mTheEventManager = 0;

EventManager*
EventManager::getEventManager()
{
    if (0 == mTheEventManager)
    {
        mTheEventManager = new EventManager();
    }
    return mTheEventManager;
}

EventSignal
EventManager::distributeMouseEvent(Window* aRootWindow,
                                   MouseEvent* aMouseEvent)
{
    return aRootWindow->handleMouseEvent(aMouseEvent);
}

EventSignal
EventManager::distributeKeyEvent(Window* aWindow, KeyEvent* aKeyEvent)
{
    return aWindow->handleKeyEvent(aKeyEvent);
}

const bool*
EventManager::setMouseChange(MouseButton aMouseButton,
                             MouseEventType aMouseEventType)
{
    // set currently pressed mouse buttons
    mIsDoubleClick = false;
    if (kMouseDown == aMouseEventType ||
        kMouseUp == aMouseEventType)
    {
        bool lPressed = false;
        if (kMouseDown == aMouseEventType)
        {
            struct timeval lCurrentTimeStruct;
            TimeStamp lCurrentTime;
            gettimeofday(&lCurrentTimeStruct, NULL);
            lCurrentTime = Float64(lCurrentTimeStruct.tv_sec) +
                           Float64(lCurrentTimeStruct.tv_usec) / 1000000.0;
            if (lCurrentTime - mLastLeftMouseDownTime < 0.3)
            {
                mIsDoubleClick = true;
            }
            mLastLeftMouseDownTime = lCurrentTime;

            lPressed = true;
        }
        else
        {
            lPressed = false;
        }
        mCurrentlyPressedMouseButtons[aMouseButton] = lPressed;
    }
#   ifdef DEBUG_EVENTS
    fprintf(stderr, "EventManager::setMouseChange - mouse buttons: ");
    for (UInt32 i = 0; i < NUM_MOUSE_BUTTONS; i++)
    {
        fprintf(stderr, "%d ", mCurrentlyPressedMouseButtons[i]);
    }
    fprintf(stderr, "\n");
#   endif
    return mCurrentlyPressedMouseButtons;
}

const bool*
EventManager::setKeyboardChange(Key aKey, KeyEventType aKeyEventType)
{
    bool lPressed;
    if (kKeyPressed == aKeyEventType)
    {
        lPressed = true;
    }
    else
    {
        lPressed = false;
    }
    mCurrentlyPressedKeys[aKey] = lPressed;

#   ifdef DEBUG_EVENTS
    fprintf(stderr, "EventManager::setKeyboardChange - keys:\n");
    for (UInt32 i = 0; i < NUM_KEYS; i++)
    {
        fprintf(stderr, "%d ", mCurrentlyPressedKeys[i]);
    }
    fprintf(stderr, "\n");
#   endif

    return mCurrentlyPressedKeys;
}

// Get the currently pressed buttons and keys
const bool*
EventManager::getPressedMouseButtons(void)
{
    // TODO We might have to return a copy here
    return mCurrentlyPressedMouseButtons;
}
const bool*
EventManager::getPressedKeys(void)
{
    // TODO We might have to return a copy here
    return mCurrentlyPressedKeys;
}

bool
EventManager::isDoubleClick()
{
    return mIsDoubleClick;
}
