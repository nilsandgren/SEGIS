//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  MenuController: 
//      Implementation
//

#include <SEGIS/MenuController.h>
#include <SEGIS/GUIMenuItem.h>

MenuController::MenuController()
    : mFocusedMenuItem(0)
    , mFocusedMenuParent(0)
    , mFocusedMenuRoot(0)
{
}

MenuController::~MenuController()
{
}

MenuController*
MenuController::mTheMenuController = 0;

MenuController*
MenuController::getMenuController()
{
    if (0 == mTheMenuController)
    {
        mTheMenuController = new MenuController();
    }
    return mTheMenuController;
}

void 
MenuController::setFocusedMenuItem(GUIMenuItem* aMenuItem)
{
    // play it safe
    if (!aMenuItem)
    {
        return;
    }
    // first call
    if (!mFocusedMenuRoot && !mFocusedMenuParent && !mFocusedMenuItem)
    {
        mFocusedMenuItem = aMenuItem;
        mFocusedMenuParent = aMenuItem;
        mFocusedMenuRoot = aMenuItem;
        aMenuItem->expandSubMenuItems();
        return;
    }
    // no need to do anything if same element
    if (aMenuItem != mFocusedMenuItem)
    {
        // check at which level focus has changed
        GUIMenuItem* lNewMenuRoot = aMenuItem->getRootMenuItem();
        if (!lNewMenuRoot) // the root's root is 0
        {
            lNewMenuRoot = aMenuItem;
        }
        GUIMenuItem* lNewMenuParent = aMenuItem->getParentMenuItem();
        if (!lNewMenuParent) 
        {
            lNewMenuParent = aMenuItem;
        }

        // new root?  (user has switched menu completely)
        if (lNewMenuRoot != mFocusedMenuRoot)
        {
            if (mFocusedMenuRoot)
            {
                mFocusedMenuRoot->collapseSubMenuItems();
            }
            aMenuItem->expandSubMenuItems();
        }
        // new parent. collapse old one (if new parent != old item), expand new element.
        else if (mFocusedMenuParent != lNewMenuParent)
        {
            if (mFocusedMenuItem != lNewMenuParent && mFocusedMenuParent)
            {
                mFocusedMenuParent->collapseSubMenuItems();
            }
            if (aMenuItem->getSubMenuItems()->size())
            {
                aMenuItem->expandSubMenuItems();
            }
        }
        // new element at same level. collapse old, expand new.
        else
        {
            if (lNewMenuRoot != mFocusedMenuItem && mFocusedMenuItem)
            {
                mFocusedMenuItem->collapseSubMenuItems();
            }
            if (aMenuItem->getSubMenuItems()->size())
            {
                aMenuItem->expandSubMenuItems();
            }
        }
        if (mFocusedMenuItem)
        {
            mFocusedMenuItem->setFocus(false);
        }
        mFocusedMenuItem = aMenuItem;
        mFocusedMenuParent = lNewMenuParent;
        mFocusedMenuRoot = lNewMenuRoot;
    }
}

void
MenuController::setNoMenuFocused()
{
    if (mFocusedMenuRoot)
    {
        mFocusedMenuRoot->collapseSubMenuItems();
    }
    mFocusedMenuItem = 0;
    mFocusedMenuRoot = 0;
}
