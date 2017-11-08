//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Events: Structures for mouse and keyboard events
//

#pragma once

#include "Types.h"
#include "Keys.h"

// keyboard events
#define NUM_KEYS 256

class GUIElement;

typedef enum
{
    kKeyPressed,
    kKeyReleased,
    kKeyTyped
} KeyEventType;

typedef enum
{
    kKeyTypeNormal, // alphanumericals and others
    kKeyTypeSpecial // shift, arrows, function keys etc.
} KeyType;

typedef struct
{
    KeyEventType mKeyEventType;
    KeyType      mKeyType;
    Key          mKey;
    GUIElement*  mAcceptor;
} KeyEvent;


// Mouse Events
#define NUM_MOUSE_BUTTONS 3

typedef enum
{
    kMouseDown,
    kMouseUp,
    kMouseMoved,
    kMouseDragged
} MouseEventType;

typedef enum
{
    kMouseLeftButton = 0,
    kMouseMiddleButton = 1,
    kMouseRightButton = 2
} MouseButton;

struct MouseEvent
{
    MouseEventType  mMouseEventType;
    const bool*     mMouseButtons;
    Point2D         mMousePosition;         // the current position
    Point2D         mMouseOriginalPosition; // original position when dragging
    GUIElement*     mAcceptor;
    bool            mDoubleClick;

    MouseEvent()
        : mMouseEventType(kMouseDown)
        , mMouseButtons(nullptr)
        , mMousePosition({0, 0})
        , mMouseOriginalPosition({0, 0})
        , mAcceptor(nullptr)
        , mDoubleClick(false)
    {
    }
};

// Event Signals - Used to check whether an Event has been accepted by any GUIElement
typedef enum
{
    kEventAbsorbed,
    kEventIgnored
} EventSignal;
