//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUILayerPanel: 
//      Subclass to GUIElement
//      An panel containing GUIElements in different layers. Only one of these layers are
//      visible at a time.
//
//       ____ ____ ____
//      | L0 | L1 | L2 |
//      |----'    '--------------.
//      |                        |
//      |                        |
//      |     Layer 1            |
//      |                        |
//      |                        |
//      |                        |
//      '------------------------'
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIElement.h"
#include "TextElement.h"
#include <deque>

using namespace std;

class GUILayerPanelButton;

class GUILayerPanel : public GUIElement, public TextElement
{
    // variables
    private:
        deque< GUIElement* >          mLayers;             // the contained layers
        GUIElement*                   mButtonArea;         // buttons for selecting layer
        GUIElement*                   mLayerArea;          // area for the visible layer
        GUIElement*                   mSelectedLayer;
        deque< GUILayerPanelButton* > mLayerButtons;
        SInt32                        mSelectedLayerIndex; // currently selected index

        SInt32                        mLayerButtonHeight;  // height of button row
        Color                         mTextColor;          // label text color

    // functions
    private:
        void updateButtonIndices(void);
    public:
        GUILayerPanel(UInt32      aID,
                      Rectangle2D aBounds);

        virtual ~GUILayerPanel();

        // contained GUIElements are stored in mLayers. only one of these are stored in
        // mChildren at a time so we need to overload some GUIElement functions
        virtual Status addChild(GUIElement* aGUIElement);
        virtual Status addChild(Window* aParentWindow, GUIElement* aGUIElement);
        virtual Status removeChild(GUIElement* aGUIElement);

        virtual void setAlignment(Alignment aAlignment);

        // control the layers
        virtual Status setSelectedLayer(UInt32 aLayerIndex);
        virtual Status selectNextLayer(void);
        virtual Status selectPrevLayer(void);
        virtual GUIElement* getSelectedLayer(void);
        virtual SInt32 getSelectedLayerIndex(void);
        virtual Status setLayerLabelText(UInt32 aLayerIndex,
                                         const WString & aLabelText);

        // set/get label color
        virtual void setLabelColor(Color aColor);
        virtual Color getLabelColor(void);

        virtual void setColor(Color aColor);

        virtual void setFocusColor(Color aColor, bool aRecurse);

        // draws this GUILayerPanel
        virtual Status draw(void);

        virtual bool isInTabOrder(void) { return false; }

        virtual UInt32 getMinimumSafeWidth();
        virtual UInt32 getMinimumSafeHeight();
        virtual void resizeLayerPanelButtons();
};
