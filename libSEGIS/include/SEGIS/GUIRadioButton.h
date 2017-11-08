//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIRadioButton:
//      Subclass to GUIOptionElement
//      The GUIRadioButton is used for mutually exclusive options and is contained in a 
//      RadioButtonGroup that controls which GUIRadioButton is enabled.
//      A BooleanOption can be attached to the GUIRadioButton. The option will be updated
//      when the GUIRadioButton is enabled or disabled.
//
//       ------
//      | ---- |
//      || ## ||
//      || ## ||
//      | ---- |
//       ------

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "Observer.h"

class BooleanOption;
class GUIRadioButton;
class RadioButtonGroup
{
    // variables
    protected:
        deque< GUIRadioButton* > mRadioButtons;

    // functions
    protected:
    public:
        RadioButtonGroup();
        virtual ~RadioButtonGroup();

        // add a radio button
        virtual Status addRadioButton(GUIRadioButton* aRadioButton);
        // remove a radio button - assumes that only one pointer to the child exists in array
        virtual Status removeRadioButton(GUIRadioButton* aRadioButton);

        // deactivate GUIRadioButtons != aRadioButton
        virtual Status deactivateOthers(GUIRadioButton* aRadioButton);
};

class GUIRadioButton : public GUIOptionElement, public Observer
{
    // variables
    private:
        bool              mIsChecked; 
        Color             mTickColor;
        BooleanOption*    mBooleanOption;

        UInt32            mBoxSize;
        Rectangle2D       mBoxBounds;

        bool              mSynchedWithOption;

        RadioButtonGroup* mRadioButtonGroup;

        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

    // functions
    protected:
    public:
        GUIRadioButton(UInt32      aID,
                       Rectangle2D aBounds);

        virtual ~GUIRadioButton();

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // override setBounds to assure checkbox coordinates
        virtual void setBounds(Rectangle2D aBounds);

        // get/set box size
        virtual void setBoxSize(UInt32 aBoxSize);
        virtual UInt32 getBoxSize(void);
        
        // set the value of the checkbox. true => checked, false => unchecked
        virtual void setValue(bool aValue);

        // set/get the tick color
        void setTickColor(Color aColor);
        Color getTickColor(void);

        // attach a boolean option to the checkbox
        virtual Status setBooleanOption(BooleanOption* aBooleanOption);
        virtual BooleanOption* getBooleanOption(void);

        // draws this GUIRadioButton
        virtual Status draw(void);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a BooleanOption
        virtual Status update(const Observable* aOption);

        // set group belonging
        virtual Status setGroup(RadioButtonGroup* aGroup);

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);
};
