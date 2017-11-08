//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  MenuController: 
//      Keeps track of which menu item was selected last and so on
//      Singleton implementation
//  

#pragma once

#include "Types.h"
#include "GUIElement.h"

class GUIMenuItem;

class MenuController
{
    // variables
    private:
        static MenuController*  mTheMenuController;
        GUIMenuItem*            mFocusedMenuItem;
        GUIMenuItem*            mFocusedMenuParent;
        GUIMenuItem*            mFocusedMenuRoot;

    // functions
    private:
        MenuController();
    public:
        ~MenuController();
        void setFocusedMenuItem(GUIMenuItem* aMenuItem);
        void setNoMenuFocused();

        static MenuController* getMenuController();
};
