//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIOptionElement: 
//      Abstract base class
//      A GUIElement which might be connected to an option
//

#pragma once

#include "GUIElement.h"

class GUIOptionElement : public GUIElement
{
    // variables
    protected:

    // functions
    protected:
        GUIOptionElement();
                   
    public:
        GUIOptionElement(UInt32      aID,
                         Rectangle2D aBounds);

        GUIOptionElement(UInt32        aID,
                         Rectangle2D   aBounds,
                         UInt32        aPadding,
                         SizeConstraint aSizeConstraint,
                         FillDirection aFillDirection,
                         Alignment     aAlignment);

        virtual ~GUIOptionElement();
        
        // BUG this might solve bug 0004 in a kind of ugly way
        virtual void updateFromOption() = 0;
};
