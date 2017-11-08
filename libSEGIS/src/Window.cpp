//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  Window: 
//      Implementation
//

#include <SEGIS/Window.h>
#include <SEGIS/LayoutManager.h>
#include <SEGIS/MenuController.h>
#include <SEGIS/DisplayManager.h>

Window::Window(UInt32       aWidth, 
               UInt32       aHeight, 
               GUIElement*  aRoot, 
               const char*  aName)
  : mGUIRootElement(aRoot)
  , mIsFullScreen(false)
  , mName(0)
  , mFocusedElement(0)
  , mFocusedIndex(-1)
  , mFirstResize(true)
  , mRespectMinimumSafeSize(true)
  , mLastAnimationTime(0.0)
{
    UInt32 lStringLen = strlen(aName);
    if (lStringLen)
    {   
        mName = (char*) malloc(lStringLen + 1);
        memcpy(mName, aName, lStringLen);
        mName[lStringLen] = 0;
    }

    aRoot->setParentWindow(this);

    mWindowSize.x = mPreviousWindowSize.x = mMinimumSafeSize.x = aWidth;
    mWindowSize.y = mPreviousWindowSize.y = mMinimumSafeSize.y = aHeight;
}

Window::~Window()
{
    glutDestroyWindow(mID);
}

EventSignal
Window::handleKeyEvent(KeyEvent* aKeyEvent)
{
    EventSignal lEventSignal = kEventIgnored;
    if (mFocusedElement)
    {
        lEventSignal = mFocusedElement->handleKeyEvent(aKeyEvent);
    }
    if (kEventIgnored == lEventSignal)
    {
        lEventSignal = handleKeyEventInternal(aKeyEvent);
    }
    return lEventSignal;
}

EventSignal
Window::handleMouseEvent(MouseEvent* aMouseEvent)
{
    EventSignal lEventSignal = kEventIgnored;

    auto lOverlayItem = mOverlayElements.begin();
    for (; lOverlayItem != mOverlayElements.end(); ++lOverlayItem)
    {
        // send to all visible overlay elements
        if ((*lOverlayItem)->isVisible())
        {
            lEventSignal = (*lOverlayItem)->handleMouseEvent(aMouseEvent);
            if (kEventAbsorbed == lEventSignal)
            {
                if (aMouseEvent->mAcceptor != mFocusedElement)
                {
                    setFocusedElement(aMouseEvent->mAcceptor);
                }
                return kEventAbsorbed;
            }
        }
    }

    if (!mGUIRootElement)
    {
        return kEventIgnored;
    }

    // XXX TODO 
    // perhaps send to GUIElement if position OR original position is in bounds
    // when dragging
    if (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(mGUIRootElement->getBounds(), aMouseEvent->mMouseOriginalPosition))
    {
        lEventSignal = mGUIRootElement->handleMouseEvent(aMouseEvent);
    }
    else if (Util::inBounds(mGUIRootElement->getBounds(), aMouseEvent->mMousePosition))
    {
        lEventSignal = mGUIRootElement->handleMouseEvent(aMouseEvent);
    }
    else
    {
        // if the MouseEvent is not in bounds of the root GUIElement
        // then it won't be for any of it's children
        return kEventIgnored;
    }

    // nobody has accepted this event yet
    if (kEventIgnored == lEventSignal && !aMouseEvent->mAcceptor)
    {
        for (UInt32 i = 0; i < mGUIRootElement->getChildren()->size(); i++)
        {
            lEventSignal = handleMouseEvent((*mGUIRootElement->getChildren())[i], 
                                            aMouseEvent);
            if (kEventAbsorbed == lEventSignal)
            {
                break;
            }
        }
    }

    if (kMouseMoved != aMouseEvent->mMouseEventType)
    {
        MenuController * lMenuController = MenuController::getMenuController();
        lMenuController->setNoMenuFocused();
        setFocusedElement(aMouseEvent->mAcceptor); // can be NULL
    }

    return lEventSignal;
}

