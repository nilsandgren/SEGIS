//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUICheckBox:
//      Subclass to GUIOptionElement and Observer
//      A CheckBox that can be checked. The GUICheckBox can have a BooleanOption
//      attached to it, which is updated when the GUICheckBox is checked/unckecked.
//
//       ------
//      | ---- |
//      || \/ ||
//      || /\ ||
//      | ---- |
//       ------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "Observer.h"

class BooleanOption;

class GUICheckBox : public GUIOptionElement, public Observer
{
    // variables
    private:
        bool            mIsChecked; 
        Color           mTickColor;
        BooleanOption*  mBooleanOption;

        UInt32          mBoxSize;
        Rectangle2D     mBoxBounds;

        bool            mInitialUpdateDone;

        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

    // functions
    protected:
    public:
        GUICheckBox(UInt32      aID,
                    Rectangle2D aBounds);

        virtual ~GUICheckBox();

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

        // get/set tick color
        virtual void setTickColor(Color aColor);
        virtual Color getTickColor(void);

        // attach a boolean option to the checkbox
        virtual Status setBooleanOption(BooleanOption* aBooleanOption);

        // draws this GUICheckBox
        virtual Status draw(void);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a BooleanOption
        virtual Status update(const Observable* aOption);

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);
};
