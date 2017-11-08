//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIMenuBar: 
//      Implementation
//

#include <SEGIS/GUIMenuBar.h>
#include <SEGIS/Window.h>
#include <SEGIS/DisplayManager.h>

GUIMenuBar::GUIMenuBar(UInt32      aID,
                       Rectangle2D aBounds)
    : GUIElement(aID, aBounds)
    , mCurrentTopMenuItem(0)
    , mCurrentSubMenuItem(0)
{
}

GUIMenuBar::GUIMenuBar(UInt32        aID,
                       Rectangle2D   aBounds,
                       UInt32        aPadding,
                       SizeConstraint aSizeConstraint,
                       FillDirection aFillDirection,
                       Alignment     aAlignment)
    : GUIElement(aID, aBounds, aPadding, aSizeConstraint, aFillDirection, aAlignment)
    , mCurrentTopMenuItem(0)
    , mCurrentSubMenuItem(0)
{
}

GUIMenuBar::~GUIMenuBar()
{
    mMenus.clear();
}

Status
GUIMenuBar::draw(void)
{
    auto lMenuItem = mMenus.begin();
    for (; lMenuItem != mMenus.end(); ++lMenuItem)
    {
        (*lMenuItem)->draw();
    }
    return kSucceeded;
}

Status
GUIMenuBar::addChild(GUIMenuItem* aGUIMenuItem)
{
    Status lStatus = GUIElement::addChild(aGUIMenuItem);
    if (kSucceeded != lStatus)
    {
        return lStatus;
    }
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mMenus.push_back(aGUIMenuItem);
    }
    else
    {
        mMenus.push_front(aGUIMenuItem);
    }

    aGUIMenuItem->setIsInTabOrder(true);
    if (mParentWindow)
    {
        mParentWindow->addOverlayElement(aGUIMenuItem);
    }

    aGUIMenuItem->setVisible(true);

    mCurrentTopMenuItem = aGUIMenuItem;
    mCurrentSubMenuItem = aGUIMenuItem;

    return lStatus;
}

Status 
GUIMenuBar::addChild(Window* aParentWindow, GUIMenuItem* aGUIMenuItem)
{
    Status lStatus = GUIElement::addChild(aParentWindow, aGUIMenuItem);
    if (kSucceeded != lStatus)
    {
        return lStatus;
    }
    if (mFillDirection == kFillDirectionHorizontal)
    {
        mMenus.push_back(aGUIMenuItem);
    }
    else
    {
        mMenus.push_front(aGUIMenuItem);
    }
    mCurrentTopMenuItem = aGUIMenuItem;
    mCurrentSubMenuItem = aGUIMenuItem;
    return lStatus;
}

Status 
GUIMenuBar::removeChild(GUIMenuItem* aGUIMenuItem)
{
    Status lStatus = GUIElement::removeChild(aGUIMenuItem);
    if (kSucceeded != lStatus)
    {
        return lStatus;
    }
    auto lMenu = mMenus.begin();
    for (; lMenu != mMenus.end(); ++lMenu)
    {
        if (*lMenu == aGUIMenuItem)
        {
            mMenus.erase(lMenu);
            return kSucceeded;
        }
    }
    #ifdef DEBUG_GUIELEMENT
    fprintf(stderr, "GUIMenuBar::removeChild - GUIMenuItem not found\n");
    #endif
    return kFailed;
}

EventSignal
GUIMenuBar::handleKeyEvent(KeyEvent* /* aKeyEvent */)
{
    return kEventIgnored;
}

EventSignal
GUIMenuBar::handleMouseEvent(MouseEvent* /* aMouseEvent */)
{
    return kEventIgnored;
}