EventSignal
Window::handleMouseEvent(GUIElement* aGUIElement, MouseEvent* aMouseEvent)
{
    EventSignal lEventSignal = kEventIgnored;

    if (!aGUIElement)
    {
        return kEventIgnored;
    }

    if (kMouseDragged == aMouseEvent->mMouseEventType &&
        Util::inBounds(aGUIElement->getBounds(), aMouseEvent->mMouseOriginalPosition))
    {
        lEventSignal = aGUIElement->handleMouseEvent(aMouseEvent);
    }
    else if (Util::inBounds(aGUIElement->getBounds(), aMouseEvent->mMousePosition))
    {
        lEventSignal = aGUIElement->handleMouseEvent(aMouseEvent);
    }
    else
    {
        // if the MouseEvent is not in bounds of the root GUIElement
        // then it won't be for any of it's children
        return kEventIgnored;
    }

    // nobody has accepted this event yet
    if (kEventIgnored == lEventSignal && !aMouseEvent->mAcceptor)
    {
        for (UInt32 i = 0; i < aGUIElement->getChildren()->size(); i++)
        {
            lEventSignal = handleMouseEvent((*aGUIElement->getChildren())[i], 
                                            aMouseEvent);
            if (kEventAbsorbed == lEventSignal)
            {
                break;
            }
        }
    }

    // don't change focus on plain mouse motion
    if (kMouseMoved != aMouseEvent->mMouseEventType)
    {
        setFocusedElement(aMouseEvent->mAcceptor); // can be NULL
    }

    return lEventSignal;
}

Status
Window::resize(UInt32 aWidth, UInt32 aHeight)
{
    Status lStatus = kSucceeded;

    // default is to begin at (0,0) of viewport
    Rectangle2D lBounds = {0, 0, 0, 0};
    bool lForceReshape = false;
    if (mRespectMinimumSafeSize)
    {
        Point2D lMinimumSize = getMinimumSafeSize();
        // BUG 0013 causes + 2
        UInt32  lMinW = lMinimumSize.x + 2;
        UInt32  lMinH = lMinimumSize.y + 2;
        if (aWidth < lMinW || aHeight < lMinH)
        {
            if (aHeight < lMinH)
            {
                lBounds.y = SInt32(aHeight) - SInt32(mMinimumSafeSize.y);
            }
            aWidth = max(aWidth, lMinW);
            aHeight = max(aHeight, lMinH);
            lForceReshape = true;
        }
    }
    mWindowSize.x = aWidth;
    mWindowSize.y = aHeight;
    lBounds.w = mWindowSize.x;
    lBounds.h = mWindowSize.y;

#   ifdef DEBUG_WINDOW
    fprintf(stderr, 
            "Window::resize passing resize to LayoutManager (%dx%d @ %d,%d)\n",
            lBounds.w,
            lBounds.h,
            lBounds.x,
            lBounds.y);
#   endif

    // force resize first time
    LayoutManager::resize(mGUIRootElement, lBounds, mFirstResize);
    mFirstResize = false;

    // reshape to safe size if window is too small
    if (lForceReshape)
    {
        glutReshapeWindow(mWindowSize.x, mWindowSize.y);
    }
        
    lStatus = draw();

    return lStatus;
}

Status
Window::fullscreen()
{
    // keep previous size for restore
    mPreviousWindowSize.x = mWindowSize.x;
    mPreviousWindowSize.y = mWindowSize.y;
    mIsFullScreen = true;
    // tell GL to go fullscreen
    glutFullScreen();
    glutSetWindowTitle(mName);
    // NOTE: resize will be called by the reshape callback 
    //       function with the new dimensions, so there's no
    //       need for a manual resize call here.
    return kSucceeded;
}

Status
Window::restore()
{
    // reshape to previous size
    mIsFullScreen = false;
    glutReshapeWindow(mPreviousWindowSize.x, mPreviousWindowSize.y);
    // XXX BUG This doesn't work for some reason
    glutSetWindowTitle(mName);
    // NOTE: resize will be called by the reshape callback 
    //       function with the new dimensions, so there's no
    //       need for a manual resize call here.
    return kSucceeded;
}

