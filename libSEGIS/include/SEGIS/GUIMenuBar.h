//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIMenuBar: 
//      Subclass of GUIElement that hold a number of root GUIMenuItems
//
//       ----------------------
//      | -------------------- |
//      || menu | menu | menu ||
//      | -------------------- |
//        --------------------  
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "Option.h"
#include "GUIElement.h"
#include "GUIMenuItem.h"

#include <deque>
using namespace std;

class GUIMenuBar : public GUIElement
{
    // variables
    protected:
        GUIMenuItem*          mCurrentTopMenuItem;
        GUIMenuItem*          mCurrentSubMenuItem;
        deque< GUIMenuItem* > mMenus;

    // functions
    public:
        GUIMenuBar(UInt32      aID,
                   Rectangle2D aBounds);

        GUIMenuBar(UInt32         aID,
                   Rectangle2D    aBounds,
                   UInt32         aPadding,
                   SizeConstraint aSizeConstraint,
                   FillDirection  aFillDirection,
                   Alignment      aAlignment);

        virtual ~GUIMenuBar();

        virtual Status draw(void);

        // adds a top-level GUIMenuItem to children, but also to mMenus
        virtual Status addChild(GUIMenuItem* aGUIMenuItem);
        virtual Status addChild(Window* aParentWindow, GUIMenuItem* aGUIMenuItem);

        // removes a top-level GUIMenuItem from mChildren and mMenus
        virtual Status removeChild(GUIMenuItem* aGUIMenuItem);

        virtual EventSignal handleKeyEvent(KeyEvent* aKeyEvent);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);
};
