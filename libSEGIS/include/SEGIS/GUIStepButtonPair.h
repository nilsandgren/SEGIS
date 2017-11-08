//
//  SEGIS - Settings and Event handling Graphical Interface System
//
//  Created by Nils Andgren 2005
//
//  GUIStepButtonPair:
//      Subclass to GUIElement
//      A pair of buttons for increasing/decreasing a numberoption by value step
//
//       -------
//      | ------|
//      ||  #  ||
//      || ### ||
//      | ------|
//      |-------|
//      | ------|
//      || ### ||
//      ||  #  ||
//      | ------|
//       -------
//

#pragma once

#include "Types.h"
#include "Events.h"
#include "Util.h"
#include "GraphicsUtil.h"
#include "GUIOptionElement.h"
#include "GUIButton.h"
#include "GUIStepButton.h"
#include "NumberOption.h"
#include "Observer.h"

#include <deque>

using namespace std;

class GUIStepButtonPair : public GUIOptionElement, public Observer
{
    // variables
    private:
        bool            mIsPressed[2];
        NumberOption*   mNumberOption; // option attached to this button pair
        UInt32          mButtonSize;   // size of each button
        Color           mArrowColor;
        GUIStepButton*  mButtons[2];

        SInt08 mPulseMax;
        SInt08 mPulseVal;
        SInt08 mPulseSign;

    // functions
    public:
        GUIStepButtonPair(UInt32        aID,
                          FillDirection aFillDirection);
                          

        virtual ~GUIStepButtonPair();

        // Take care of Events
        virtual EventSignal handleKeyEvent(KeyEvent*);
        virtual EventSignal handleMouseEvent(MouseEvent*);

        // draws this GUIStepButtonPair
        virtual Status draw(void);

        // might have to override setBounds to prevent trouble
        void setBounds(Rectangle2D aBounds);

        // the bevel of a button will always be positive
        virtual void setBevel(SInt32 aBevel);

        // override set padding, since it's not allowed
        virtual void setPadding(UInt32 /* aPadding */ )
        {
            fprintf(stderr, "GUIStepButtonPair::setPadding - padding not allowed.\n");
        };

        // attach an option
        virtual Status setNumberOption(NumberOption* aNumberOption);

        virtual Color getColor(void);
        virtual void setColor(Color aColor);

        // arrow Color
        virtual void setArrowColor(Color aColor);
        virtual Color getArrowColor(void);

        // implement GUIOptionElement interface
        virtual void updateFromOption()
        {
            // no action needed
        }

        virtual Status update(const Observable* /* aObservable */ )
        {
            // no action needed
            return kSucceeded;
        }

        virtual bool isInTabOrder(void);

        virtual bool updateAnimation(void);

        // simplified addChild since we dont have a parent Window in time
        Status addChild(GUIStepButton* aStepButton);
};
