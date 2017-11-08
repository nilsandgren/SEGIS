//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  DisplayManager: 
//      Handles updating of the display. Singleton implementation.
//  

#pragma once

#include "Types.h"
#include "GUIElement.h"

class DisplayManager
{
    // variables
    private:
        static DisplayManager* mTheDisplayManager;

    // functions
    private:
        DisplayManager();
    public:
        ~DisplayManager();
        // update this GUIElement and those below 
        // use this function for local updates in the GUI caused
        // by e.g. a mouse event. complete updates of a Window
        // when scaling the view are handled automatically.
        void requestUpdate(GUIElement* aRoot);

        static DisplayManager* getDisplayManager();
};
