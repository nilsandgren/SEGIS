//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIGradientPanel: 
//      Subclass of GUIElement that can be given a unique color for each vertex.
//

#pragma once

#include "Types.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "Events.h"
#include "Option.h"
#include "GUIElement.h"

#include <deque>
using namespace std;

class GUIGradientPanel : public GUIElement
{
    // variables
    protected:
        Color   mColor0;
        Color   mColor1;
        Color   mColor2;
        Color   mColor3;

    // functions
    public:
        GUIGradientPanel(UInt32      aID,
                         Rectangle2D aBounds);

        GUIGradientPanel(UInt32        aID,
                         Rectangle2D   aBounds,
                         UInt32        aPadding,
                         SizeConstraint aSizeConstraint,
                         FillDirection aFillDirection,
                         Alignment     aAlignment);

        virtual ~GUIGradientPanel();

        virtual Status draw(void);

        const Color getColor(void)
        {
            return mColor;
        }
        virtual void setColor(Color aColor);

        // sets the color for each vertex in the panel
        virtual void setColor(Color aColor0,
                              Color aColor1,
                              Color aColor2,
                              Color aColor3);

        virtual bool isInTabOrder(void)
        {
            return false;   
        }
};
