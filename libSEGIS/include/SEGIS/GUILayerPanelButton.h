//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILayerPanelButton: 
//      Subclass to GUIButton
//      A button for selecting layer in a GUILayerPanel. 
//      Only used internally by GUILayerPanel.
//
//       -------------  
//      | Layer Label | 
//      |             | 
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIButton.h"
#include <deque>

class GUILayerPanel;

using namespace std;

class GUILayerPanelButton : public GUIButton
{
    // variables
    private:
        // the index of the layer this GUILayerPanelButton controls
        SInt32          mLayerIndex;
        GUILayerPanel*  mLayerPanel;
    // functions
    public:
        GUILayerPanelButton(UInt32         aID,
                            Rectangle2D    aBounds,
                            UInt32         aPadding,
                            SizeConstraint aSizeConstraint,
                            FillDirection  aFillDirection,
                            Alignment      aAlignment);

        GUILayerPanelButton(UInt32        aID,
                            Rectangle2D   aBounds);

        virtual ~GUILayerPanelButton();

        void setLayerPanel(GUILayerPanel* aGUILayerPanel);
        void setLayerIndex(SInt32 aIndex);
    
        // draws this GUILayerPanelButton
        virtual Status draw(void);

        virtual Status doAction(void);

        virtual bool isInTabOrder(void);
};
