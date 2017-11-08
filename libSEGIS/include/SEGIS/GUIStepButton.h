//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIStepButton: 
//      Subclass to GUIButton
//      A button with a little arrow on it
//
//       ------------
//      | ---------- |
//      ||    ##    ||
//      ||   ####   ||
//      || ######## ||
//      ||          ||
//      | ---------- |
//       ------------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIButton.h"
#include <deque>

using namespace std;

class GUIStepButton : public GUIButton
{
    // variables
    private:
        Direction mArrowDirection;
    // functions
    public:
        GUIStepButton(UInt32        aID,
                      Rectangle2D   aBounds,
                      UInt32        aPadding,
                      SizeConstraint aSizeConstraint,
                      FillDirection aFillDirection,
                      Alignment     aAlignment);

        virtual ~GUIStepButton();

        // draws this GUIStepButton
        virtual Status draw(void);

        // decide which way to point the arrow
        void setDirection(Direction aDirection);

        virtual bool isInTabOrder(void);
};
