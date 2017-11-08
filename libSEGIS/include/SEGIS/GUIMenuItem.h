//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIMenuItem:
//      A menu item that can hold a set of sub-menu items.
//      Since drawing/layout of menu items is a bit special addChild and draw are overloaded
//      in this class to make LayoutManager and DisplayManager less confused
//
//       --------------------
//      |                    |
//      |  M e n u I t e m   |
//      |                    |
//       --------------------
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "GUIElement.h"
#include "GUILabel.h"
#include <deque>

class GUIMenuItem : public GUILabel
{
    // variables
    private:
        Point2D               mChildPosition;
        Point2D               mLargestChildSize;
        bool                  mIsExpanded; // tells if sub-menu is expanded
        SInt08                mPulseMax;
        SInt08                mPulseVal;
        SInt08                mPulseSign;
    protected:
        GUIMenuItem*          mRootMenuItem;
        GUIMenuItem*          mParentMenuItem;
        deque< GUIMenuItem* > mSubMenuItems;

    // functions
    protected:
        // set the position of the sub-menu items for correct display
        virtual void positionSubMenuItems();
        // set the size (width only probably) for sub-menu items
        virtual void resizeSubMenuItems();

    public:
        GUIMenuItem(UInt32          aID,
                    Rectangle2D     aBounds,
                    const WString & aString);

        virtual ~GUIMenuItem();

        // expand THE NEXT LEVEL of submenu items i.e. position and make them visible
        virtual void expandSubMenuItems();
        // collapse ALL LEVELS of submenu items below i.e. make them non-visible
        virtual void collapseSubMenuItems();
        // get sub-menu items
        deque< GUIMenuItem* > * getSubMenuItems(void);

        virtual bool isExpanded(void);

        virtual void setFocus(bool aIsFocused);

        // parent menu item
        virtual GUIMenuItem* getParentMenuItem(void)
        {
            return mParentMenuItem;
        }
        virtual void setParentMenuItem(GUIMenuItem* aMenuItem)
        {
            mParentMenuItem = aMenuItem;
        }

        // root menu item
        virtual GUIMenuItem* getRootMenuItem(void)
        {
            return mRootMenuItem;
        }
        virtual void setRootMenuItem(GUIMenuItem* aMenuItem)
        {
            mRootMenuItem = aMenuItem;
        }
        
        // adds a menu item to the mSubMenuItems array
        virtual Status addChild(GUIMenuItem* aMenuItem);
        // an alias for addChild
        virtual Status addSubMenuItem(GUIMenuItem* aMenuItem);
        // remove a menu item from mSubMenuItems
        // assumes that only one pointer to the menu item  exists in array
        virtual Status removeSubMenuItem(GUIMenuItem* aMenuItem);

        // do whatever should be done when the menu item is selected
        virtual Status doAction();

        // overloadng draw and setBounds is a good idea since mChildren isn't used in the 
        // ordinary way 
        virtual Status draw(void);
        virtual void setBounds(Rectangle2D aBounds);

        virtual bool updateAnimation(void);

        // so top level GUIMenuItems can be included in tab order
        virtual void setIsInTabOrder(bool aIsInTabOrder) { mIsInTabOrder = aIsInTabOrder; }

        virtual void setVisible(bool aIsVisible);

        // take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent* aKeyEvent);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);
};
