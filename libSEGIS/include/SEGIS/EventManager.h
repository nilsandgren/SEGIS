//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  EventManager: 
//      Handles distribution of events to GUI components
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Window.h"
#include "GUIElement.h"

class EventManager
{
    // variables
    private:
        bool mCurrentlyPressedMouseButtons[NUM_MOUSE_BUTTONS];
        bool mCurrentlyPressedKeys[NUM_KEYS];
        static EventManager* mTheEventManager;
        TimeStamp mLastLeftMouseDownTime;
        bool mIsDoubleClick;

    // functions
    private:
        EventManager();

    public: 
        ~EventManager();

        // Return the one and only instance
        static EventManager* getEventManager();
    
        // Send a MouseEvent to the root element of a Window, and let it
        // propagate to all children until an element absorbs it.
        // kEventIgnored is returned if no GUIElement accepts the event
        // kEventAbsorbed otherwise
        EventSignal distributeMouseEvent(Window* aRootWindow, 
                                         MouseEvent* aMouseEvent);

        // Send a KeyEvent to a Window
        // kEventAbsorbed is returned if a GUIElement, or the Window itself,
        // accepts the event
        // kEventAbsorbed otherwise
        EventSignal distributeKeyEvent(Window* aWindow, KeyEvent* aKeyEvent);

        // Tell EventManager about mouse and keyboard changes 
        const bool* setMouseChange(MouseButton aMouseButton,
                                   MouseEventType aMouseEventType);
        const bool* setKeyboardChange(Key aKey,
                                      KeyEventType aKeyEventType);

        // Get the currently pressed buttons and keys
        const bool* getPressedMouseButtons(void);
        const bool* getPressedKeys(void);
    
        bool isDoubleClick();
};
