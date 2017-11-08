//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  LayoutManager: 
//      Handles size and positioning of GUI components
//

#pragma once

#include "GUIElement.h"

class LayoutManager
{
    private:
        static void resizeChildrenFillDirectionHorizontal(GUIElement* aRoot, 
                                                          Rectangle2D aNewBounds,
                                                          bool        aForceResize = false);
        static void resizeChildrenFillDirectionVertical(GUIElement* aRoot, 
                                                        Rectangle2D aNewBounds,
                                                        bool        aForceResize = false);
        static void resizeChildrenFillDirectionNull(GUIElement* aRoot,
                                                    Rectangle2D aNewBounds,
                                                    bool        aForceResize = false);
    public:
        LayoutManager();
        ~LayoutManager();
        // resizes and positions all GUIElements contained in aRoot according to their 
        // SizeConstraints in combination with aRoot's FillDirection and Alignment.
        // aForceResize forces recalculation of childrens' size and position even when
        // aNewBounds is identical to previous bounds of aRoot
        static void resize(GUIElement* aRoot, Rectangle2D aNewBounds, bool aForceResize = false);
};