Status
Window::close()
{
    return kSucceeded;
}

Status
Window::draw()
{
    Status lStatus;
    lStatus = mGUIRootElement->drawTree();  // draw all GUIElements, beginning at mGUIRootElement
    return lStatus;
}

SInt32
Window::getID()
{
    return mID;
}

Point2D
Window::getSize()
{
    return mWindowSize;
}

Point2D
Window::getMinimumSafeSize(void)
{
    updateMinimumSafeSize();
#   ifdef DEBUG_BOUNDS
    fprintf(stderr, 
            "Window::getMinimumSafeSize - %d x %d\n", 
            mMinimumSafeSize.x, 
            mMinimumSafeSize.y);
#   endif
    return mMinimumSafeSize;
}

void
Window::updateMinimumSafeSize(void)
{
    // TODO find out if an update is necessary
    if (mGUIRootElement)
    {
        mMinimumSafeSize.x = mGUIRootElement->getMinimumSafeWidth();
        mMinimumSafeSize.y = mGUIRootElement->getMinimumSafeHeight();
    }
    else
    {
        mMinimumSafeSize.x = 0;
        mMinimumSafeSize.y = 0;
    }
}

bool 
Window::getRespectMinimumSafeSize(void)
{
    return mRespectMinimumSafeSize;
}

void
Window::setRespectMinimumSafeSize(bool aRespectMinimumSafeSize)
{
    mRespectMinimumSafeSize = aRespectMinimumSafeSize;
}

GUIElement*
Window::getRootGUIElement(void)
{
    return mGUIRootElement;
}

void
Window::updateTabOrder(GUIElement* aRoot)
{
    deque< GUIElement* > * lChildren = aRoot->getChildren();
    if (kFillDirectionVertical == aRoot->getFillDirection())
    {
        for (SInt32 i = lChildren->size() - 1; i >= 0; --i)
        {
            GUIElement* lElement = (*lChildren)[i];
            if (lElement->isInTabOrder() && lElement->isVisible())
            {
                mTabElements.push_back(lElement);
            }
            updateTabOrder(lElement);
        }
    }
    else
    {
        for (UInt32 i = 0; i < lChildren->size(); ++i)
        {
            GUIElement* lElement = (*lChildren)[i];
            if (lElement->isInTabOrder() && lElement->isVisible())
            {
                mTabElements.push_back(lElement);
            }
            updateTabOrder(lElement);
        }
    }
}

void
Window::addOverlayElement(GUIElement* aOverlayElement)
{
    if (aOverlayElement)
    {
        mOverlayElements.push_back(aOverlayElement);
    }
}

Status 
Window::removeOverlayElement(GUIElement* aGUIElement)
{
    auto lOverlayElement = mOverlayElements.begin();
    for (; lOverlayElement != mOverlayElements.end(); ++lOverlayElement)
    {
        if (*lOverlayElement == aGUIElement)
        {
            mOverlayElements.erase(lOverlayElement);
            return kSucceeded;
        }
    }
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "Window::removeOverlayElement - GUIElement not found\n");
    #endif
    return kFailed;
}

void
Window::updateTabOrder(void)
{
    if (!mGUIRootElement)
    {
        return;
    }
    mTabElements.clear();
    updateTabOrder(mGUIRootElement);
}

void
Window::setFocusedElement(GUIElement* aGUIElement)
{
    if (!aGUIElement)
    {
        if (mFocusedElement)
        {
            mFocusedElement->setFocus(false);
            mFocusedElement = 0;
            mFocusedIndex = -1;
        }
        return;
    }
    
    if (aGUIElement == mFocusedElement)
    {
        return;
    }
    
    GUIElement* lElement = 0;
    UInt32      lIndex = 0;

    // linear search for the right element
    auto lChild = mTabElements.begin();
    for (; lChild != mTabElements.end(); ++lChild)
    {
        if ((*lChild) == aGUIElement)
        {
            lElement = *lChild;
            break;
        }
        lIndex++;
    }  

    // drop focus from previous focused element
    if (mFocusedElement)
    {
        mFocusedElement->setFocus(false);
    }

    // tell the new element it is focused
    if (lElement)
    {
        lElement->setFocus(true);
        mFocusedIndex = lIndex;
        mFocusedElement = lElement;
    }
    else
    {
        aGUIElement->setFocus(true);
        mFocusedElement = aGUIElement;
    }
}

