//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIComboBoxItem:
//      Subclass to GUIOptionElement and Observer
//      The GUIComboBoxItem is contained in a GUIComboBox and has a
//      BooleanOption attached to it.
//
//       --------------------
//      |                    |
//      | I t e m L a b e l  |
//      |                    |
//       --------------------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "GUIMenuItem.h"
#include "GUIComboBox.h"
#include "Observer.h"

class BooleanOption;

class GUIComboBoxItem : public GUIOptionElement, public Observer, public TextElement
{
    // variables
    private:
        bool           mIsSelected;
        BooleanOption* mBooleanOption;
        bool           mInitialUpdateDone;
        GUIComboBox*   mParentComboBox;

        Color          mLabelColor;

        // animation stuff
        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

    // functions
    protected:
    public:
        GUIComboBoxItem(UInt32          aID,
                        Rectangle2D     aBounds,
                        const WString & aLabelText);

        virtual ~GUIComboBoxItem();

        virtual Status draw(void);

        virtual void setParent(GUIComboBox* aGUIComboBox);

        // virtual void setBounds(Rectangle2D aBounds);

        // set the value of this GUIComboBoxItem
        virtual void setValue(bool aValue);

        virtual void setVisible(bool aIsVisible);

        virtual void setFocus(bool aIsFocused);

        // set/get the label color
        virtual Status setLabelColor(Color aColor);
        virtual Color  getLabelColor(void);

        // attach a boolean option to the checkbox
        virtual Status setBooleanOption(BooleanOption* aBooleanOption);
        virtual BooleanOption* getBooleanOption(void);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a BooleanOption
        virtual Status update(const Observable* aOption);

        // take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent* aKeyEvent);
        virtual EventSignal handleMouseEvent(MouseEvent* aMouseEvent);

        // children cannot be added to this class
        virtual Status addChild(GUIElement* aGUIElement);
        virtual Status addChild(Window* aParentWindow, GUIElement* aGUIElement);

        virtual bool updateAnimation(void);
};
