//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUISplitView: 
//      Subclass of GUIElement that contains two GUIElements with a split in between.
//      The split can be either vertical or horizontal. If the FillDirection of the 
//      GUISplitView is horizontal, then the split will be vertical and vice versa.
//
//      .------------------------------------------.
//      |                       | |                |
//      |                       |S|                |
//      |                       |p|                |
//      |   Element1            |l|  Element2      |
//      |                       |i|                |
//      |                       |t|                |
//      |                       | |                |
//      '------------------------------------------'
//
//     The size of Element1 and Element2 can be changed by moving the Split.
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

class GUISplitView : public GUIElement
{
    // variables
    protected:
        Rectangle2D mChildBoundsBackup[2];
        Rectangle2D mHandleBounds;
        Rectangle2D mHandleBoundsBackup;

    // functions
    private:
        void updateHandleBounds(void);
        void backupHandleBounds(void);
        void backupChildBounds(void);

    public:
        GUISplitView(UInt32      aID,
                     Rectangle2D aBounds);

        GUISplitView(UInt32         aID,
                     Rectangle2D    aBounds,
                     UInt32         aPadding,
                     SizeConstraint aSizeConstraint,
                     FillDirection  aFillDirection,
                     Alignment      aAlignment);

        virtual ~GUISplitView();

        virtual EventSignal handleMouseEvent(MouseEvent*);

        virtual Status draw(void);

        // makes sure at most two children are added
        // children must have kChangable as SizeConstraint in FillDirection
        virtual Status addChild(GUIElement* aGUIElement);
        virtual Status addChildBack(GUIElement* aGUIElement);

        virtual void setBounds(Rectangle2D aBounds);

        virtual void setFillDirection(FillDirection aFillDirection);

        virtual bool isInTabOrder(void)
        {
            return false;
        }
};