void 
Window::setFocusColor(Color aColor)
{
    if (mGUIRootElement)
    {
        mGUIRootElement->setFocusColor(aColor, true);
    }
}

Color
Window::getFocusColor()
{
    if (mGUIRootElement)
    {
        return mGUIRootElement->getFocusColor();
    }
    #ifdef DEBUG_BOUNDS
    fprintf(stderr, "Window::getFocusColor - No GUIRootElement, returning default focus color.");
    #endif
    return GUIElement::kDefaultFocusColor;
}

GUIElement*
Window::getFocusedElement(void)
{
    return mFocusedElement;
}

GUIElement*
Window::focusNextElement(void)
{
    if (mFocusedElement)
    {
        mFocusedElement->setFocus(false);
    }
    if (mTabElements.size())
    {
        mFocusedIndex++;
        if (UInt32(mFocusedIndex) >= mTabElements.size())
        {
            // wrap
            mFocusedIndex = 0;
        }
        mFocusedElement = mTabElements[mFocusedIndex];
        mFocusedElement->setFocus(true);
    }
    return mFocusedElement;
}

GUIElement*
Window::focusPrevElement(void)
{
    if (mFocusedElement)
    {
        mFocusedElement->setFocus(false);
    }
    if (mTabElements.size())
    {
        mFocusedIndex--;
        if (mFocusedIndex < 0)
        {
            // wrap
            mFocusedIndex = mTabElements.size() - 1;
        }
        mFocusedElement = mTabElements[mFocusedIndex];
        mFocusedElement->setFocus(true);
    }
    return mFocusedElement;
}

EventSignal 
Window::handleKeyEventInternal(KeyEvent* aKeyEvent)
{
#   ifdef DEBUG_EVENTS
    fprintf(stderr, "Window::handleKeyEvent - Window %s got KeyEvent. Key: %d\n", mName, aKeyEvent->mKey);
#   endif
    Key lKey = aKeyEvent->mKey;
    if (kKeyTypeNormal == aKeyEvent->mKeyType &&
        kKeyPressed == aKeyEvent->mKeyEventType)
    {
        switch (lKey)
        {
            // F is default for fullscreen
            case 'f': 
            case 'F':
                if (this->isFullScreen())
                {
                    this->restore();
                }
                else
                {
                    this->fullscreen();
                }
                return kEventAbsorbed;
            // Tab is default for sending focus to next element
            // 9 == tab
            case 9:
                focusNextElement();
                return kEventAbsorbed;
            // Shift-Tab is default for sending focus to previous element
            // 25 == tab
            case 25:
                focusPrevElement();
                return kEventAbsorbed;
        }
    }
    return kEventIgnored;
}

void
Window::updateAnimation(GUIElement* aRoot)
{
    // ordinary elements
    deque< GUIElement* > * lChildren = aRoot->getChildren();
    deque< GUIElement* >::const_iterator lChild = lChildren->begin();
    for (; lChild != lChildren->end(); ++lChild)
    {
        if ((*lChild) && (*lChild)->isAnimatable())
        {
            (*lChild)->updateAnimation();
        }
        // update the childs children.
        if (*lChild)
        {
            updateAnimation((*lChild));
        }
    }  
}

void
Window::updateAnimation(void)
{
    if (!mGUIRootElement)
    {
        return;
    }
    updateAnimation(mGUIRootElement);

    // overlay elements
    auto lOverlayElement = mOverlayElements.begin();
    for (; lOverlayElement != mOverlayElements.end(); ++lOverlayElement)
    {
        if ((*lOverlayElement)->isAnimatable())
        {
            (*lOverlayElement)->updateAnimation();
        }
    }  
}

