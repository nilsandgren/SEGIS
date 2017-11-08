//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIKnob:
//      Subclass to GUIElement which can observe an Option
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "Observer.h"
#include <deque>

using namespace std;

class NumberOption;

class GUIKnob : public GUIOptionElement, public Observer
{
    // variables
    protected:
        Float32       mPositionFraction;         // position of slider 0.0 to 1.0
        NumberOption* mNumberOption;             // option attached to the slider
        bool          mInitialUpdateDone;

        Color         mHandleColor;
        Color         mHandleColorHighLight;
        Color         mHandleColorShadow;
        bool          mHandleColorIsSet;
        Color         mTickColor;

        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

        GLUquadric*   mQuadric;

    // functions
    private:
        // transforms between the slider position and attached option's value
        // floating point value is used since it covers all number options
        Float32 fractionToOptionValue(Float32 aFraction);
        Float32 optionValueToFraction(Float32 aOptionValue);

    public:
        GUIKnob(UInt32      aID,
                Rectangle2D aBounds);

        virtual ~GUIKnob();

        UInt32 getPadding(void)
        {
            return mPadding;
        }
        virtual void setPadding(UInt32 /* aPadding */ )
        {
            // padding not supported
            mPadding = 0;
        }

        // background color
        Color getColor(void);
        virtual void setColor(Color aColor);

        // knob color
        Color getHandleColor(void);
        virtual void setHandleColor(Color aHandleColor);

        // position marker color
        Color getTickColor(void);
        virtual void setTickColor(Color aColor);

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUIKnob
        virtual Status draw(void);

        // attach a NumberOption to the slider
        virtual Status setNumberOption(NumberOption* aNumberOption);

        virtual void updateFromOption();

        // implement the Observer interface
        // update state from an observable, which should be a NumberOption
        virtual Status update(const Observable* aOption);

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);
};
